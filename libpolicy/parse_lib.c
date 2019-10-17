#include <parse_lib.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h> // memcpy
#include <assert.h> // assert()

int yyparse(Parser *parser);
int yylex(Parser *parser);
void print_token(int t, const char *string);

extern FILE *yyin;
extern char *yytext;

Parser *NewParser()
{
    Parser *parser = xcalloc(1, sizeof(Parser));
    parser->policy = NewPolicyFile();
    return parser;
}

void DestroyParser(Parser *p)
{
    if (p != NULL)
    {
        DestroyPolicyFile(p->policy);
        free(p);
    }
}

PolicyFile *CloseParser(Parser *parser)
{
    assert(parser != NULL);
    if (parser->errors != 0)
    {
        DestroyParser(parser);
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
    Parser *parser = NewParser();
    yyin = input_file;
    if (yyin == NULL)
    {
        return NULL;
    }

    while (!feof(yyin))
    {
        yyparse(parser);

        if (ferror(yyin))
        {
            return NULL;
        }
    }

    fclose(yyin);

    return CloseParser(parser);
}

bool LexFileStream(FILE *const input_file)
{
    Parser *parser = NewParser();

    yyin = input_file;
    if (yyin == NULL)
    {
        return NULL;
    }

    while (!feof(yyin))
    {
        int ret = yylex(parser);
        while (ret != 0)
        {
            print_token(ret, yytext);
            ret = yylex(parser);
        }

        if (ferror(yyin))
        {
            return NULL;
        }
    }

    fclose(yyin);

    return CloseParser(parser);
}

PolicyFile *ParseFile(const char *const path)
{
    FILE *file = fopen(path, "r");
    PolicyFile *policy = ParseFileStream(file);
    return policy;
}
