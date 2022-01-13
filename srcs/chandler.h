#ifndef __CHANDLER__
#define __CHANDLER__

#include "term/operation.h"

struct __ctx handle_main_ctx(void);

val_t handle_set_int(const char *str, int base, struct __ctx *ctx);
val_t handle_set_float(const char *str, int base, struct __ctx *ctx);
val_t handle_set_cmpx(const char *str, int base, struct __ctx *ctx);
val_t handle_set_bool(int id, struct __ctx *ctx);
val_t handle_set_none(struct __ctx *ctx);
val_t handle_set_str(const char *str, struct __ctx *ctx);

void handle_free_int(int_t *val);
void handle_free_float(float_t *val);
void handle_free_cmpx(cmpx_t *val);
void handle_free_bool(bool_t *val);
void handle_free_str(str_t *val);

#endif /* __CHANDLER__ */
