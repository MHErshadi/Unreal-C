#ifndef __VALUE__
#define __VALUE__

#include "value/str.h"
#include "value/list.h"
#include "value/tuple.h"
#include "context.h"

static const char *types_str[8] =
{
    "int",
    "float",
    "cmpx",
    "bool",
    "none",
    "str",
    "list",
    "tuple"
};

val_t set_val(enum val_t type, void *val, struct __ctx *ctx, pos_t poss, pos_t pose);
val_t cpy_val(val_t value);
void disp_val(val_t value, char end);
void free_val(val_t value);
void free_vals(val_t *vals, size_t vals_s);

int val_logic(val_t value);

int val_type(val_t value, enum val_t type);

#endif /* __VALUE__ */
