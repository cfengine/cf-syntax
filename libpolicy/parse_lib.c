#include <parse_lib.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h> // memcpy
#include <assert.h> // assert()

#include <cf_parser.h>
#include <cf_tokenizer.h>

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
    if (input_file == NULL)
    {
        return NULL;
    }

    Parser *parser = NewParser();
    yyscan_t scanner;
    yylex_init(&scanner);
    yyset_in(input_file, scanner);

    while (!feof(input_file))
    {
        yyparse(scanner, parser);

        if (ferror(input_file))
        {
            return NULL;
        }
    }
    yylex_destroy(scanner);

    fclose(input_file);

    return CloseParser(parser);
}

bool LexFileStream(FILE *const input_file)
{
    if (input_file == NULL)
    {
        return NULL;
    }

    Parser *parser = NewParser();
    yyscan_t scanner;
    yylex_init(&scanner);
    yyset_in(input_file, scanner);

    char *token = NULL;
    while (!feof(input_file))
    {
        int ret = yylex(&token, scanner, parser);

        while (ret != 0)
        {
            print_token(ret, token, parser);
            token = NULL;
            ret = yylex(&token, scanner, parser);
        }
        token = NULL;

        if (ferror(input_file))
        {
            return NULL;
        }
    }

    yylex_destroy(scanner);

    fclose(input_file);

    return CloseParser(parser);
}

PolicyFile *ParseFile(const char *const path)
{
    FILE *file = fopen(path, "r");
    PolicyFile *policy = ParseFileStream(file);
    return policy;
}
