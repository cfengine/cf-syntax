#include <parse_lib.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h> // memcpy

int yyparse(void);
int yylex(void);
void print_token(int t);

extern FILE *yyin;
extern char *yytext;

Parser *parser;

Parser *NewParser()
{
    Parser *p = xcalloc(1, sizeof(Parser));
    p->policy = NewPolicyFile();
    return p;
}

PolicyFile *CloseParser()
{
    if (parser->errors != 0)
    {
        return NULL;
    }
    PolicyFile *policy = parser->policy;
    parser->policy = NULL;
    return policy;
}

PolicyFile *NewPolicyFile()
{
    return xcalloc(1, sizeof(PolicyFile));
}

void DestroyPolicyFile(PolicyFile *policy_file)
{
    free(policy_file);
}

PolicyFile *ParseFileStream(FILE *const input_file)
{
    parser = NewParser();
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
    parser = NewParser();

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
