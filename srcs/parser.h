#ifndef __PARSER__
#define __PARSER__

#include "core/node.h"
#include "core/token.h"
#include "errlib/errlib.h"

typedef struct
{
    node_t node;

    int haserr : 1;
    inv_syn_err_t error;
} pres_t;

pres_t parse(tok_t *toks);

#endif /* __PARSER__ */
