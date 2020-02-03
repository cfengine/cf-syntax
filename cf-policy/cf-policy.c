#include <parse_lib.h>
#include <string_lib.h> // StringMatchesOption()
#include <file_lib.h>   // safe_fopen()
#include <body.h>       // Body

int main(int argc, const char *const *argv)
{
    // Remove executable name from argc/argv:
    argc -= 1;
    argv += 1;

    // Parse remaining command line args:
    bool print_tokens = false;
    const char *filename = NULL;
    while (argc > 0)
    {
        if (StringMatchesOption(argv[0], "--tokens", "-t"))
        {
            print_tokens = true;
        }
        else
        {
            if (filename != NULL)
            {
                printf("Error: Too many file names!\n");
                return 1;
            }
            filename = argv[0];
        }
        argc -= 1;
        argv += 1;
    }

    // Open file if specified:
    FILE *file_stream = stdin;
    if (filename != NULL)
    {
        file_stream = safe_fopen(filename, "r");
    }
    int ret = 0;
    // Lex / Parse file:
    if (print_tokens)
    {
        // Only print tokens, don't check syntax:
        // (Uses only the flex generated lexer, not the yacc parser grammar)
        ret = LexFileStream(file_stream, filename) ? 0 : 1;
    }
    else
    {
        PolicyFile *policy = ParseFileStream(file_stream, filename);
        // Parse file, check syntax according to yacc grammar:
        if (policy != NULL)
        {
            printf("Syntax check: " GREEN "OK" RST "\n");

            stdout = safe_fopen(filename, "w");
            size_t length = SeqLength(policy->bodies);
            for (int i = 0; i < length; ++i)
            {
                Body *body = SeqAt(policy->bodies, i);
                BodyPrint(body);
                if (i != (length - 1))
                {
                    printf("\n");
                }
            }
        }
        else
        {
            printf("Syntax check: " RED "FAILED" RST "\n");
            ret = 1;
        }
        DestroyPolicyFile(policy);
    }
    return ret;
}
