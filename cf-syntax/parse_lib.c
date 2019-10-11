#include <parse_lib.h>

int yyparse(void);

extern FILE *yyin;

bool ParseFile(FILE *input_file)
{
    yyin = input_file;
    if (yyin == NULL)
    {
        return false;
    }

    while (!feof(yyin))
    {
        yyparse();

        if (ferror(yyin))
        {
            return false;
        }
    }

    fclose(yyin);
    return true;
}
