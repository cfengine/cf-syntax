#include <parse_lib.h>
#include <stdlib.h>
#include <alloc.h>

int yyparse(void);

extern FILE *yyin;

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
