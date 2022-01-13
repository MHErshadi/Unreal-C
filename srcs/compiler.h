#ifndef __COMPILER__
#define __COMPILER__

#include "core/node.h"
#include "errlib/errlib.h"

typedef struct
{
    size_t size;
    char **str;
} funs_t;

typedef struct
{
    char *incs;
    char *deps;
    char *defs;
    char *main;
    funs_t funs;

    int haserr : 1;
    runtime_err_t error;
} cres_t;

cres_t compile(node_t node);

funs_t set_funs(size_t size, char **str);
void disp_funs(funs_t funs);
void free_funs(funs_t funs);

#endif /* __COMPILER__ */
