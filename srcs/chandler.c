#include "chandler.h"
#include "unreal.h"

struct __ctx handle_main_ctx(void)
{
    return set_ctx_def(DEF_CTX);
}

val_t handle_set_int(const char *str, int base, struct __ctx *ctx)
{
    int_t *val = int_set_str(str, base);
    val_t value = set_val(INT_V, val, ctx, NULL_POS, NULL_POS);

    return value;
}

val_t handle_set_float(const char *str, int base, struct __ctx *ctx)
{
    float_t *val = float_set_str(str, base);
    val_t value = set_val(FLOAT_V, val, ctx, NULL_POS, NULL_POS);

    return value;
}

val_t handle_set_cmpx(const char *str, int base, struct __ctx *ctx)
{
    cmpx_t *val = cmpx_set_str(str, base);
    val_t value = set_val(CMPX_V, val, ctx, NULL_POS, NULL_POS);

    return value;
}

val_t handle_set_bool(int id, struct __ctx *ctx)
{
    bool_t *val = bool_set(id);
    val_t value = set_val(BOOL_V, val, ctx, NULL_POS, NULL_POS);

    return value;
}

val_t handle_set_none(struct __ctx *ctx)
{
    val_t value = set_val(NONE_V, NULL, ctx, NULL_POS, NULL_POS);

    return value;
}

val_t handle_set_str(const char *str, struct __ctx *ctx)
{
    str_t *val = str_set_str(str);
    val_t value = set_val(STR_V, val, ctx, NULL_POS, NULL_POS);

    return value;
}

void handle_free_int(int_t *val)
{
    int_free(val);
}

void handle_free_float(float_t *val)
{
    float_free(val);
}

void handle_free_cmpx(cmpx_t *val)
{
    cmpx_free(val);
}

void handle_free_bool(bool_t *val)
{
    bool_free(val);
}

void handle_free_str(str_t *val)
{
    str_free(val);
}
