#ifndef quads_H
#define quads_H
#include "Symbol_Table.h"

#define EXPAND_SIZE 1024
#define CURR_SIZE (total*sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE*sizeof(quad)+CURR_SIZE)



typedef enum iopcode{
    assign, add , sub,
    mul,    n_div,    mod,
    uminus, and,    or,
    not,    if_eq,  if_noteq,
    if_lesseq,  if_geatereq, if_less,
    if_greater, call,   param,
    ret,    getretval,  funcstart,funcend,
    funcdef, tablecreate,   
    tablegetelem, tablesetelem ,jump     
}iopcode;

typedef enum expr_t{
    var_e,
    tableitem_e,

    programfunc_e,
    libraryfunc_e,

    arithexpr_e,
    boolexpr_e,
    assignexpr_e,
    newtable_e,

    constnum_e,
    constbool_e,
    conststring_e,

    nil_e

}expr_t;


typedef struct expr{
    expr_t          type;
    struct var*        sym; // check this 
    struct expr *   index;  
    double          numConst;
    char *          strConst;
    unsigned        boolConst;
    struct expr *   next;
}expr;

typedef struct quad{
    iopcode     op;
    expr *      result;
    expr *      arg1;
    expr *      arg2;
    unsigned    label;
    unsigned    line;
}quad;

typedef struct indexed{
    expr *indexedelem;
    expr *value;
    struct indexed *next;
}indexed;


typedef struct call_s
{
    expr* elist;
    unsigned char method;
    char* name;
}call_s;


/**
 * epektini ton pinaka twn quads
*/
void expand(void);

/**
 * @brief constractor gia quad 
 * to vazei sto teleuteo quads
 * @param iopcode op
 * @param expr* arg1
 * @param expr* arg2
 * @param expr* result
 * @param unsigned int label
 * @param unsigned int line
 * @return void
*/
void emit(iopcode op, expr* arg1, expr* arg2, expr* result, unsigned int label, unsigned int  line);


call_s *call_constractor(expr* e,unsigned char method,char* name);
/**
 * @brief ["x" : kati_x , "b" : kati_b]
 * 
 * @param indexedelem  "x"
 * @param value "kati_x"
 * @param next "b"
 * @return indexed* 
 */
indexed *indexed_constractor(expr *indexedelem,expr *value,indexed *next);
/**
 * @brief 
 * @return to type ths metavlhths analoga to scope ths
 * @param none
*/
enum scopespace_t currscopespace(void);

/**
 * @brief to offset sto current scope space
 * @param void
 * @return unsigned int 
*/
unsigned int  currscopeoffset(void);

/**
 * increment to offset in to current scope space
 * depend the type of var  , insert in to ST
 * @param void
 * @return void
*/
void inccurrscopeoffset(void);

/**
 * increment to the counter scopeSpaceCounter
 * @param void
 * @return void
*/
void enterscopespace(void);

/**
 * decrement to the counter scopeSpaceCounter 
 * @param void
 * @return void
*/
void exitscopespace(void);

/**
 * assign 0 to the offset of forml args
 * @param void
 * @return void
*/
void resetformalargsoffset(void);

/**
 * assign 0 to the offset of function locals
 * @param void
 * @return void
*/
void resetfunctionlocaloffset(void);

/**
 * reset the initial scope space
 * @param void
 * @return void
*/
void restorecurrscopeoffset(unsigned int  n);

/**
 * return the label from the next quad
 * @param void
 * @return unsigned int
*/
unsigned int nextquadlabel(void);

/**
 * function for backpatching
 * @param unsigned int quadNo
 * @param unsigned int label
 * @return void
*/
void patchlabel(unsigned int quadNo, unsigned int label);

/**
 * create a lvalue expr and return it
 * @param var* sym
 * @return expr*
*/
expr* lvalue_expr(struct var* sym);

/**
 * create a table member
 * @param expr* lv
 * @param char* name
 * @return expr*
*/
expr* member_item (expr* lv, char* name);

/**
 * create a new expr with type expr_t
 * @param expr_t  t
 * @return expr*
*/
expr* newexpr(expr_t t);

/**
 * create a new expr str/num/bool/double/nil
 * @param char* s
 * @return expr*
*/
expr* newexpr_conststring(char* s);
expr* newexpr_constnum(double n);
expr* newexpr_constbool(unsigned  c);
expr* newexpr_constdouble(double n);
expr* newexpr_nil();

/**
 * den to xw kanei use akomh
 * @return char*
*/
char* newtempvars(void);


/**
 * kanei emit an einai table item
 * @param expr* e
 * @return expr*
*/
expr* emit_iftableitem(expr* e);

/**
 * kanei call to fuc
 * @param expr* lv
 * @param expr* reversed_elist
 * @return expr*
*/
expr* make_call(expr* lv,expr* reversed_elist);

/*printing ola*/
void print_expr(expr* e);
void print_quad(struct quad* q);
const char* get_op_name(iopcode opcode);
const char* get_expr_t_name(expr_t type);
void print_all_quads();
/**
 * @brief check if v is a fuction
 * error if not
 */
void check_if_fuction(expr* e);
/**
 * @brief check if its a number expr
 * 
 * @param e 
 */
void check_arith(expr* e);
/**
 * @brief do numeric operation to e1 and e2
 * 
 * @param expr1 
 * @param expr2 
 * @param op 
 * @return expr* 
 */
expr* do_maths(expr* expr1,expr* expr2,iopcode op);
/**
 * @brief do bool operation to e1 and e2
 * 
 * @param expr1 
 * @param expr2 
 * @param op 
 * @return expr* 
 */
expr* do_bool(expr* expr1,expr* expr2,iopcode op);

/**
 * @brief Get the elist length object
 * 
 * @param e to exp
 * @return size
 */
int get_elist_length(expr *e);
/**
 * @brief Get the indexed length object
 * 
 * @param index 
 * @return size
 */
int get_indexed_length(indexed *index);
/**
 * @brief create and emit in a new table
 * 
 * @return expr* 
 */
expr *tablecreate_and_emit();

/**
 * @brief create temp name for vars
 * 
 * @return char* 
 */
unsigned int istempname (char* s); 
/**
 * @brief function to reused variables
 * 
 * @return expr* 
 */
unsigned int istempexpr (expr* e);
/**
 * @brief function for arithm operators
 * 
 * @return expr* 
 */
expr* arithop(expr* expr1,expr* expr2,iopcode op);
/**
 * @brief function for relop operators
 * 
 * @return expr* 
 */
expr* relop(expr* expr1,expr* expr2,iopcode op);
/**
 * @brief function for emit the relops
 * 
 * @return void 
 */
void emit_relop(expr * e,iopcode op);
/**
 * @brief function for emit the relops
 * @param expr *e1
 * @param expr* e2
 * @param iopcode op
 * @return expr* 
 */
expr *check_if_same_type(expr *e1,expr* e2,iopcode op);
/**
 * @brief function for emit the relops
 * @param expr *e1
 * @param expr* e2
 * @param iopcode op
 * @return expr* 
 */
expr* boolo(expr* expr1,expr* expr2,iopcode op);
/**
 * @brief function to check is boolean 
 * @param expr *e
 * @return 0 for true and 1 for false
 */
int check_if_bool(expr * expr);
expr *is_temp_else_create(expr *e1,expr *e2,expr_t type);

void copy_value(expr *e1,expr *e2);
#endif /*quads_H*/