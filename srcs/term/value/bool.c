#include <stdio.h>
#include <string.h>

#include "bool.h"
#include "../../macro.h"
#include "valm.h"

bool_t *bool_set(int id)
{
    bool_t *val;
    alloc(val, bool_t, 1);

    ID(val) = id;

    return val;
}

bool_t *bool_set_str(const char *str)
{
    return bool_set(!strcmp(str, "true"));
}

bool_t *bool_cpy(const bool_t *val)
{
    return bool_set(ID(val));
}

void bool_disp(const bool_t *val)
{
    printf(ID(val) ? "true" : "false");
}

void bool_free(bool_t *val)
{
    fre(val);
}

bool_t *bool_equal(const bool_t *op1, const bool_t *op2)
{
    return bool_set(ID(op1) == ID(op2));
}

bool_t *bool_nequal(const bool_t *op1, const bool_t *op2)
{
    return bool_set(ID(op1) != ID(op2));
}
