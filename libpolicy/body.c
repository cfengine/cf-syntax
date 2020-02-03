#include <body.h>
#include <alloc.h>
#include <condition_macros.h>

Attribute *AttributeNew(const char *name)
{
    Attribute *a = xcalloc(1, sizeof(Attribute));
    a->name = xstrdup(name);
    return a;
}

void AttributeSetValue(Attribute *attribute, Element *value)
{
    assert(attribute != NULL && value != NULL);
    assert(attribute->value == NULL);
    attribute->value = value;
}

void AttributeDestroy(Attribute *attribute)
{
    if (attribute != NULL)
    {
        ElementDestroy(attribute->value);
        free(attribute->name);
        free(attribute);
    }
}

Body *BodyNew()
{
    Body *body = xcalloc(1, sizeof(Body));
    body->attributes = SeqNew(8, ElementDestroy);
    return body;
}

void BodyAppendAttribute(Body *body, Element *element)
{
    assert(body != NULL && element != NULL);
    assert(element->type == TYPE_ATTRIBUTE);
    SeqAppend(body->attributes, element);
}

void BodyDestroy(Body *body)
{
    if (body != NULL)
    {
        SeqDestroy(body->attributes);
        free(body->type);
        free(body->name);
        free(body);
    }
}

Element *ElementNewBody()
{
    Element *v = xcalloc(1, sizeof(Element));
    v->type = TYPE_BODY;
    v->body = BodyNew();
    return v;
}

Element *ElementNewString(const char *string)
{
    Element *v = xcalloc(1, sizeof(Element));
    v->type = TYPE_STRING;
    v->string = xstrdup(string);
    return v;
}

Element *ElementNewList()
{
    Element *e = xcalloc(1, sizeof(Element));
    e->type = TYPE_LIST;
    e->list = SeqNew(8, ElementDestroy);
    return e;
}

Element *ElementNewAttribute(const char *name)
{
    Element *e = xcalloc(1, sizeof(Element));
    e->type = TYPE_ATTRIBUTE;
    e->attribute = AttributeNew(name);
    return e;
}

void ElementDestroy(Element *value)
{
    if (value != NULL)
    {
        switch (value->type)
        {
        case TYPE_STRING:
            free(value->string);
            break;
        case TYPE_LIST:
            SeqDestroy(value->list);
            break;
        case TYPE_BODY:
            BodyDestroy(value->body);
            break;
        case TYPE_ATTRIBUTE:
            AttributeDestroy(value->attribute);
            break;
        default:
            debug_abort_if_reached();
            break;
        }
        free(value);
    }
}

Body *ElementToBody(Element *element)
{
    assert(element != NULL && element->type == TYPE_BODY);
    Body *body = element->body;
    free(element);
    return body;
}

void ElementAddChild(Element *parent, Element *child)
{
    assert(parent != NULL && child != NULL);

    Type t = parent->type;

    switch (t)
    {
    case TYPE_LIST:
        SeqAppend(parent->list, child);
        break;
    case TYPE_ATTRIBUTE:
        AttributeSetValue(parent->attribute, child);
        break;
    case TYPE_BODY:
        BodyAppendAttribute(parent->body, child);
        break;
    case TYPE_UNKNOWN:
    case TYPE_STRING:
    default:
        debug_abort_if_reached();
    }
}

void PrintList(Element *element)
{
    assert(element != NULL);
    assert(element->type == TYPE_LIST);
    Seq *list = element->list;
    size_t length = SeqLength(list);
    printf("{");
    if (length != 0)
    {
        printf(" ");
    }
    for (int i = 0; i < length; ++i)
    {
        Element *current = SeqAt(list, i);
        assert(current->type == TYPE_STRING);
        printf("%s", current->string);
        if (i != length - 1)
        {
            printf(", ");
        }
    }

    if (length != 0)
    {
        printf(" ");
    }
    printf("}");
}

void BodyPrint(Body *body)
{
    printf("body %s %s\n{\n", body->type, body->name);
    size_t length = SeqLength(body->attributes);
    for (int i = 0; i < length; ++i)
    {
        Element *element = SeqAt(body->attributes, i);
        assert(element->type == TYPE_ATTRIBUTE);
        Attribute *attr = element->attribute;
        Element *value = attr->value;
        printf("  %s => ", attr->name);
        switch (value->type)
        {
        case TYPE_STRING:
            printf("%s", value->string);
            break;
        case TYPE_LIST:
            PrintList(value);
            break;
        default:
            debug_abort_if_reached();
        }
        if (i == (length - 1))
        {
            printf(";");
        }
        else
        {
            printf(",");
        }
        printf("\n");
    }
    printf("}\n");
}
