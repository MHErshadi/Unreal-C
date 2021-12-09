#ifndef __INTERPRETER__
#define __INTERPRETER__

#include "core/node.h"
#include "term/value.h"
#include "errlib/errlib.h"

typedef struct
{
    val_t val;

    int haserr : 1;
    runtime_err_t error;
} vres_t;

vres_t visit(node_t node, struct __ctx *ctx, int shf);

#endif /* __INTERPRETER__ */
