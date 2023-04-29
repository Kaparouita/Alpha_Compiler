#include <assert.h>
#include "quads.h"

unsigned total=0;
unsigned int currQuad=0;
unsigned programVarOffset=0;
unsigned functionLocalOffset=0;
unsigned formalArgOffset=0;
unsigned scopeSpaceCounter=1;
int tempcounter=0;

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

void emit(iopcode op, expr* arg1, expr* arg2, expr* result, unsigned int label, unsigned int line){
    if(currQuad==total) 
        expand();

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

unsigned int currscopeoffset(){
    switch (currscopespace()){
        case programvar : return programVarOffset;
        case functionlocal : return functionLocalOffset;
        case formalarg : return formalArgOffset;
        default: assert(0);
    }
}

void inccurrscopeoffset(){
    switch (currscopespace()){
        case programvar :  ++programVarOffset; break;
        case functionlocal :++functionLocalOffset; break;
        case formalarg : ++formalArgOffset; break;
        default: assert(0);
    }
}

void enterscopespace(){ ++scopeSpaceCounter;}

void exitscopespace(){ 
    assert(scopeSpaceCounter>1);
    --scopeSpaceCounter;
}

void resetformalargsofset(){ formalArgOffset=0;}

void resetfunctionlocaloffset(){ functionLocalOffset=0;}

void restorecurrscopeoffset(unsigned int  n){
    switch (currscopespace()){
        case programvar :  programVarOffset=n; break;
        case functionlocal :functionLocalOffset=n; break;
        case formalarg : formalArgOffset=n; break;
        default: assert(0);
    }
}

unsigned int nextquadlabel(){ return currQuad;}

void patchlabel(unsigned int quadNo, unsigned int  label){
    assert(quadNo<currQuad);
    quads[quadNo].label=label; //edw mporei na thelei alagh 
}

expr* lvalue_expr(symbol* sym){
    assert(sym);
    expr* e=(expr*)malloc(sizeof(expr));
    memset(e,0,sizeof(expr));

    e->next=(expr*) 0;
    e->sym=sym;

    switch (sym->type){
        case var_s :  e->type=var_e; break;
        case programfunc_s :e->type=programfunc_e; break;
        case libraryfunc_s : e->type=libraryfunc_e; break;
        default: assert(0);
    }
    return e;
}

expr* member_item (expr* lv, char* name) {
    lv = emit_iftableitem(lv); // Emit code if r-value use of table item
    expr* ti = newexpr(tableitem_e); // Make a new expression
    ti->sym = lv->sym;
    ti->index = newexpr_conststring(name); // Const string index
    return ti;
}


expr* newexpr(expr_t t){
    expr* e=(expr*)malloc(sizeof(expr));
    memset(e,0,sizeof(expr));
    e->type=t;
    return e;
}

expr* newexpr_conststring(char* s){
    expr* e=newexpr(conststring_e);
    e->strConst=strdup(s);
    return e;
} 

char* newtempvars() {
    char buffer[32];             // allocate a fixed-size buffer on the stack
    sprintf(buffer, "_t%d", tempcounter++);
    return strdup(buffer);      // use strdup to allocate a new string on the heap
}

//!!! SLIDE 45 LEC 9
symrec_t* newtemp() {
    char *name = newtempvars();
    symrec_t *sym = LOOKUP(scope,name); //!!!!PREPEI NA TO FTIAKSUME AFTO 
    if (sym == NULL) 
        sym = insert(scope, 0, SYM_LOCAL_VAR, name);  //!!!!PREPEI NA TO FTIAKSUME AFTO
  
    return sym;
}

expr* emit_iftableitem(expr* e){
    if(e->type!=tableitem_e) return e;
    else{
        expr* result=newexpr(var_e);
        result->sym=newtemp(); 
        emit(tablegetelem,e,e->index,result);
        return result;
    }
}

expr* make_call(expr* lv,expr* reversed_elist){
    expr*func=emit_iftableitem(lv);
    while ((reversed_elist)){
        emit(param,reversed_elist,NULL,NULL);
        reversed_elist=reversed_elist->next;
    }
    emit(call,func,NULL,NULL);
    expr* result=newexpr(var_e);
    result->sym=newtemp();
    emit(getretval,NULL,NULL,result);
    return result;
}

