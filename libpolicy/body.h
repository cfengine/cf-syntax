#ifndef CF_BODY_H
#define CF_BODY_H

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

Body *BodyNew();
void BodyDestroy(Body *body);
void BodyAppendAttribute(Body *body, Element *element);
void BodyPrint(Body *body);

Attribute *AttributeNew(const char *name);
void AttributeSetValue(Attribute *attribute, Element *value);
void AttributeDestroy(Attribute *attribute);

Element *ElementNewBody();
Element *ElementNewString(const char *string);
Element *ElementNewList();
Element *ElementNewAttribute(const char *name);
void ElementDestroy(Element *element);
Body *ElementToBody(Element *element);
void ElementAddChild(Element *parent, Element *child);

#endif
