#ifndef __VALUE__
#define __VALUE__

#include "context.h"

typedef struct
{
    val_t key;
    val_t val;
} kval_t;

#include "value/str.h"
#include "value/list.h"
#include "value/tuple.h"
#include "value/dict.h"
#include "value/set.h"

static const char *types_str[10] =
{
    "int",
    "float",
    "cmpx",
    "bool",
    "none",
    "str",
    "list",
    "tuple",
    "dict",
    "set"
};

val_t set_val(enum val_t type, void *val, struct __ctx *ctx, pos_t poss, pos_t pose);
val_t cpy_val(val_t value);
void disp_val(val_t value, char end);
void free_val(val_t value);
void free_vals(val_t *vals, unsigned long long vals_s);

int val_logic(val_t value);
int val_type(val_t value, enum val_t type);
int vals_has(const val_t *vals, unsigned long long vals_s, val_t val);

kval_t set_kval(val_t key, val_t val);
kval_t cpy_kval(kval_t value);
void free_kval(kval_t kval);
void free_kvals(kval_t *kvals, unsigned long long kvals_s);

int kvals_has(const kval_t *kvals, unsigned long long kvals_s, val_t key);
int kvals_haskv(const kval_t *kvals, unsigned long long kvals_s, kval_t kval);

#endif /* __VALUE__ */
