#ifndef __BOOL__
#define __BOOL__

typedef struct
{
    int id : 1;
} bool_t;

bool_t *bool_set(int id);
bool_t *bool_set_str(const char *str);

bool_t *bool_cpy(const bool_t *val);

void bool_disp(const bool_t *val);
void bool_free(bool_t *val);

bool_t *bool_equal(const bool_t *op1, const bool_t *op2);
bool_t *bool_nequal(const bool_t *op1, const bool_t *op2);

#endif /* __BOOL__ */
