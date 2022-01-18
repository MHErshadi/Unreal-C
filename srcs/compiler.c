#include <string.h>

#include "compiler.h"
#include "term/operation.h"
#include "macro.h"

#define DEF_MEMS_S 32
#define DEF_MEMS_D 2

#define NULL_FUNCS ((funcs_t){0, NULL})

struct __strs
{
    char *incs;
    char *deps;
    char *defs;
    char *main;
    char *end_main;
    char *funcs_head;
};

struct __def_mem
{
    unsigned id;

    enum val_t type;
    char *val;
};

struct __def_mems
{
    unsigned long long dma;
    unsigned long long dms;

    struct __def_mem *mems;
};

struct __strs conv_cres_to_strs(cres_t other);
void free_strs(struct __strs strs);

struct __def_mem set_def_mem(unsigned id, enum val_t type, char *val);
void free_def_mem(struct __def_mem def_mem);
int cmp_def_mem(struct __def_mem def_mem, enum val_t type, const char *val);
unsigned find_def_mem(struct __def_mems def_mems, enum val_t type, const char *val);

struct __def_mems set_def_mems(unsigned long long dma, unsigned long long dms, struct __def_mem *mems);
void free_def_mems(struct __def_mems def_mems);

void comp_succ(cres_t *res, char *incs, char *deps, char *defs, char *main, char *end_main, char *funcs_head, funcs_t funcs);
void comp_fail(cres_t *res, runtime_err_t error);
struct __strs reg_comp_res(cres_t *res, cres_t other);

char *comp_incs(void);
char *comp_deps(void);
char *comp_defs(void);
char *comp_main(void);
char *comp_funcs_head(void);
funcs_t comp_funcs(void);

cres_t comp_node(node_t node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id);
cres_t comp_int(int_nd *node, funcs_t *funcs, struct __def_mems *def_mems);
cres_t comp_float(float_nd *node, funcs_t *funcs, struct __def_mems *def_mems);
cres_t comp_cmpx(cmpx_nd *node, funcs_t *funcs, struct __def_mems *def_mems);
cres_t comp_bool(bool_nd *node);
cres_t comp_none(void);
cres_t comp_str(str_nd *node, funcs_t *funcs, struct __def_mems *def_mems);
cres_t comp_list(list_nd *node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id);
cres_t comp_tuple(tuple_nd *node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id);
cres_t comp_dict(dict_nd *node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id);
cres_t comp_set(set_nd *node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id);
cres_t comp_bin_op(bin_op_nd *node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id);

int ull_digits(unsigned long long number);

cres_t compile(node_t node)
{
    unsigned long long dma = DEF_MEMS_S;
    struct __def_mem *mems;
    alloc(mems, struct __def_mem, dma);

    struct __def_mems def_mems = set_def_mems(dma, 0, mems);

    funcs_t funcs = comp_funcs();

    unsigned val_id = 0;

    cres_t comp_res = comp_node(node, &funcs, &def_mems, &val_id);

    free_def_mems(def_mems);

    if (HERR(comp_res))
    {
        free_funcs(funcs);

        cres_t res;
        comp_fail(&res, ERR(comp_res));
        return res;
    }

    char *incs = comp_incs();
    char *deps = comp_deps();
    char *defs = comp_defs();
    char *main = comp_main();
    char *funcs_head = comp_funcs_head();

    if (comp_res.incs)
    {
        ralloc(incs, char, strlen(incs) + strlen(comp_res.incs) + 1);
        strcat(incs, comp_res.incs);

        fre(comp_res.incs);
    }

    if (comp_res.deps)
    {
        ralloc(deps, char, strlen(deps) + strlen(comp_res.deps) + 1);
        strcat(deps, comp_res.deps);
    
        fre(comp_res.deps);
    }

    if (comp_res.defs)
    {
        ralloc(defs, char, strlen(defs) + strlen(comp_res.defs) + 1);
        strcat(defs, comp_res.defs);

        fre(comp_res.defs);
    }

    if (comp_res.main)
    {
        ralloc(main, char, strlen(main) + strlen(comp_res.main) + 1);
        strcat(main, comp_res.main);

        fre(comp_res.main);
    }

    if (comp_res.end_main)
    {
        ralloc(main, char, strlen(main) + strlen(comp_res.end_main) + 24);
        sprintf(main, "%s;%s\ncf_gc();\nreturn 0;\n}\n", main, comp_res.end_main);
    }
    else
    {
        ralloc(main, char, strlen(main) + 24);
        strcat(main, ";\ncf_gc();\nreturn 0;\n}\n");
    }

    ralloc(funcs.strs[0], char, strlen(funcs.strs[0]) + 3);
    strcat(funcs.strs[0], "}\n");

    cres_t res;
    comp_succ(&res, incs, deps, defs, main, NULL, funcs_head, funcs);
    return res;
}

funcs_t set_funcs(unsigned long long size, char **strs)
{
    funcs_t funcs;
    funcs.size = size;
    funcs.strs = strs;

    return funcs;
}

void disp_funcs(funcs_t funcs)
{
    unsigned long long i;
    for (i = 0; i < funcs.size; i++)
        printf("%s\n", funcs.strs[i]);
}

void free_funcs(funcs_t funcs)
{
    unsigned long long i;
    for (i = 0; i < funcs.size; i++)
        fre(funcs.strs[i]);

    fre(funcs.strs);
}

struct __strs conv_cres_to_strs(cres_t other)
{
    struct __strs strs;
    strs.incs = other.incs;
    strs.deps = other.deps;
    strs.defs = other.defs;
    strs.main = other.main;
    strs.end_main = other.end_main;
    strs.funcs_head = other.funcs_head;

    return strs;
}

void free_strs(struct __strs strs)
{
    if (strs.incs)
        fre(strs.incs);

    if (strs.deps)
        fre(strs.deps);

    if (strs.defs)
        fre(strs.defs);

    if (strs.main)
        fre(strs.main);

    if (strs.end_main)
        fre(strs.end_main);

    if (strs.funcs_head)
        fre(strs.funcs_head);
}

struct __def_mem set_def_mem(unsigned id, enum val_t type, char *val)
{
    struct __def_mem def_mem;
    def_mem.id = id;
    TYP(def_mem) = type;
    VAL(def_mem) = val;

    return def_mem;
}

void free_def_mem(struct __def_mem def_mem)
{
    fre(VAL(def_mem));
}

int cmp_def_mem(struct __def_mem def_mem, enum val_t type, const char *val)
{
    return TYP(def_mem) == type && !strcmp(VAL(def_mem), val);
}

unsigned find_def_mem(struct __def_mems def_mems, enum val_t type, const char *val)
{
    unsigned long long i;
    for (i = 0; i < def_mems.dms; i++)
        if (cmp_def_mem(def_mems.mems[i], type, val))
            return def_mems.mems[i].id;

    return 0;
}

struct __def_mems set_def_mems(unsigned long long dma, unsigned long long dms, struct __def_mem *mems)
{
    struct __def_mems def_mems;
    def_mems.dma = dma;
    def_mems.dms = dms;
    def_mems.mems = mems;

    return def_mems;
}

void free_def_mems(struct __def_mems def_mems)
{
    unsigned long long i;
    for (i = 0; i < def_mems.dms; i++)
        free_def_mem(def_mems.mems[i]);

    fre(def_mems.mems);
}

void comp_succ(cres_t *res, char *incs, char *deps, char *defs, char *main, char *end_main, char *funcs_head, funcs_t funcs)
{
    HERR(*res) = 0;
    res->incs = incs;
    res->deps = deps;
    res->defs = defs;
    res->main = main;
    res->end_main = end_main;
    res->funcs_head = funcs_head;
    res->funcs = funcs;
}

void comp_fail(cres_t *res, runtime_err_t error)
{
    HERR(*res) = 1;
    ERR(*res) = error;
}

struct __strs reg_comp_res(cres_t *res, cres_t other)
{
    if (HERR(other))
    {
        HERR(*res) = 1;
        ERR(*res) = ERR(other);
    }

    return conv_cres_to_strs(other);
}

char *comp_incs(void)
{
    char *incs;
    alloc(incs, char, 1);

    incs[0] = '\0';

    return incs;
}

char *comp_deps(void)
{
    char *deps;
    alloc(deps, char, 41);

    strcpy(deps, "struct __ctx c_ctx = handle_main_ctx();\n");

    return deps;
}

char *comp_defs(void)
{
    char *defs;
    alloc(defs, char, 121);

    strcpy(defs, "val_t cd_0 = handle_set_none(&c_ctx);\n");
    strcat(defs, "val_t cd_1 = handle_set_bool(1, &c_ctx);\n");
    strcat(defs, "val_t cd_2 = handle_set_bool(0, &c_ctx);\n");

    return defs;
}

char *comp_main(void)
{
    char *main;
    alloc(main, char, 35);

    strcpy(main, "int main(int argc, char **argv) {\n");

    return main;
}

char *comp_funcs_head(void)
{
    char *funcs_head;
    alloc(funcs_head, char, 15);

    strcpy(funcs_head, "void cf_gc();\n");

    return funcs_head;
}

funcs_t comp_funcs(void)
{
    char **strs;
    alloc(strs, char*, 1);

    alloc(strs[0], char, 92);

    strcpy(strs[0], "void cf_gc() {\nhandle_free_bool((bool_t*)VAL(cd_1));\nhandle_free_bool((bool_t*)VAL(cd_2));\n");

    funcs_t funcs = set_funcs(1, strs);
    return funcs;
}

cres_t comp_node(node_t node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id)
{
    switch (TYP(node))
    {
    case INT_N:
        return comp_int((int_nd*)NOD(node), funcs, def_mems);
    case FLOAT_N:
        return comp_float((float_nd*)NOD(node), funcs, def_mems);
    case CMPX_N:
        return comp_cmpx((cmpx_nd*)NOD(node), funcs, def_mems);
    case BOOL_N:
        return comp_bool((bool_nd*)NOD(node));
    case NONE_N:
        return comp_none();
    case STR_N:
        return comp_str((str_nd*)NOD(node), funcs, def_mems);
    case LIST_N:
        return comp_list((list_nd*)NOD(node), funcs, def_mems, val_id);
    case TUPLE_N:
        return comp_tuple((tuple_nd*)NOD(node), funcs, def_mems, val_id);
    case DICT_N:
        return comp_dict((dict_nd*)NOD(node), funcs, def_mems, val_id);
    case SET_N:
        return comp_set((set_nd*)NOD(node), funcs, def_mems, val_id);
    default:
        un_crash("comp_node function: invalid node type `%d`\n", TYP(node));
        break;
    }
}

cres_t comp_int(int_nd *node, funcs_t *funcs, struct __def_mems *def_mems)
{
    unsigned id = find_def_mem(*def_mems, INT_V, VAL(node->num_tok));

    char *defs = NULL;
    char *main;

    if (id)
    {
        alloc(main, char, ull_digits(id) + 4);
        sprintf(main, "cd_%u", id);
    }
    else
    {
        def_mems->dms++;
        if (def_mems->dms > def_mems->dma)
            ralloc(def_mems->mems, struct __def_mem, def_mems->dma += DEF_MEMS_S);

        id = def_mems->dms + DEF_MEMS_D;

        alloc(defs, char, ull_digits(id) + strlen(VAL(node->num_tok)) + 45);
        sprintf(defs, "val_t cd_%u = handle_set_int(\"%s\", 10, &c_ctx);\n", id, VAL(node->num_tok));

        alloc(main, char, ull_digits(id) + 4);
        sprintf(main, "cd_%u", id);

        def_mems->mems[def_mems->dms - 1] = set_def_mem(id, INT_V, VAL(node->num_tok));
    }

    ralloc(funcs->strs[0], char, strlen(funcs->strs[0]) + ull_digits(id) + 36);
    sprintf(funcs->strs[0], "%shandle_free_int((int_t*)VAL(cd_%u));\n", funcs->strs[0], id);

    fre(node);

    cres_t res;
    comp_succ(&res, NULL, NULL, defs, main, NULL, NULL, NULL_FUNCS);
    return res;
}

cres_t comp_float(float_nd *node, funcs_t *funcs, struct __def_mems *def_mems)
{
    unsigned id = find_def_mem(*def_mems, FLOAT_V, VAL(node->num_tok));

    char *defs = NULL;
    char *main;

    if (id)
    {
        alloc(main, char, ull_digits(id) + 4);
        sprintf(main, "cd_%u", id);
    }
    else
    {
        def_mems->dms++;
        if (def_mems->dms > def_mems->dma)
            ralloc(def_mems->mems, struct __def_mem, def_mems->dma += DEF_MEMS_S);

        id = def_mems->dms + DEF_MEMS_D;

        alloc(defs, char, ull_digits(id) + strlen(VAL(node->num_tok)) + 47);
        sprintf(defs, "val_t cd_%u = handle_set_float(\"%s\", 10, &c_ctx);\n", id, VAL(node->num_tok));

        alloc(main, char, ull_digits(id) + 4);
        sprintf(main, "cd_%u", id);

        def_mems->mems[def_mems->dms - 1] = set_def_mem(id, FLOAT_V, VAL(node->num_tok));
    }

    ralloc(funcs->strs[0], char, strlen(funcs->strs[0]) + ull_digits(id) + 40);
    sprintf(funcs->strs[0], "%shandle_free_float((float_t*)VAL(cd_%u));\n", funcs->strs[0], id);

    fre(node);

    cres_t res;
    comp_succ(&res, NULL, NULL, defs, main, NULL, NULL, NULL_FUNCS);
    return res;
}

cres_t comp_cmpx(cmpx_nd *node, funcs_t *funcs, struct __def_mems *def_mems)
{
    unsigned id = find_def_mem(*def_mems, CMPX_V, VAL(node->num_tok));

    char *defs = NULL;
    char *main;

    if (id)
    {
        alloc(main, char, ull_digits(id) + 4);
        sprintf(main, "cd_%u", id);
    }
    else
    {
        def_mems->dms++;
        if (def_mems->dms > def_mems->dma)
            ralloc(def_mems->mems, struct __def_mem, def_mems->dma += DEF_MEMS_S);

        id = def_mems->dms + DEF_MEMS_D;

        alloc(defs, char, ull_digits(id) + strlen(VAL(node->num_tok)) + 46);
        sprintf(defs, "val_t cd_%u = handle_set_cmpx(\"%s\", 10, &c_ctx);\n", id, VAL(node->num_tok));

        alloc(main, char, ull_digits(id) + 4);
        sprintf(main, "cd_%u", id);

        def_mems->mems[def_mems->dms - 1] = set_def_mem(id, CMPX_V, VAL(node->num_tok));
    }

    ralloc(funcs->strs[0], char, strlen(funcs->strs[0]) + ull_digits(id) + 38);
    sprintf(funcs->strs[0], "%shandle_free_cmpx((cmpx_t*)VAL(cd_%u));\n", funcs->strs[0], id);

    fre(node);

    cres_t res;
    comp_succ(&res, NULL, NULL, defs, main, NULL, NULL, NULL_FUNCS);
    return res;
}

cres_t comp_bool(bool_nd *node)
{
    char *main;
    alloc(main, char, 5);

    if (node->id)
        strcpy(main, "cd_1");
    else
        strcpy(main, "cd_2");

    fre(node);

    cres_t res;
    comp_succ(&res, NULL, NULL, NULL, main, NULL, NULL, NULL_FUNCS);
    return res;
}

cres_t comp_none(void)
{
    char *main;
    alloc(main, char, 5);

    strcpy(main, "cd_0");

    cres_t res;
    comp_succ(&res, NULL, NULL, NULL, main, NULL, NULL, NULL_FUNCS);
    return res;
}

cres_t comp_str(str_nd *node, funcs_t *funcs, struct __def_mems *def_mems)
{
    unsigned id = find_def_mem(*def_mems, STR_V, VAL(node->str_tok));

    char *defs = NULL;
    char *main;

    if (id)
    {
        alloc(main, char, ull_digits(id) + 4);
        sprintf(main, "cd_%u", id);
    }
    else
    {
        def_mems->dms++;
        if (def_mems->dms > def_mems->dma)
            ralloc(def_mems->mems, struct __def_mem, def_mems->dma += DEF_MEMS_S);

        id = def_mems->dms + DEF_MEMS_D;

        alloc(defs, char, ull_digits(id) + strlen(VAL(node->str_tok)) + 41);
        sprintf(defs, "val_t cd_%u = handle_set_str(\"%s\", &c_ctx);\n", id, VAL(node->str_tok));

        alloc(main, char, ull_digits(id) + 4);
        sprintf(main, "cd_%u", id);

        def_mems->mems[def_mems->dms - 1] = set_def_mem(id, STR_V, VAL(node->str_tok));
    }

    ralloc(funcs->strs[0], char, strlen(funcs->strs[0]) + ull_digits(id) + 36);
    sprintf(funcs->strs[0], "%shandle_free_str((str_t*)VAL(cd_%u));\n", funcs->strs[0], id);

    fre(node);

    cres_t res;
    comp_succ(&res, NULL, NULL, defs, main, NULL, NULL, NULL_FUNCS);
    return res;
}

cres_t comp_list(list_nd *node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id)
{
    cres_t res;
    HERR(res) = 0;

    char *incs;
    alloc(incs, char, 1);
    incs[0] = '\0';

    char *deps;
    alloc(deps, char, 1);
    deps[0] = '\0';

    char *defs;
    alloc(defs, char, 1);
    defs[0] = '\0';

    char *main;
    alloc(main, char, ull_digits(*val_id) + ull_digits(node->elems_n) + 17);

    sprintf(main, "val_t cvl_%u[%llu] = {", (*val_id)++, node->elems_n);

    char *end_main;
    alloc(end_main, char, 1);
    end_main[0] = '\0';

    char *funcs_head;
    alloc(funcs_head, char, 1);
    funcs_head[0] = '\0';

    if (node->elems_n)
    {
        struct __strs strs = reg_comp_res(&res, comp_node(node->elems_nd[0], funcs, def_mems, val_id));

        if (HERR(res))
        {
            fre(deps);
            fre(defs);
            fre(main);
            fre(end_main);
            fre(funcs_head);
            goto ret;
        }

        if (strs.incs)
        {
            ralloc(incs, char, strlen(incs) + strlen(strs.incs) + 1);

            strcat(incs, strs.incs);
        }

        if (strs.deps)
        {
            ralloc(deps, char, strlen(deps) + strlen(strs.deps) + 1);

            strcat(deps, strs.deps);
        }

        if (strs.defs)
        {
            ralloc(defs, char, strlen(defs) + strlen(strs.defs) + 1);

            strcat(defs, strs.defs);
        }

        if (strs.end_main)
        {
            ralloc(end_main, char, strlen(end_main) + strlen(strs.end_main) + 1);

            strcat(end_main, strs.end_main);
        }

        if (strs.funcs_head)
        {
            ralloc(funcs_head, char, strlen(funcs_head) + strlen(strs.funcs_head) + 1);

            strcat(funcs_head, strs.funcs_head);
        }

        ralloc(main, char, strlen(main) + strlen(strs.main) + 1);

        strcat(main, strs.main);

        free_strs(strs);

        unsigned long long i;
        for (i = 1; i < node->elems_n; i++)
        {
            strs = reg_comp_res(&res, comp_node(node->elems_nd[i], funcs, def_mems, val_id));
            if (HERR(res))
            {
                fre(deps);
                fre(defs);
                fre(main);
                fre(end_main);
                fre(funcs_head);
                goto ret;
            }

            if (strs.incs)
            {
                ralloc(incs, char, strlen(incs) + strlen(strs.incs) + 1);

                strcat(incs, strs.incs);
            }

            if (strs.deps)
            {
                ralloc(deps, char, strlen(deps) + strlen(strs.deps) + 1);

                strcat(deps, strs.deps);
            }

            if (strs.defs)
            {
                ralloc(defs, char, strlen(defs) + strlen(strs.defs) + 1);

                strcat(defs, strs.defs);
            }

            if (strs.end_main)
            {
                ralloc(end_main, char, strlen(end_main) + strlen(strs.end_main) + 1);

                strcat(end_main, strs.end_main);
            }

            if (strs.funcs_head)
            {
                ralloc(funcs_head, char, strlen(funcs_head) + strlen(strs.funcs_head) + 1);

                strcat(funcs_head, strs.funcs_head);
            }

            ralloc(main, char, strlen(main) + strlen(strs.main) + 3);

            sprintf(main, "%s, %s", main, strs.main);

            free_strs(strs);
        }
    }

    ralloc(main, char, strlen(main) + 2);

    strcat(main, "}");

    comp_succ(&res, incs, deps, defs, main, end_main, funcs_head, NULL_FUNCS);

    ret:
    free_nodes(node->elems_nd, node->elems_n);
    fre(node);
    return res;
}

cres_t comp_tuple(tuple_nd *node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id)
{

}

cres_t comp_dict(dict_nd *node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id)
{

}

cres_t comp_set(set_nd *node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id)
{

}

cres_t comp_bin_op(bin_op_nd *node, funcs_t *funcs, struct __def_mems *def_mems, unsigned *val_id)
{

}

int ull_digits(unsigned long long number)
{
    int digits = 0;
    do
        digits++;
    while ((number /= 10) > 0);

    return digits;
}
