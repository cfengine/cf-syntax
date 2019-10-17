#include <parse_lib.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h> // memcpy
#include <assert.h> // assert()

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

void DestroyParser(Parser *p)
{
    if (p != NULL)
    {
        DestroyPolicyFile(p->policy);
        free(p);
    }
}

PolicyFile *CloseParser(Parser *p)
{
    assert(p != NULL);
    if (p->errors != 0)
    {
        DestroyParser(p);
        return NULL;
    }
    PolicyFile *policy = p->policy;
    p->policy = NULL;
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

    return CloseParser(parser);
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

    return CloseParser(parser);
}

PolicyFile *ParseFile(const char *const path)
{
    FILE *file = fopen(path, "r");
    PolicyFile *policy = ParseFileStream(file);
    return policy;
}
