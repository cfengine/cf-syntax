#include <parse_lib.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h> // memcpy
#include <assert.h> // assert()

#include <cf_parser.h>
#include <cf_tokenizer.h>
#include <body.h>

Parser *NewParser(const char *filename)
{
    Parser *parser = xcalloc(1, sizeof(Parser));
    parser->policy = NewPolicyFile(filename);
    parser->line_number = 1;
    parser->column_number = 1;
    parser->stack = StackNew(10, ElementDestroy);
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
    r->bodies = SeqNew(10, BodyDestroy);
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

PolicyFile *ParseFileStream(FILE *const input_file, const char *name)
{
    if (input_file == NULL)
    {
        return NULL;
    }
    if (name == NULL)
    {
        name = "(stdin)";
    }

    Parser *parser = NewParser(name);
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

    Parser *parser = NewParser(name);
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
    PolicyFile *policy = ParseFileStream(file, path);
    return policy;
}

void ParserBeginBody(Parser *p)
{
    StackPush(p->stack, ElementNewBody());
}

void ParserSetBodyType(Parser *p, const char *type)
{
    assert(p != NULL && type != NULL);

    Element *element = StackTop(p->stack);
    assert(element->type == TYPE_BODY);

    Body *body = element->body;
    assert(body->type == NULL);

    body->type = xstrdup(type);
}

void ParserSetBodyName(Parser *p, const char *name)
{
    assert(p != NULL && name != NULL);

    Element *element = StackTop(p->stack);
    assert(element->type == TYPE_BODY);

    Body *body = element->body;
    assert(body->name == NULL);

    body->name = xstrdup(name);
}

void ParserEndBody(Parser *p)
{
    assert(p != NULL);

    Element *element = StackPop(p->stack);
    assert(element != NULL);

    Body *body = ElementToBody(element);
    SeqAppend(p->policy->bodies, body);
}

void ParserBeginAttribute(Parser *parser, const char *string)
{
    Element *e = ElementNewAttribute(string);
    StackPush(parser->stack, e);
}

void ParserEndAttribute(Parser *parser)
{
    Element *attribute = StackPop(parser->stack);
    Element *top = StackTop(parser->stack);
    ElementAddChild(top, attribute);
}

void ParserAddString(Parser *parser, const char *string)
{
    Element *top = StackTop(parser->stack);
    Element *child = ElementNewString(string);
    ElementAddChild(top, child);
}

void ParserBeginList(Parser *parser)
{
    Element *top = ElementNewList();
    StackPush(parser->stack, top);
}

void ParserEndList(Parser *parser)
{
    Element *list = StackPop(parser->stack);
    Element *top = StackTop(parser->stack);
    ElementAddChild(top, list);
}
