#include "vm.h"

double*     numConsts;
unsigned    totalNumConsts;
char**      stringConsts;
unsigned    totalStringConsts;
char*       namedLibfuncs;
unsigned    totalNamedLibfuncs;
userfunc*   userFuncs;
unsigned    totalUserFuncs;


//STACK

avm_memcell  stack[AVM_STACKSIZE];

static void avm_initstack (void){
    for (unsigned i = 0; i<AVM_STACKSIZE;i++){
        AVM_WIPEOUT(stack[i]);
        stack[i].type = undef_m;
    }
}

//TABLE
void avm_table_increfcounter (avm_table* t){
    t->refCounter++;
}

/*garbage colector*/
void avm_table_decrefcounter (avm_table* t){
    assert(t->refCounter > 0);
    if(!--t->refCounter)
        avm_table_destroy(t);
}

void avm_table_buckets_init (avm_table_bucket** p){
    for (unsigned i=0; i < AVM_TABLE_HASHSIZE; ++i)
        p[i] = (avm_table_bucket*) 0;
}

avm_table* avm_tablenew(void ){
    avm_table* t = (avm_table*) malloc (sizeof(avm_table*));
    AVM_WIPEOUT(*t);

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

void avm_tabledestroy (avm_table* t){
    avm_tablebucketsdestroy (t->strIndexed);
    avm_tablebucketsdestroy (t->numIndexed);
    free(t);
}
