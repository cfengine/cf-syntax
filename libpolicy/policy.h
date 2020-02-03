#ifndef CF_POLICY_H
#define CF_POLICY_H

#include <sequence.h>

typedef enum Type {
    TYPE_UNKNOWN = 0,
    TYPE_STRING = 1,
    TYPE_LIST = 2,
    TYPE_BODY = 3,
    TYPE_ATTRIBUTE = 4,
} Type;

typedef struct Body {
    char *type;
    char *name;
    Seq *attributes;
} Body;

typedef struct Attribute {
    char *name;
    void *value; // Actually Element *
} Attribute;

typedef struct Element {
    Type type;
    union {
        char *string;
        Seq *list;
        Body *body;
        Attribute *attribute;
    };
} Element;

Body *NewBody();
void DestroyBody(Body *body);
void AppendBodyAttribute(Body *body, Element *element);
void PrintBody(Body *body);

Attribute *NewAttribute(const char *name);
void SetAttributeValue(Attribute *attribute, Element *value);
void DestroyAttribute(Attribute *attribute);

Element *NewElementBody();
Element *NewElementString(const char *string);
Element *NewElementList();
Element *NewElementAttribute(const char *name);
void DestroyElement(Element *element);
Body *ElementToBody(Element *element);
void AppendElement(Element *parent, Element *child);

#endif
