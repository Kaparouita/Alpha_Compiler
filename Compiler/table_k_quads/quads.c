#include <assert.h>
#include "quads.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

extern int yyerror (char* yaccProvidedMessage);
extern int yylineno;

unsigned total=0;
unsigned int currQuad=0;
unsigned programVarOffset=0;
unsigned functionLocalOffset=0;
unsigned formalArgOffset=0;
unsigned scopeSpaceCounter=1;
quad* quads=(quad*)0;  //nitializes the pointer to the memory address 0,
extern int tempcounter;
extern int global_vars ;

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
    quad* p= quads + currQuad++;
    p->op  = op;
    p->arg1=arg1;
    p->arg2=arg2;
    p->result=result;
    p->label=label;
    p->line=line;
}

call_s *call_constractor(expr* e,unsigned char method,char* name){
    call_s *c = (call_s*)malloc(sizeof(call_s));
    c->elist = e;
    c->method = method;
    c->name = name;
    return c;
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
    scopeSpaceCounter--;
    resetformalargsoffset();
    resetfunctionlocaloffset();
}

void resetformalargsoffset(){ formalArgOffset=0;}

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
    assert(quadNo<currQuad );
    if(quads[quadNo].label ==0)
        quads[quadNo].label = label; //edw mporei na thelei alagh 
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

expr* newexpr_constnum(double n){
    expr* e = newexpr(constnum_e);
    e->numConst = n;
    return e;
} 

expr* newexpr_constdouble(double n){
    expr* e = newexpr(constnum_e);
    e->numConst= n;
    return e;
} 

expr* newexpr_constbool(unsigned c){
    expr* e = newexpr(constbool_e);
    e->boolConst= c;
    return e;    
}
 
expr* emit_iftableitem(expr* e){
    if(e->type != tableitem_e) 
        return e; // if not table

    expr* result = newexpr(var_e);
    result->sym = newtemp(); 
    emit(tablegetelem,e->index,e,result,0,yylineno);  //!paizei na einai lathos
    return result;
}

stmt_t *stmt_constractor(int break_list,int cont_list){
    stmt_t *s = malloc(sizeof(stmt_t));
	s->breaklist = break_list;
	s->contlist = cont_list; 
	return s;
}

for_prefix *for_prefix_constractor(int test,int enter){
    for_prefix *f = malloc(sizeof(for_prefix));
    f->enter =  enter;
    f->test  =  test;
    return f;
}

void make_stmt (stmt_t* s)  
    { s->breaklist = s->contlist = 0; }

int mergelist (int l1, int l2) {
    if (!l1)
        return l2;
    else if (!l2)
        return l1;
    else {
        int i = l1;
        while (quads[i].label)
            i = quads[i].label;
        quads[i].label = l2;
        return l1;
    }
}

void patchlist (int list, int label) {
    while (list) {
        int next = quads[list].label;
        quads[list].label = label;
        list = next;
    }
}


int newlist (int i)     { quads[i].label = 0; return i; }

expr* make_call(expr* lv,expr* reversed_elist){
    expr* func = emit_iftableitem(lv);
    while (reversed_elist){
        emit(param,reversed_elist,NULL,NULL,0,yylineno);
        reversed_elist = reversed_elist->next;
    }
    emit(call,NULL,NULL,func,0,yylineno);
    expr* result = newexpr(var_e);
    result->sym = newtemp();
    emit(getretval,NULL,NULL,result,0,yylineno);
    return result;
}



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
        if(e->boolConst == 0)
            printf("Bool : False");
        else 
            printf("Bool : True");
    }
}

void print_quad_formal(struct quad *q) {
    printf("%-14s", get_op_name(q->op));
    print_expr_formal(q->arg1);
    print_expr_formal(q->arg2);
    print_expr_formal(q->result);
    if(q->label)
        printf("%-14d", q->label);
    else
        printf("%-14s", "0");
    printf("[%d]\n",q->line);
}

void print_expr_formal(expr *e) {
    if (e == NULL) {
        printf("%-14s", "NULL");
        return;
    }
    if(e->sym != NULL){
        printf("%-14s", e->sym->name);
       //// if(e->type >= 0)
        //    printf(",%s",get_expr_t_name( e->type));
    }
    else{
        switch(e->type){
            case(constbool_e) :
                if(e->boolConst == 0)
                    printf("%-14s", "False");
                else 
                    printf("%-14s", "True");
                break;
            case(conststring_e) :
                printf("s: %-11s", e->strConst);
                break;
            case(constnum_e) :
                printf("%-14.2f", e->numConst);
                break;
        }
    }
}

const char* get_op_name(iopcode opcode) {
    switch(opcode) {
        case jump:
            return "jump";
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
        case funcend:
            return "funend";
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

int check_arith (expr* e) {
    if ( e->type == constbool_e ||
        e->type == conststring_e ||
        e->type == nil_e ||
        e->type == newtable_e ||
        e->type == programfunc_e ||
        e->type == libraryfunc_e ||
        e->type == boolexpr_e )
            {printf("Illegal expr used !\n");return 0;}
        return 1;
}

void copy_value(expr *e1,expr *e2){
    assert(e1);
    assert(e2);
    e1->type = e2->type;
    switch (e2->type){
        case constnum_e : 
            e1->numConst = e2->numConst;
            break;
        case constbool_e :
            e1->boolConst = e2->boolConst;
            break;
        case conststring_e :
            e1->strConst = e2->strConst;
            break;
    }
}

expr* arithop(expr* expr1,expr* expr2,iopcode op){
    expr* r = newexpr(arithexpr_e);
    int i;
    i = (!check_arith(expr1)) ? check_arith(expr1) : check_arith(expr2);
    if((expr1->type == constnum_e ) && (expr2->type == constnum_e )){
        switch (op){
            case add:           
                r = newexpr_constnum( expr1->numConst + expr2->numConst);
                break;
            case sub:          
                r = newexpr_constnum( expr1->numConst - expr2->numConst);
                break;
            case mul: 
                r = newexpr_constnum( expr1->numConst * expr2->numConst);
                break;
            case n_div:
                r = newexpr_constnum( expr1->numConst / expr2->numConst);
                break;
            case mod:     
                r = newexpr_constnum( (int)expr1->numConst % (int)expr2->numConst);
                break;
            default:            yyerror("wrong operation");
        }
    }else
    {
    r = is_temp_else_create(expr1,expr2,arithexpr_e);
    emit(op,expr1,expr2,r,0,yylineno);
        }
    return r;
}   

expr* relop(expr* expr1,expr* expr2,iopcode op){
    check_arith(expr1);
    check_arith(expr2);
    expr* r = newexpr(boolexpr_e);
     if((expr1->type == constnum_e ) && (expr2->type == constnum_e )){
        switch (op){
            case if_lesseq:             
                r = newexpr_constbool( expr1->numConst >= expr2->numConst);
                break;
            case if_less:               
                r = newexpr_constbool( expr1->numConst > expr2->numConst);
                break;
            case if_greater:            
                r = newexpr_constbool(expr1->numConst < expr2->numConst);
                break;
            case if_geatereq:           
                r = newexpr_constbool(expr1->numConst <= expr2->numConst);
                break;
            default:            yyerror("wrong operation");
        }
        r->sym = newtemp();
    }
    else if(check_if_same_type(expr1,expr2,op)->type != nil_e){
        switch(op){
        case if_eq:               
            r = check_if_same_type(expr1,expr2,op);  
                break;                      
        case if_noteq:
            r = check_if_same_type(expr1,expr2,op); 
                break;
        }
    }
    else
        r = is_temp_else_create(expr1,expr2,boolexpr_e);
    
    emit(op,expr1,expr2,r,currQuad+2,yylineno);
    emit(jump,NULL,NULL,NULL,currQuad+3,yylineno);
    return r;
}

expr *check_if_same_type(expr *e1,expr* e2,iopcode op){
    expr* r = newexpr(boolexpr_e);
    if(e1->type == e2->type ){
        switch (e1->type ){  
            case constnum_e:
                return (op == if_noteq) ? newexpr_constbool( e1->numConst != e2->numConst) : 
                                        newexpr_constbool( e1->numConst == e2->numConst);
                break;
            case constbool_e:
                return (op == if_noteq) ? newexpr_constbool(e1->boolConst != e2->boolConst):
                                        newexpr_constbool(e1->boolConst == e2->boolConst);
                break;
            case  conststring_e:
                return (op == if_noteq) ? newexpr_constbool(e1->strConst != e2->strConst):
                                        newexpr_constbool(e1->strConst == e2->strConst);
                break;
            default:
                r = newexpr_nil();
                break;
        }   
    }else {  
            if( (e1->type == tableitem_e || e1->type == nil_e ) && ( e2->type == tableitem_e || e2->type == nil_e) ) 
                return (op == if_noteq) ? r: r;
            else{
                printf("The exprs isnt the same type\n");
                r = newexpr_nil();
            }
    }
    return r;
}

void emit_relop(expr * e,iopcode op){
    if(e->type != nil_e){
        emit(assign,newexpr_constbool(1),NULL,e,0,yylineno);
        emit(jump,NULL,NULL,NULL,currQuad+2,yylineno);
        emit(assign,newexpr_constbool(0),NULL,e,0,yylineno);
    }
}

unsigned boolo(expr* expr1,expr* expr2,iopcode op){
    int bool1=check_if_bool(expr1);
    int bool2=check_if_bool(expr2);
    switch (op){
    case and:   return (bool1 && bool2);
    case or:    return (bool1 || bool2);
    case not:   
    default:    yyerror("wrong operation");
    }
    return 0;
}

int check_if_bool(expr * expr){
    if(expr->type == libraryfunc_e ||expr->type == tableitem_e || expr->type == programfunc_e )
        return 0;
    else if (expr->type == nil_e)
        return 1;
    else if (expr->type == constnum_e)
        (expr->numConst == 0) ? 0:1;
    else if (expr->type == conststring_e)
        (strlen(expr->strConst ) == 0) ? 0:1;
    else return 1;
}

expr *is_temp_else_create(expr *e1,expr *e2,expr_t type){
    expr *r = newexpr(type);
	if(istempexpr(e1))
		r->sym = e1->sym;
	else if(istempexpr(e2))
		r->sym = e2->sym;
	else
		r->sym = newtemp();
    return r;
}

void print_all_quads(){
    int i = 0;
    printf("\n\n/---------------------------------   PRINTING ALL QUADS    ----------------------------------/\n\n");
    printf("%-8s%-14s%-14s%-14s%-14s%-14s%-14s\n\n","NO","OP","ARG1","ARG2","RESULT","LABEL","LINE");
    while(i < currQuad){
        printf("%-8d",i);
        print_quad_formal(&quads[i]);
        i++;
    }
}

int getglobalvars(){
    return global_vars;
}

expr *tablecreate_and_emit(){
    expr* e = newexpr(newtable_e);
	e->sym = newtemp();
	emit(tablecreate, NULL, NULL,e, 0, yylineno);
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

unsigned int istempname (char* s) {
    return *s == '_';
}

unsigned int istempexpr (expr* e) {
    return e->sym && istempname(e->sym->name);
}

