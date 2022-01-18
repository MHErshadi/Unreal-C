#include <string.h>

#include "context.h"
#include "value.h"
#include "../macro.h"

struct __ctx set_ctx(const char *name, struct __ctx *prn, pos_t prn_pos)
{
    struct __ctx ctx;

    ctx.name = name;
    ctx.prn = prn;
    ctx.prn_pos = prn_pos;
    ctx.table = init_sym(VARS_S, PARS_S);

    return ctx;
}

struct __ctx set_ctx_def(const char *name)
{
    struct __ctx ctx;

    ctx.name = name;
    ctx.prn = NULL;
    ctx.prn_pos = NULL_POS;
    ctx.table = set_sym_def(&ctx);

    return ctx;
}

int getv_ctx(struct __ctx *ctx, var_t *var, int *isp, const char *name)
{
    int err = get_sym(TBL(ctx), var, isp, name);
    if (err && PRN(ctx))
        return getv_ctx(PRN(ctx), var, isp, name);

    return err;
}

symtable_t init_sym(unsigned long long vars_s, unsigned long long pars_s)
{
    symtable_t table;

    table.vars_n = 0;
    table.vars_s = vars_s;
    alloc(VARS(table), var_t, vars_s);

    table.pars_n = 0;
    table.pars_s = pars_s;
    alloc(PARS(table), var_t, pars_s);

    return table;
}

symtable_t set_sym_def(struct __ctx *ctx)
{
    symtable_t table = init_sym(VARS_S, PARS_S);

    int_t *null = int_set_ui(0);
    val_t nulll = set_val(INT_V, null, ctx, NULL_POS, NULL_POS);
    svar_sym(&table, "null", nulll, 1, 0);

    str_t *empt = str_set_str("");
    val_t emptl = set_val(STR_V, empt, ctx, NULL_POS, NULL_POS);
    svar_sym(&table, "empty", emptl, 1, 0);

    return table;
}

void free_sym(symtable_t *table)
{
    unsigned long long i, lstc = 0;

    for (i = 0; i < table->vars_n; i++)
    {
        var_t var = VARS(*table)[i];
        if (var.isstc)
        {
            lstc = i + 1;
            continue;
        }
        free_var(var);
    }
    table->vars_n = lstc;
    table->vars_s = lstc;
    ralloc(VARS(*table), var_t, table->vars_n);

    lstc = 0;

    for (i = 0; i < table->pars_n; i++)
    {
        var_t var = PARS(*table)[i];
        if (var.isstc)
        {
            lstc = i + 1;
            continue;
        }
        free_var(var);
    }
    table->pars_n = lstc;
    table->pars_s = lstc;
    ralloc(PARS(*table), var_t, table->pars_n);
}

int get_sym(symtable_t table, var_t *var, int *isp, const char *name)
{
    int err = gvar_sym(table, var, name);
    if (err)
    {
        err = gpar_sym(table, var, name);
        if (isp)
            *isp = 1;
    }

    return err;
}

int gvar_sym(symtable_t table, var_t *var, const char *name)
{
    unsigned long long i;
    for (i = 0; i < table.vars_n; i++)
    {
        var_t vari = VARS(table)[i];

        if (!vari.name)
            continue;
        if (!strcmp(vari.name, name))
        {
            *var = vari;
            return 0;
        }
    }

    return 1;
}

int gpar_sym(symtable_t table, var_t *var, const char *name)
{
    unsigned long long i;
    for (i = 0; i < table.pars_n; i++)
    {
        var_t vari = PARS(table)[i];

        if (!vari.name)
            continue;
        if (!strcmp(vari.name, name))
        {
            *var = vari;
            return 0;
        }
    }

    return 1;
}

int svar_sym(symtable_t *table, const char *name, val_t val, int iscnt, int isstc)
{
    unsigned long long i, lstf = table->vars_n;
    for (i = 0; i < table->vars_n; i++)
    {
        var_t vari = table->vars[i];

        if (!vari.name)
        {
            lstf = i;
            continue;
        }
        if (!strcmp(vari.name, name))
        {
            if (ICNT(vari))
                return 1;

            free_val(VAL(vari));

            VAL(vari) = val;
            ICNT(vari) = iscnt;
            ISTC(vari) = isstc;

            table->vars[i] = vari;
            return 0;
        }
    }

    if (table->vars_s == lstf)
        ralloc(table->vars, var_t, table->vars_s += VARS_S);

    table->vars[lstf] = set_var(name, val, iscnt, isstc);
    table->vars_n++;
    return 0;
}

int spar_sym(symtable_t *table, const char *name, val_t val, int iscnt, int isstc)
{
    unsigned long long i, lstf = table->pars_n;
    for (i = 0; i < table->pars_n; i++)
    {
        var_t vari = table->pars[i];

        if (!vari.name)
        {
            lstf = i;
            continue;
        }
        if (!strcmp(vari.name, name))
        {
            if (ICNT(vari))
                return 1;

            free_val(VAL(vari));

            VAL(vari) = val;
            ICNT(vari) = iscnt;
            ISTC(vari) = isstc;

            table->pars[i] = vari;
            return 0;
        }
    }

    if (table->pars_s == lstf)
        ralloc(table->pars, var_t, table->pars_s += VARS_S);

    table->pars[lstf] = set_var(name, val, iscnt, isstc);
    table->pars_n++;
    return 0;
}

int dvar_sym(symtable_t *table, const char *name)
{
    unsigned long long i;
    for (i = 0; i < table->vars_n; i++)
    {
        var_t vari = table->vars[i];

        if (!vari.name)
            continue;
        if (!strcmp(vari.name, name))
        {
            fre(vari.name);
            free_val(VAL(vari));
            return 0;
        }
    }

    return 1;
}

int dpar_sym(symtable_t *table, const char *name)
{
    unsigned long long i;
    for (i = 0; i < table->pars_n; i++)
    {
        var_t vari = table->pars[i];

        if (!vari.name)
            continue;
        if (!strcmp(vari.name, name))
        {
            fre(vari.name);
            free_val(VAL(vari));
            return 0;
        }
    }

    return 1;
}

var_t set_var(const char *name, val_t val, int iscnt, int isstc)
{
    char *name_c;
    alloc(name_c, char, strlen(name) + 1);
    strcpy(name_c, name);

    var_t var;

    var.name = name_c;
    VAL(var) = val;
    ICNT(var) = iscnt;
    ISTC(var) = isstc;

    return var;
}

void free_var(var_t var)
{
    fre(var.name);
    free_val(VAL(var));
}
