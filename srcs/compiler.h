#ifndef __COMPILER__
#define __COMPILER__

#include "core/node.h"
#include "errlib/errlib.h"

typedef struct
{
    unsigned long long size;
    char **strs;
} funcs_t;

typedef struct
{
    char *incs;
    char *deps;
    char *defs;
    char *main;
    char *end_main;
    char *funcs_head;
    funcs_t funcs;

    int haserr : 1;
    runtime_err_t error;
} cres_t;

cres_t compile(node_t node);

funcs_t set_funcs(unsigned long long size, char **strs);
void disp_funcs(funcs_t funcs);
void free_funcs(funcs_t funcs);

#endif /* __COMPILER__ */
