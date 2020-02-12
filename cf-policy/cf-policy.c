#include <parse_lib.h>
#include <string_lib.h> // StringMatchesOption()
#include <file_lib.h>   // safe_fopen()
#include <policy.h>     // Body
#include <unistd.h>     // unlink()

static void RenderPolicy(PolicyFile *policy, const char *filename)
{
    assert(policy != NULL);

    stdout = safe_fopen(filename, "w");
    size_t length = SeqLength(policy->bodies);
    for (int i = 0; i < length; ++i)
    {
        Body *body = SeqAt(policy->bodies, i);
        PrintBody(body);
        if (i != (length - 1))
        {
            printf("\n");
        }
    }
}

int main(int argc, const char *const *argv)
{
    // Remove executable name from argc/argv:
    argc -= 1;
    argv += 1;

    // Parse remaining command line args:
    bool print_tokens = false;
    const char *filename = NULL;
    const char *render_file = NULL;
    bool format = false;
    while (argc > 0)
    {
        if (StringMatchesOption(argv[0], "--format", "-f"))
        {
            format = true;
        }
        else if (StringMatchesOption(argv[0], "--render", "-r"))
        {
            if (argc < 2 || argv[1][0] == '-')
            {
                printf("Error: --render requires an output file name!\n");
                return 1;
            }
            argc -= 1;
            argv += 1;
            render_file = argv[0];
        }
        else if (StringMatchesOption(argv[0], "--tokens", "-t"))
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
    FILE *input_stream = stdin;
    if (filename != NULL)
    {
        input_stream = safe_fopen(filename, "r");
    }

    FILE *output = NULL;
    char tmp_file[1024];
    if (format)
    {
        snprintf(tmp_file, sizeof(tmp_file), "%s.tmp-format.cf", filename);
        output = safe_fopen(tmp_file, "w");
    }

    int ret;
    PolicyFile *policy = NULL;

    // Lex / Parse file:
    if (print_tokens)
    {
        // Only print tokens, don't check syntax:
        // (Uses only the flex generated lexer, not the yacc parser grammar)
        ret = LexFileStream(input_stream, filename) ? 0 : 1;
        goto after_syntax_check;
    }
    policy = ParseFileStream(input_stream, filename, output);
    // Parse file, check syntax according to yacc grammar:
    if (policy != NULL)
    {
        printf("Syntax check: " GREEN "OK" RST "\n");
        ret = 0;
    }
    else
    {
        printf("Syntax check: " RED "FAILED" RST "\n");
        ret = 1;
    }

after_syntax_check:
    if (input_stream != stdin)
    {
        fclose(input_stream);
    }
    if (output != NULL)
    {
        fclose(output);
    }

    if (ret == 0 && format && filename != NULL)
    {
        unlink(filename);
        rename(tmp_file, filename);
    }

    if (render_file != NULL && policy != NULL)
    {
        RenderPolicy(policy, render_file);
    }

    DestroyPolicyFile(policy);

    return ret;
}
