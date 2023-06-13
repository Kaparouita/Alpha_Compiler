#include "avm.h"
#include "vm.h"


avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned	top, topsp;
avm_memcell stack[AVM_STACKSIZE];
int N;
unsigned char executionFinished = 0;
unsigned pc = 0;


//reverse translation for constants
extern double*     numConsts           ;
extern unsigned    totalNumConsts      ;
extern char**      stringConsts        ;       
extern unsigned    totalStringConsts   ;
extern char**       namedLibfuncs      ;
extern unsigned    totalNamedLibfuncs  ;
extern userfunc*   userFuncs           ;
extern unsigned    totalUserFuncs      ;

//STACK

int N = AVM_STACKSIZE;
avm_memcell  stack[AVM_STACKSIZE];                  //the stack
unsigned  globmem;                                  //the first global's variable oofset on mem
unsigned  callmem;
unsigned  datamem;   

avm_memcell * avm_memcell_constractor(avm_memcell_t type,data_union data){
    avm_memcell* cell = (avm_memcell*) malloc (sizeof(avm_memcell*));
    cell->data = data;
    cell->type = type;
    return cell;
}


avm_memcell* avm_translate_operand (vmarg* arg, avm_memcell* reg){
    if(!arg){return NULL;}
	switch(arg->type){
		/*Variables*/
		case global_a:	return &stack[AVM_STACKSIZE - 1 - arg->val];
		case local_a:	return &stack[topsp - arg->val];
		case formal_a:	return &stack[topsp + AVM_STACKENV_SIZE + 1 + arg->val];
		case retval_a:	return &retval;
		case number_a: {
			reg->type = number_m;
			reg->data.numVal = consts_getnumber(arg->val);
			//printf("-----%u-----",arg->val);
			return reg;
		}
		case string_a: {
			reg->type =  string_m;
			reg->data.strVal = strdup(consts_getstring(arg->val));
			return reg;
		}
		case bool_a: {
			reg->type = bool_m;
			reg->data.boolVal = arg->val;
			return reg;
		}
		case nil_a: reg->type = nil_m; return reg;
		case userfunc_a: {
			reg->type = userfunc_m;
			/*if function address is directly stored*/
			reg->data.funcVal = arg->val;
			/*if function i table is stored*/
			reg->data.funcVal = userfuncs_getaddress(arg->val);
			return reg;
		}
		case libfunc_a: {
			reg->type = libfunc_m;
			reg->data.libfuncVal = libfuncs_getused(arg->val);
			return reg;
		}
		default: assert(0);
	}	
}


//TABLE
void avm_table_increfcounter (avm_table* t){
    t->refCounter++;
}

void avm_table_decrefcounter (avm_table* t){
    if(--t->refCounter <= 0)
        avm_table_destroy(t);
}

void avm_table_buckets_init (avm_table_bucket** p){
	
    for (unsigned i=0; i < AVM_TABLE_HASHSIZE; ++i)
        p[i] = (avm_table_bucket*) 0;
}

avm_table* avm_tablenew(void ){
    avm_table* t = (avm_table*) malloc (sizeof(avm_table));
    t->refCounter = t->totalStr = t->totanNum = 0;
    avm_table_buckets_init(t->numIndexed);
    avm_table_buckets_init(t->strIndexed);

    return t;
}


void avm_tablebucketsdestroy (avm_table_bucket** p){
    for ( unsigned i = 0;i < AVM_TABLE_HASHSIZE; ++i,++p){
        for (avm_table_bucket *b = *p; b;){
            avm_table_bucket* del = b;
            b = b->next;
            avm_memcellclear (&del->key);
            avm_memcellclear (&del->value);
            free(del);
        }
        p[i] = (avm_table_bucket*) 0;
    }
}

void avm_table_destroy (avm_table* t){
    avm_tablebucketsdestroy (t->strIndexed);
    avm_tablebucketsdestroy (t->numIndexed);
    free(t);
}

/*girname to value tou apothkeumenou pinaka
pou fortosame sto read*/
double consts_getnumber(unsigned i) {
    return numConsts[i-1];
}
char*  consts_getstring(unsigned i) {
    return stringConsts[i-1];
}
char*  libfuncs_getused(unsigned i) {
    return namedLibfuncs[i];
}
unsigned userfuncs_getaddress(unsigned i) {
    return userFuncs[i].address;
}

extern void memclear_string(avm_memcell* m);
extern void memclear_table(avm_memcell* m);

typedef void (*memclear_func_t)(avm_memcell*);

/*CLEAR*/
memclear_func_t memclearFuncs[] = {
	0,	/*number*/
	memclear_string,
	0,	/*bool*/
	memclear_table,
	0,	/*userfunc*/
	0,	/*libfunc*/
	0,	/* nil*/
	0,	/*undef*/
};

void avm_memcellclear (avm_memcell* m){
	if(m->type != undef_m){
		memclear_func_t f = memclearFuncs[m->type];
		if(f){
			(*f)(m);
		}
		m->type = undef_m;
	}
}

void memclear_string (avm_memcell* m){
	assert(m->data.strVal);
	free(m->data.strVal);
}

void memclear_table (avm_memcell* m){
	assert(m->data.tableVal);
	avm_table_decrefcounter(m->data.tableVal);
} 

