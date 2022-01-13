#include <string.h>

#include "compiler.h"
#include "term/operation.h"
#include "macro.h"

#define DEF_MEMS_S 32
#define DEF_MEMS_D 2

#define NULL_FUNS ((funs_t){0, NULL})

struct __strs
{
    char *incs;
    char *deps;
    char *defs;
    char *main;
    funs_t funs;
};

struct __def_mem
{
    unsigned id;

    enum val_t type;
    char *val;
};

struct __def_mems
{
    size_t dma;
    size_t dms;

    struct __def_mem *mems;
};

struct __strs conv_cres_to_strs(cres_t other);

struct __def_mem set_def_mem(unsigned id, enum val_t type, char *val);
void free_def_mem(struct __def_mem def_mem);
int cmp_def_mem(struct __def_mem def_mem, enum val_t type, const char *val);
unsigned find_def_mem(struct __def_mems def_mems, enum val_t type, const char *val);

struct __def_mems set_def_mems(size_t dma, size_t dms, struct __def_mem *mems);
void free_def_mems(struct __def_mems def_mems);

void comp_succ(cres_t *res, char *incs, char *deps, char *defs, char *main, funs_t funs);
void comp_fail(cres_t *res, runtime_err_t error);
struct __strs reg_comp_res(cres_t *res, cres_t other);

funs_t concat_funs(funs_t funs1, funs_t funs2);

char *comp_incs(void);
char *comp_deps(void);
char *comp_defs(void);
char *comp_main(void);
funs_t comp_funs(void);

cres_t comp_int(int_nd *node, struct __def_mems *def_mems);
cres_t comp_float(float_nd *node, struct __def_mems *def_mems);
cres_t comp_cmpx(cmpx_nd *node, struct __def_mems *def_mems);
cres_t comp_bool(bool_nd *node);
cres_t comp_none(void);
cres_t comp_str(str_nd *node, struct __def_mems *def_mems);
cres_t comp_list(list_nd *node);
cres_t comp_tuple(tuple_nd *node);
cres_t comp_dict(dict_t *node);
cres_t comp_set(set_t *node);
cres_t comp_bin_op(bin_op_nd *node);

int ull_digits(unsigned long long number);

cres_t compile(node_t node)
{
    size_t dma = DEF_MEMS_S;
    struct __def_mem *mems;
    alloc(mems, struct __def_mem, dma);

    struct __def_mems def_mems = set_def_mems(dma, 0, mems);

    char *incs = comp_incs();
    char *deps = comp_deps();
    char *defs = comp_defs();
    char *main = comp_main();
    funs_t funs = comp_funs();

    printf("%llu\n", funs.size);

    cres_t comp_res;
    switch (TYP(node))
    {
    case INT_N:
        comp_res = comp_int((int_nd*)NOD(node), &def_mems);
        break;
    case FLOAT_N:
        comp_res = comp_float((float_nd*)NOD(node), &def_mems);
        break;
    case CMPX_N:
        comp_res = comp_cmpx((cmpx_nd*)NOD(node), &def_mems);
        break;
    case BOOL_N:
        comp_res = comp_bool((bool_nd*)NOD(node));
        break;
    case NONE_N:
        comp_res = comp_none();
        break;
    case STR_N:
        comp_res = comp_str((str_nd*)NOD(node), &def_mems);
        break;
    case LIST_N:
        comp_res = comp_list((list_nd*)NOD(node));
        break;
    case TUPLE_N:
        comp_res = comp_tuple((tuple_nd*)NOD(node));
        break;
    default:
        un_crash("compile function: invalid node type `%d`\n", TYP(node));
        break;
    }

    printf("%llu\n", funs.size);

    free_def_mems(def_mems);

    if (HERR(comp_res))
    {
        fre(incs);
        fre(deps);
        fre(defs);
        fre(main);
        free_funs(funs);

        cres_t res;
        comp_fail(&res, ERR(comp_res));
        return res;
    }

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
    if (comp_res.funs.size)
        concat_funs(funs, comp_res.funs);

    ralloc(main, char, strlen(main) + 15);
    strcat(main, ";\nreturn 0;\n}\n");

    ralloc(funs.str[0], char, strlen(funs.str[0]) + 3);
    strcat(funs.str[0], "}\n");

    cres_t res;
    comp_succ(&res, incs, deps, defs, main, funs);
    return res;
}


funs_t set_funs(size_t size, char **str)
{
    funs_t funs;
    funs.size = size;
    funs.str = str;

    return funs;
}

void disp_funs(funs_t funs)
{
    size_t i;
    for (i = 0; i < funs.size; i++)
        printf("%s\n", funs.str[i]);
}

void free_funs(funs_t funs)
{
    size_t i;
    for (i = 0; i < funs.size; i++)
        fre(funs.str[i]);

    fre(funs.str);
}

struct __strs conv_cres_to_strs(cres_t other)
{
    struct __strs strs;
    strs.incs = other.incs;
    strs.deps = other.deps;
    strs.defs = other.defs;
    strs.main = other.main;
    strs.funs = other.funs;

    return strs;
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
    size_t i;
    for (i = 0; i < def_mems.dms; i++)
        if (cmp_def_mem(def_mems.mems[i], type, val))
            return def_mems.mems[i].id;

    return 0;
}

struct __def_mems set_def_mems(size_t dma, size_t dms, struct __def_mem *mems)
{
    struct __def_mems def_mems;
    def_mems.dma = dma;
    def_mems.dms = dms;
    def_mems.mems = mems;

    return def_mems;
}

void free_def_mems(struct __def_mems def_mems)
{
    size_t i;
    for (i = 0; i < def_mems.dms; i++)
        free_def_mem(def_mems.mems[i]);

    fre(def_mems.mems);
}

void comp_succ(cres_t *res, char *incs, char *deps, char *defs, char *main, funs_t funs)
{
    HERR(*res) = 0;
    res->incs = incs;
    res->deps = deps;
    res->defs = defs;
    res->main = main;
    res->funs = funs;
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

funs_t concat_funs(funs_t funs1, funs_t funs2)
{
    size_t res_size = funs1.size + funs2.size;
    printf("%llu + %llu = %llu\n", funs1.size, funs2.size, res_size);

    char **res_str;
    alloc(res_str, char*, res_size);

    size_t i, j;
    for (i = 0; i < funs1.size; i++)
        res_str[i] = funs1.str[i];
    for (j = 0; j < funs2.size; j++)
        res_str[i + j] = funs2.str[j];

    funs_t res = set_funs(res_size, res_str);
    return res;
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

    sprintf(deps, "struct __ctx c_ctx = handle_main_ctx();\n");

    return deps;
}

char *comp_defs(void)
{
    char *defs;
    alloc(defs, char, 121);

    sprintf(defs, "val_t cd_0 = handle_set_none(&c_ctx);\n");
    strcat(defs, "val_t cd_1 = handle_set_bool(1, &c_ctx);\n");
    strcat(defs, "val_t cd_2 = handle_set_bool(0, &c_ctx);\n");

    return defs;
}

char *comp_main(void)
{
    char *main;
    alloc(main, char, 35);

    sprintf(main, "int main(int argc, char **argv) {\n");

    return main;
}

funs_t comp_funs(void)
{
    char **str;
    alloc(str, char*, 1);

    alloc(str[0], char, 92);

    sprintf(str[0], "void cf_gc() {\nhandle_free_bool((bool_t*)VAL(cd_1));\nhandle_free_bool((bool_t*)VAL(cd_2));\n");

    funs_t funs = set_funs(1, str);
    return funs;
}

cres_t comp_int(int_nd *node, struct __def_mems *def_mems)
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

    char **str;
    alloc(str, char*, 1);

    alloc(str[0], char, ull_digits(id) + 36);

    sprintf(str[0], "handle_free_int((int_t*)VAL(cd_%u));\n", id);

    funs_t funs = set_funs(1, str);
    printf("%llu\n", funs.size);

    fre(node);

    cres_t res;
    comp_succ(&res, NULL, NULL, defs, main, funs);
    return res;
}

cres_t comp_float(float_nd *node, struct __def_mems *def_mems)
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

    fre(node);

    cres_t res;
    comp_succ(&res, NULL, NULL, defs, main, NULL_FUNS);
    return res;
}

cres_t comp_cmpx(cmpx_nd *node, struct __def_mems *def_mems)
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

    fre(node);

    cres_t res;
    comp_succ(&res, NULL, NULL, defs, main, NULL_FUNS);
    return res;
}

cres_t comp_bool(bool_nd *node)
{
    char *main;
    alloc(main, char, 5);

    if (node->id)
        sprintf(main, "cd_1");
    else
        sprintf(main, "cd_2");

    fre(node);

    cres_t res;
    comp_succ(&res, NULL, NULL, NULL, main, NULL_FUNS);
    return res;
}

cres_t comp_none(void)
{
    char *main;
    alloc(main, char, 5);

    sprintf(main, "cd_0");

    cres_t res;
    comp_succ(&res, NULL, NULL, NULL, main, NULL_FUNS);
    return res;
}

cres_t comp_str(str_nd *node, struct __def_mems *def_mems)
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

    fre(node);

    cres_t res;
    comp_succ(&res, NULL, NULL, defs, main, NULL_FUNS);
    return res;
}

cres_t comp_list(list_nd *node)
{
    cres_t res;
    HERR(res) = 0;

    
}

cres_t comp_tuple(tuple_nd *node)
{

}

cres_t comp_dict(dict_t *node)
{

}

cres_t comp_set(set_t *node)
{

}

cres_t comp_bin_op(bin_op_nd *node)
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
