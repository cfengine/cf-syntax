#ifndef CF_BODY_H
#define CF_BODY_H

typedef struct Body {
    char *type;
    char *name;
} Body;

Body *BodyNew();
void BodyDestroy(Body *body);

#endif
