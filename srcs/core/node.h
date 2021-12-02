#ifndef __NODE__
#define __NODE__

#include "token.h"

#define LIST_S 16
#define TUPLE_S 16
#define DICT_S 16
#define SET_S 16
#define IF_CASE_S 8
#define SWITCH_CASE_S 12

enum node_t
{
    INT_N = 1,
    FLOAT_N,
    CMPX_N,
    BOOL_N,
    NONE_N,
    STR_N,
    LIST_N,
    TUPLE_N,
    DICT_N,
    SET_N,
    BIN_OP_N,
    UNARY_OP_N,
    IDX_N,
    VAR_ASSIGN_N,
    VAR_ACCESS_N,
    IF_N,
    SWITCH_N,
    FOR_N,
    FOREACH_N,
    WHILE_N,
    DOWHILE_N,
    LOOP_N
};

typedef struct
{
    enum node_t type;
    void *node;

    pos_t poss;
    pos_t pose;
} node_t;

typedef struct
{
    tok_t num_tok;
} int_nd;

typedef struct
{
    tok_t num_tok;
} float_nd;

typedef struct
{
    tok_t num_tok;
} cmpx_nd;

typedef struct
{
    tok_t bool_tok;
} bool_nd;

typedef struct
{
    tok_t str_tok;
} str_nd;

typedef struct
{
    size_t elems_n;
    node_t *elems_nd;
} list_nd;

typedef struct
{
    size_t elems_n;
    node_t *elems_nd;
} tuple_nd;

struct __kv
{
    node_t key_nd;
    node_t val_nd;
};

typedef struct
{
    size_t kvs_n;
    struct __kv *kvs_nd;
} dict_nd;

typedef struct
{
    size_t elems_n;
    node_t *elems_nd;
} set_nd;

typedef struct
{
    node_t op1_nd;
    tok_t op_tok;
    node_t op2_nd;
} bin_op_nd;

typedef struct
{
    tok_t op_tok;
    node_t op_nd;
} unary_op_nd;

typedef struct
{
    node_t op_nd;
    node_t idxs_nd;
} idx_nd;

typedef struct
{
    int *props;

    tok_t name_tok;
    tok_t assign_tok;
    node_t val_nd;
} var_assign_nd;

typedef struct
{
    tok_t name_tok;
} var_access_nd;

struct __case
{
    node_t cond_nd;
    node_t body_nd;
};

typedef struct
{
    size_t case_n;
    struct __case *cases;

    node_t ebody_nd;
} if_nd;

typedef struct
{
    node_t check_nd;

    size_t case_n;
    struct __case *cases;

    node_t dbody_nd;
} switch_nd;

typedef struct
{
    tok_t name_tok;

    node_t start_nd;
    node_t end_nd;
    node_t step_nd;

    node_t body_nd;
} for_nd;

typedef struct
{
    tok_t name_tok;
    node_t iter_nd;

    node_t body_nd;
} foreach_nd;

typedef struct
{
    node_t cond_nd;
    node_t body_nd;
} while_nd;

typedef struct
{
    node_t body_nd;
    node_t cond_nd;
} dowhile_nd;

typedef struct
{
    node_t init_nd;
    node_t cond_nd;
    node_t step_nd;

    node_t body_nd;
} loop_nd;

node_t set_node(enum node_t type, void *nd, pos_t poss, pos_t pose);
void disp_node(node_t node);
void free_node(node_t node);
void free_nodes(node_t *nodes, size_t nodes_s);

int_nd *set_int_nd(tok_t num_tok);
float_nd *set_float_nd(tok_t num_tok);
cmpx_nd *set_cmpx_nd(tok_t num_tok);
bool_nd *set_bool_nd(tok_t bool_tok);
str_nd *set_str_nd(tok_t str_tok);
list_nd *set_list_nd(size_t elems_n, node_t *elems_nd);
tuple_nd *set_tuple_nd(size_t elems_n, node_t *elems_nd);
dict_nd *set_dict_nd(size_t kvs_n, struct __kv *kvs_nd);
set_nd *set_set_nd(size_t elems_n, node_t *elems_nd);
bin_op_nd *set_bin_op_nd(node_t op1_nd, tok_t op_tok, node_t op2_nd);
unary_op_nd *set_unary_op_nd(tok_t op_tok, node_t op_nd);
idx_nd *set_idx_nd(node_t op_nd, node_t idxs_nd);
var_assign_nd *set_var_assign_nd(int *props, tok_t name_tok, tok_t assign_tok, node_t val_nd);
var_access_nd *set_var_access_nd(tok_t name_tok);
if_nd *set_if_nd(size_t case_n, struct __case *cases, node_t ebody_nd);
switch_nd *set_switch_nd(node_t check_nd, size_t case_n, struct __case *cases, node_t dbody_nd);
for_nd *set_for_nd(tok_t name_tok, node_t start_nd, node_t end_nd, node_t step_nd, node_t body_nd);
foreach_nd *set_foreach_nd(tok_t name_tok, node_t iter_nd, node_t body_nd);
while_nd *set_while_nd(node_t cond_nd, node_t body_nd);
dowhile_nd *set_dowhile_nd(node_t body_nd, node_t cond_nd);
loop_nd *set_loop_nd(node_t init_nd, node_t cond_nd, node_t step_nd, node_t body_nd);

struct __kv set_kv(node_t key, node_t val);
void free_kvs(struct __kv *kvs, size_t kv_n);
void free_kv(struct __kv kv);

struct __case set_case(node_t cond, node_t body);
void free_cases(struct __case *cases, size_t case_n);
void free_case(struct __case case_);

int node_type(node_t node, enum node_t type);

#endif /* __NODE__ */
