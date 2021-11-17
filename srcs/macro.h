#ifndef __MACRO__
#define __MACRO__

#include <stdlib.h>

#define alloc(x, type, size) ((x) = (type*)malloc((size) * sizeof(type)))
#define salloc(x, type, size) ((x) = (type*)calloc((size), sizeof(type)))
#define ralloc(x, type, size) ((x) = (type*)realloc(x, (size) * sizeof(type)))
#define fre(x) (free(x))

#define TYP(x) ((x).type)

#define NOD(x) ((x).node)
#define VAL(x) ((x).val)
#define KEY(x) ((x).key)

#define CTX(x) ((x).ctx)
#define POSS(x) ((x).poss)
#define POSE(x) ((x).pose)

#define HERR(x) ((x).haserr)
#define ERR(x) ((x).error)

#endif /* __MACRO__ */
