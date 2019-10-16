#include <parse_lib.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h> // memcpy

int yyparse(void);
int yylex(void);
void print_token(int t);

extern FILE *yyin;
extern char *yytext;

ParserState parser_state;

PolicyFile *ParseFileStream(FILE *const input_file)
{
    yyin = input_file;
    if (yyin == NULL)
    {
        return NULL;
    }

    while (!feof(yyin))
    {
        yyparse();

        if (ferror(yyin))
        {
            return NULL;
        }
    }

    fclose(yyin);

    return CloseParser();
}

bool LexFileStream(FILE *const input_file)
{
    yyin = input_file;
    if (yyin == NULL)
    {
        return NULL;
    }

    while (!feof(yyin))
    {
        int ret = yylex();
        while (ret != 0)
        {
            print_token(ret);
            ret = yylex();
        }

        if (ferror(yyin))
        {
            return NULL;
        }
    }

    fclose(yyin);

    return CloseParser();
}

PolicyFile *ParseFile(const char *const path)
{
    FILE *file = fopen(path, "r");
    PolicyFile *policy = ParseFileStream(file);
    return policy;
}

PolicyFile *CloseParser()
{
    if (parser_state.errors != 0)
    {
        return NULL;
    }
    PolicyFile *policy = xmalloc(sizeof(PolicyFile));
    memcpy(policy, &(parser_state.policy), sizeof(PolicyFile));
    return policy;
}
