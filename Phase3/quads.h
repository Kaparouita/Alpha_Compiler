#include "Symbol_Table.h"
#include <assert.h>

unsigned total=0;
unsigned int currQuad=0;
unsigned programVarOffset=0;
unsigned functionLocalOffset=0;
unsigned formalArgOffset=0;
unsigned scopeSpaceCounter=1;

#define EXPAND_SIZE 1024
#define CURR_SIZE (total*sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE*sizeof(quad)+CURR_SIZE)

typedef enum iopcode{
    assign, add , sub,
    mul,    div,    mod,
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
    symbol_t * type;
    char *  name; //dynamic string
    scopespace_t space; //originating scope space
    unsigned   offset;  //offset in scope space
    unsigned   scope;   //scope value
    unsigned   line;    //source line of declaration
}symbol;


typedef struct expr{
    expr_t  type;
    symbol *   sym;
    expr *  index;  
    double  numConst;
    char *  strConst;
    unsigned char   boolConst;
    expr * next;
}expr;

typedef struct quad{
    iopcode     op;
    expr * result;
    expr * arg1;
    expr * arg2;
    unsigned  label;
    unsigned  line;
}quad;

quad* quads=(quad*)0;  //nitializes the pointer to the memory address 0,


void expand(){
    assert(total==currQuad);
    quad* p=(quad*)malloc(NEW_SIZE);
    if(quads==0){
        memcpy(p,quads,CURR_SIZE);
        free(quads);
    }
    quads=p;
    total+=EXPAND_SIZE;
}

void emit(iopcode op, expr* arg1, expr* arg2, expr* result, unsigned label, unsigned line){
    if(currQuad==total) expand();

    quad* p=quads+currQuad++;
    p->arg1=arg1;
    p->arg2=arg2;
    p->result=result;
    p->label=label;
    p->line=line;
}

scopespace_t currscopespace(){
    if(scopeSpaceCounter==1)
        return programvar;
    else
        if(scopeSpaceCounter%2==0)
            return formalarg;
        else 
            return functionlocal;
}

unsigned currscopeoffset(){
    switch (currscopespace)
    {
    case /* constant-expression */:
        /* code */
        break;
    
    default: assert(0);
    }
}