#include <parse_lib.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h> // memcpy
#include <assert.h> // assert()

#include <cf_parser.h>
#include <cf_tokenizer.h>
#include <policy.h>

Parser *NewParser(const char *filename, FILE *output)
{
    Parser *parser = xcalloc(1, sizeof(Parser));
    parser->policy = NewPolicyFile(filename);
    parser->line_number = 1;
    parser->column_number = 1;
    parser->stack = StackNew(10, DestroyElement);
    parser->output = output;
    parser->output_column = 0;
    parser->insert_empty_line = false;
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

PolicyFile *NewPolicyFile(const char *filename)
{
    PolicyFile *r = xcalloc(1, sizeof(PolicyFile));
    r->name = xstrdup(filename);
    r->bodies = SeqNew(10, DestroyBody);
    return r;
}

void DestroyPolicyFile(PolicyFile *policy_file)
{
    if (policy_file != NULL)
    {
        SeqDestroy(policy_file->bodies);
        free(policy_file->name);
        free(policy_file);
    }
}

PolicyFile *ParseFileStream(
    FILE *const input_file, const char *name, FILE *const output)
{
    if (input_file == NULL)
    {
        return NULL;
    }
    if (name == NULL)
    {
        name = "(stdin)";
    }

    Parser *parser = NewParser(name, output);
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

bool LexFileStream(FILE *const input_file, const char *name)
{
    if (input_file == NULL)
    {
        return NULL;
    }
    if (name == NULL)
    {
        name = "(stdin)";
    }

    Parser *parser = NewParser(name, NULL);
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
    PolicyFile *policy = ParseFileStream(file, path, NULL);
    return policy;
}

static void FormatterNewline(Parser *p)
{
    if (p->output != NULL && p->output_column != 0)
    {
        fprintf(p->output, "\n");
        p->output_column = 0;
    }
}

static void FormatterEmptyLine(Parser *p)
{
    if (p->output != NULL)
    {
        p->insert_empty_line = true;
    }
}

static void FormatterWrite(Parser *p, const char *str)
{
    if (p->output != NULL)
    {
        if (p->insert_empty_line)
        {
            fprintf(p->output, "\n");
            p->output_column = 0;
            p->insert_empty_line = false;
        }
        p->output_column += fprintf(p->output, "%s", str);
    }
}

void ParserBeginBody(Parser *p)
{
    StackPush(p->stack, NewElementBody());
    FormatterNewline(p);
    FormatterWrite(p, "body ");
}

void ParserSetBodyType(Parser *p, const char *type)
{
    assert(p != NULL && type != NULL);

    Element *element = StackTop(p->stack);
    assert(element->type == TYPE_BODY);

    Body *body = element->body;
    assert(body->type == NULL);

    body->type = xstrdup(type);
    FormatterWrite(p, type);
    FormatterWrite(p, " ");
}

void ParserSetBodyName(Parser *p, const char *name)
{
    assert(p != NULL && name != NULL);

    Element *element = StackTop(p->stack);
    assert(element->type == TYPE_BODY);

    Body *body = element->body;
    assert(body->name == NULL);

    body->name = xstrdup(name);
    FormatterWrite(p, name);
    FormatterNewline(p);
    FormatterWrite(p, "{");
    FormatterNewline(p);
}

void ParserEndBody(Parser *p)
{
    assert(p != NULL);

    Element *element = StackPop(p->stack);
    assert(element != NULL);

    Body *body = ElementToBody(element);
    SeqAppend(p->policy->bodies, body);

    FormatterNewline(p);
    FormatterWrite(p, "}");
    FormatterNewline(p);
    FormatterEmptyLine(p);
}

void ParserBeginAttribute(Parser *parser, const char *string)
{
    Element *e = NewElementAttribute(string);
    StackPush(parser->stack, e);

    FormatterNewline(parser);
    FormatterWrite(parser, "  ");
    FormatterWrite(parser, string);
    FormatterWrite(parser, " => ");
}

void ParserEndAttribute(Parser *parser)
{
    Element *attribute = StackPop(parser->stack);
    Element *top = StackTop(parser->stack);
    AppendElement(top, attribute);

    FormatterWrite(parser, ";");
    FormatterNewline(parser);
}

void ParserAddString(Parser *parser, const char *string)
{
    Element *top = StackTop(parser->stack);
    ssize_t length = ChildrenLength(top);
    if (length > 0)
    {
        // List, but not first element
        FormatterWrite(parser, ", ");
    }
    else if (length == 0)
    {
        // First element of a list
        FormatterWrite(parser, " ");
    }

    Element *child = NewElementString(string);
    AppendElement(top, child);

    FormatterWrite(parser, string);
}

void ParserBeginList(Parser *parser)
{
    Element *top = NewElementList();
    StackPush(parser->stack, top);

    FormatterWrite(parser, "{");
}

void ParserEndList(Parser *parser)
{
    Element *list = StackPop(parser->stack);
    Element *top = StackTop(parser->stack);
    AppendElement(top, list);
    if (ChildrenLength(list) > 0)
    {
        FormatterWrite(parser, " ");
    }
    FormatterWrite(parser, "}");
}
