#ifndef __STR__
#define __STR__

#include <ustr.h>

#include "int.h"

typedef struct
{
    ustr_t str;
} str_t;

str_t *str_set(ustr_t str);
str_t *str_set_str(const char *str);

str_t *str_cpy(const str_t *val);

void str_disp(const str_t *val);
void str_free(str_t *val);

str_t *str_concat(const str_t *op1, const str_t *op2);
str_t *str_rem(const str_t *op1, const int_t *op2);
str_t *str_repeat(const str_t *op1, const int_t *op2);

str_t *str_reverse(const str_t *op);

bool_t *str_equal(const str_t *op1, const str_t *op2);
bool_t *str_nequal(const str_t *op1, const str_t *op2);

bool_t *str_has(const str_t *op1, const str_t *op2);

str_t *str_idx(const str_t *op1, const int_t *op2);

ustr_s str_len(const str_t *val);

int str_cmp(const str_t *op1, const str_t *op2);

#endif /* __STR__ */
