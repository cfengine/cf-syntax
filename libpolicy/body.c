#include <body.h>
#include <alloc.h>

Body *BodyNew()
{
    return xcalloc(1, sizeof(Body));
}

void BodyDestroy(Body *body)
{
    if (body != NULL)
    {
        free(body->type);
        free(body->name);
        free(body);
    }
}
