#include <assert.h>
#include "quads.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

extern int yyerror (char* yaccProvidedMessage);

unsigned total=0;
unsigned int currQuad=0;
unsigned programVarOffset=0;
unsigned functionLocalOffset=0;
unsigned formalArgOffset=0;
unsigned scopeSpaceCounter=1;
quad* quads=(quad*)0;  //nitializes the pointer to the memory address 0,
extern int tempcounter;

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
    p->op  = op;
    p->arg1=arg1;
    p->arg2=arg2;
    p->result=result;
    p->label=label;
    p->line=line;
    print_quad(p);
}

indexed *indexed_constractor(expr *indexedelem,expr *value,indexed *next){
    indexed *i = (indexed*)malloc(sizeof(indexed));
    i->indexedelem = indexedelem;
    i->value = value;
    i->next = next;
    return i;
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
    scopeSpaceCounter -=2;
    resetformalargsofset();
    resetfunctionlocaloffset();
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


expr* lvalue_expr(var* sym){
    assert(sym);
    expr* e=(expr*)malloc(sizeof(expr));
    memset(e,0,sizeof(expr));

    e->next=(expr*) 0;
    e->sym=sym;

    switch (sym->id){
        case global :  e->type=var_e; break;
        case formal :  e->type=var_e; break;
        case local :  e->type=var_e; break;
        case user_func :e->type=programfunc_e; break;
        case lib_func : e->type=libraryfunc_e; break;
        default: assert(0);
    }
    return e;
}

expr* member_item (expr* lv, char* name) {
    lv = emit_iftableitem(lv);              // Emit code if r-value use of table item
    expr* ti = newexpr(tableitem_e);        // Make a new expression
    ti->sym = lv->sym;
    ti->index = newexpr_conststring(name);  // Const string index
    return ti;
}


expr* newexpr(expr_t t){
    expr* e = (expr*)malloc(sizeof(expr));
    memset(e,0,sizeof(expr));
    e->type=t;
    return e;
}

expr* newexpr_conststring(char* s){
    expr* e = newexpr(conststring_e);
    e->strConst = strdup(s);
    return e;
} 

expr* newexpr_nil(){
    expr* e = newexpr(nil_e);
    return e;
}

expr* newexpr_constnum(int n){
    expr* e = newexpr(constnum_e);
    e->numConst = n;
    return e;
} 

expr* newexpr_constdouble(double n){
    expr* e = newexpr(constnum_e);
    e->numConst= n;
    return e;
} 

expr* newexpr_constbool(char c){
    expr* e = newexpr(constbool_e);
    e->boolConst= c;
    return e;    
}
 


expr* emit_iftableitem(expr* e){
    if(e->type != tableitem_e) return e; // if not table
    else {
        expr* result = newexpr(var_e);
        result->sym = newtemp(); 
        emit(tablegetelem,result,e,e->index,0,0);  //!paizei na einai lathos
        return result;
    }
}
/*
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
*/



/*PAIZW ME PRINTS IGNORE*/
void print_quad(struct quad *q) {
    printf("(op: %s, arg1: ", get_op_name(q->op));
    print_expr(q->arg1);
     printf("),{ arg2: ");
    print_expr(q->arg2);
    printf("},[ result: ");
    print_expr(q->result);
    printf("],( label: %d, line: %d)\n", q->label, q->line);
}


void print_expr(expr* e) {
    if (e == NULL) {
        printf("NULL");
        return;
    }
    printf("Type: %s ,",get_expr_t_name(e->type));
    if(e->sym != NULL)
        printf("Sym : %s ,", e->sym->name);
    printf("Index : ");
    if (e->index == NULL) {
        printf("None ,");
    } else {
        print_expr(e->index);
    }
    if (e->type == constnum_e) {
        printf("Num : %f ", e->numConst);
    }
    if (e->type == conststring_e) {
        printf("Str : %s ", e->strConst);
    }
    if (e->type == constbool_e){ 
        if(e->boolConst == '0')
            printf("Bool : False");
        else 
            printf("Bool : True");
    }
}

const char* get_op_name(iopcode opcode) {
    switch(opcode) {
        case assign:
            return "assign";
        case add:
            return "add";
        case sub:
            return "sub";
        case mul:
            return "mul";
        case n_div:
            return "n_div";
        case mod:
            return "mod";
        case uminus:
            return "uminus";
        case and:
            return "and";
        case or:
            return "or";
        case not:
            return "not";
        case if_eq:
            return "if_eq";
        case if_noteq:
            return "if_noteq";
        case if_lesseq:
            return "if_lesseq";
        case if_geatereq:
            return "if_geatereq";
        case if_less:
            return "if_less";
        case if_greater:
            return "if_greater";
        case call:
            return "call";
        case param:
            return "param";
        case ret:
            return "ret";
        case getretval:
            return "getretval";
        case funcstart:
            return "funcstart";
        case funcdef:
            return "funcdef";
        case tablecreate:
            return "tablecreate";
        case tablegetelem:
            return "tablegetelem";
        case tablesetelem:
            return "tablesetelem";
        default:
            return "unknown";
    }
}

const char* get_expr_t_name(expr_t type) {
    switch(type) {
        case var_e: return "var_e";
        case tableitem_e: return "tableitem_e";
        case programfunc_e: return "programfunc_e";
        case libraryfunc_e: return "libraryfunc_e";
        case arithexpr_e: return "arithexpr_e";
        case boolexpr_e: return "boolexpr_e";
        case assignexpr_e: return "assignexpr_e";
        case newtable_e: return "newtable_e";
        case constnum_e: return "constnum_e";
        case constbool_e: return "constbool_e";
        case conststring_e: return "conststring_e";
        case nil_e: return "nil_e";
        default: return "unknown";
    }
}

void check_if_fuction(expr* e){
    if(e == NULL)
        return;
    if(e->type == tableitem_e || e->type == programfunc_e || e->type == libraryfunc_e || e->type == nil_e)
        yyerror("Illegal operation with fuction");
    return;
}

void check_arith (expr* e) {
    if ( e->type == constbool_e ||
        e->type == conststring_e ||
        e->type == nil_e ||
        e->type == newtable_e ||
        e->type == programfunc_e ||
        e->type == libraryfunc_e ||
        e->type == boolexpr_e )
            yyerror("Illegal expr used in %s!");
}

expr* do_maths(expr* expr1,expr* expr2,iopcode op){
    return NULL; // kapws prepei na anashrw ta expr apo ton parser prin auto
    check_arith(expr1);
    check_arith(expr2);
    expr* r = newexpr(arithexpr_e);
    switch (op)
    {
    case add:           return newexpr_constnum( expr1->numConst + expr2->numConst);
    case sub:           return newexpr_constnum( expr1->numConst - expr2->numConst);
    case mul:           return newexpr_constnum( expr1->numConst * expr2->numConst);
    case n_div:         return newexpr_constnum( expr1->numConst / expr2->numConst);
    case if_lesseq:     return newexpr_constnum( expr1->numConst <= expr2->numConst);
    case if_geatereq:   return newexpr_constnum( expr1->numConst >= expr2->numConst);
    case if_greater:    return newexpr_constnum( expr1->numConst > expr2->numConst);
    case if_less:       return newexpr_constnum( expr1->numConst < expr2->numConst);
    default:            yyerror("wrong operation");
    }
}

expr* do_bool(expr* expr1,expr* expr2,iopcode op){
    check_if_fuction(expr1);
    check_if_fuction(expr2);
    switch (op){
    case if_eq:
        return newexpr_constbool(expr1->numConst == expr2->numConst);
    case if_noteq:
        return newexpr_constbool(expr1->numConst != expr2->numConst);
    case and:
        return newexpr_constbool(expr1->numConst && expr2->numConst);
    case or:
        return newexpr_constbool(expr1->numConst || expr2->numConst);
    default:
        yyerror("wrong operation");
    }
}

void print_all_quads(){
    int i = 0;
    printf("------------ PRINTING ALL QUADS -----------\n");
    while(i <= currQuad){
        printf("%d :",i);
        print_quad(quads++);
        i++;
    }
}

expr *tablecreate_and_emit(){
    expr* e = newexpr(newtable_e);
	e->sym = newtemp();
	emit(tablecreate, e, NULL, NULL, 0, 0);
    return e;
}

int get_elist_length(expr *e){
    int i = 0;
    while(e = e->next) i++;
    return i;
}

int get_indexed_length(indexed *index){
    int i = 0;
    while(index = index->next) i++;
    return i;
}
