#ifndef __CONTEXT__
#define __CONTEXT__

#include "../core/pos.h"

#define VARS_S 64
#define PARS_S 16

enum val_t
{
    INT_V = 1,
    FLOAT_V,
    CMPX_V,
    BOOL_V,
    NONE_V,
    STR_V,
    LIST_V,
    TUPLE_V
};

typedef struct
{
    enum val_t type;
    void *val;

    struct __ctx *ctx;
    pos_t poss;
    pos_t pose;
} val_t;

typedef struct
{
    char *name;
    val_t val;

    int iscnt : 1;
    int isstc : 1;
} var_t;

typedef struct
{
    size_t vars_n;
    size_t vars_s;
    var_t *vars;

    size_t pars_n;
    size_t pars_s;
    var_t *pars;
} symtable_t;

struct __ctx
{
    const char *name;

    struct __ctx *prn;
    pos_t prn_pos;

    symtable_t table;
};

struct __ctx set_ctx(const char *name, struct __ctx *prn, pos_t prn_pos);
struct __ctx set_ctx_def(const char *name);
int getv_ctx(struct __ctx *ctx, var_t *var, int *isp, const char *name);

symtable_t init_sym(size_t vars_s, size_t pars_s);
symtable_t set_sym_def(struct __ctx *ctx);
void free_sym(symtable_t *table);

int get_sym(symtable_t table, var_t *var, int *isp, const char *name);
int gvar_sym(symtable_t table, var_t *var, const char *name);
int gpar_sym(symtable_t table, var_t *var, const char *name);

int svar_sym(symtable_t *table, const char *name, val_t val, int iscnt, int isstc);
int spar_sym(symtable_t *table, const char *name, val_t val, int iscnt, int isstc);

var_t set_var(const char *name, val_t val, int iscnt, int isstc);
void free_var(var_t var);

#define NAM(c) ((c)->name)
#define PRN(c) ((c)->prn)
#define PRNP(c) ((c)->prn_pos)
#define TBL(c) ((c)->table)

#define VARS(c) ((c).vars)
#define PARS(c) ((c).pars)

#define ICNT(c) ((c).iscnt)
#define ISTC(c) ((c).isstc)

#endif /* __CONTEXT__ */
