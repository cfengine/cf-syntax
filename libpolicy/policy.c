#include <policy.h>
#include <alloc.h>
#include <condition_macros.h>

Attribute *NewAttribute(const char *name)
{
    Attribute *a = xcalloc(1, sizeof(Attribute));
    a->name = xstrdup(name);
    return a;
}

void SetAttributeValue(Attribute *attribute, Element *value)
{
    assert(attribute != NULL && value != NULL);
    assert(attribute->value == NULL);
    attribute->value = value;
}

void DestroyAttribute(Attribute *attribute)
{
    if (attribute != NULL)
    {
        DestroyElement(attribute->value);
        free(attribute->name);
        free(attribute);
    }
}

Body *NewBody()
{
    Body *body = xcalloc(1, sizeof(Body));
    body->attributes = SeqNew(8, DestroyElement);
    return body;
}

void AppendBodyAttribute(Body *body, Element *element)
{
    assert(body != NULL && element != NULL);
    assert(element->type == TYPE_ATTRIBUTE);
    SeqAppend(body->attributes, element);
}

void DestroyBody(Body *body)
{
    if (body != NULL)
    {
        SeqDestroy(body->attributes);
        free(body->type);
        free(body->name);
        free(body);
    }
}

Element *NewElementBody()
{
    Element *v = xcalloc(1, sizeof(Element));
    v->type = TYPE_BODY;
    v->body = NewBody();
    return v;
}

Element *NewElementString(const char *string)
{
    Element *v = xcalloc(1, sizeof(Element));
    v->type = TYPE_STRING;
    v->string = xstrdup(string);
    return v;
}

Element *NewElementList()
{
    Element *e = xcalloc(1, sizeof(Element));
    e->type = TYPE_LIST;
    e->list = SeqNew(8, DestroyElement);
    return e;
}

Element *NewElementAttribute(const char *name)
{
    Element *e = xcalloc(1, sizeof(Element));
    e->type = TYPE_ATTRIBUTE;
    e->attribute = NewAttribute(name);
    return e;
}

void DestroyElement(Element *value)
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
            DestroyBody(value->body);
            break;
        case TYPE_ATTRIBUTE:
            DestroyAttribute(value->attribute);
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

void AppendElement(Element *parent, Element *child)
{
    assert(parent != NULL && child != NULL);

    Type t = parent->type;

    switch (t)
    {
    case TYPE_LIST:
        SeqAppend(parent->list, child);
        break;
    case TYPE_ATTRIBUTE:
        SetAttributeValue(parent->attribute, child);
        break;
    case TYPE_BODY:
        AppendBodyAttribute(parent->body, child);
        break;
    case TYPE_UNKNOWN:
    case TYPE_STRING:
    default:
        debug_abort_if_reached();
    }
}

ssize_t ChildrenLength(Element *parent)
{
    assert(parent != NULL);

    const Type t = parent->type;

    if (t != TYPE_LIST)
    {
        return -1;
    }

    return SeqLength(parent->list);
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

void PrintBody(Body *body)
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
