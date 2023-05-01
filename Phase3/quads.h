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
    ret,    getretval,  funcstart,
    funcdef, tablecreate,   
    tablegetelem, tablesetelem        
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

typedef enum scopespace_t{
    programvar,
    functionlocal,
    formalarg
}scopespace_t;

typedef enum symbol_t{var_s,programfunc_s,libraryfunc_s} symbol_t;

typedef struct symbol{
    symbol_t        type;
    char *          name;       //dynamic string
    scopespace_t    space;      //originating scope space
    unsigned        offset;     //offset in scope space
    unsigned        scope;      //scope value
    unsigned        line;       //source line of declaration
}symbol;


typedef struct expr{
    expr_t          type;
    symbol *        sym; // check this 
    struct expr *   index;  
    double          numConst;
    char *          strConst;
    unsigned char   boolConst;
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

/**
 * @brief Create a symbol object
 * 
 * @param type to type tou sumbol(var,fuct,lib)
 * @param name to name
 * @param space an einai (programvar,functionlocal,formalarg)
 * @param offset poios einai o arithmos sto scope tou
 * @param scope to scope
 * @param line  to line
 * @return symbol* 
 */
symbol* create_symbol(symbol_t type, char* name, scopespace_t space, unsigned offset, unsigned scope, unsigned line);
/**
 * @brief 
 * @return to type ths metavlhths analoga to scope ths
 * @param none
*/
scopespace_t currscopespace(void);

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
void resetformalargsofset(void);

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
 * retern the label from the next quad
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
 * @param symbol* sym
 * @return expr*
*/
expr* lvalue_expr(symbol* sym);

/**
 * create a table member
 * @param expr* lv
 * @param char* name
 * @return expr*
*/
expr* member_item (expr* lv, char* name);

//-------------------------------------------

/**
 * create a lvalue expr and return it
 * @param expr_t  t
 * @return expr*
*/
expr* newexpr(expr_t t);

/**
 * create a lvalue expr and return it
 * @param char* s
 * @return expr*
*/
expr* newexpr_conststring(char* s);

expr* newexpr_constnum(int n);
expr* newexpr_constbool(char c);

/**
 * create a lvalue expr and return it
 * @return char*
*/
char* newtempvars(void);


symrec_t* newtemp(void);

/**
 * create a lvalue expr and return it
 * @param expr* e
 * @return expr*
*/
expr* emit_iftableitem(expr* e);

/**
 * create a lvalue expr and return it
 * @param expr* lv
 * @param expr* reversed_elist
 * @return expr*
*/
expr* make_call(expr* lv,expr* reversed_elist);


void print_expr(expr* e);
void print_quad(struct quad* q);
const char* get_op_name(iopcode opcode);
const char* get_expr_t_name(expr_t type);
/**
 * @brief check if v is a fuction
 * error if not
 */
void check_if_fuction(expr* e);

symbol_t sym_var_type(int type);

expr* do_maths(expr* expr1,expr* expr2,iopcode op);
expr* do_bool(expr* expr1,expr* expr2,iopcode op);