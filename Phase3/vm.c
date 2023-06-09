#include "vm.h"
#include <assert.h>

double*     numConsts           = (double*)0;
unsigned    totalNumConsts      = 0;
char**      stringConsts        = (char**) 0;       
unsigned    totalStringConsts   = 0;
char**       namedLibfuncs       = (char **)0;
unsigned    totalNamedLibfuncs  = 0;
userfunc*   userFuncs           = (userfunc*)0;
unsigned    totalUserFuncs      = 0;

//THE LISTS
//gia to genarate
void malloc_all_lists(){
    stringConsts = (char**)malloc(sizeof(char**));
    numConsts = (double*)malloc(sizeof(double));
    namedLibfuncs = (char**)malloc(sizeof(char**));
    userFuncs = (userfunc*)malloc(sizeof(userfunc*));

    //init lib
    for(int i = 0; i<11;i++)
        libfuncs_newused(get_lib_fuctions()[i]);

}

unsigned consts_newstring(char* str){
    //check an yparxei idi
    for(int i=0; i<totalStringConsts; ++i) {
        if(!strcmp(stringConsts[i], str))
            return i;
    }
    stringConsts= realloc(stringConsts, sizeof(char*)*(totalStringConsts+1));
    stringConsts[totalStringConsts++] = strdup(str);
    return totalStringConsts;
}
unsigned consts_newnumber(double n){
    for(int i=0; i<totalNumConsts; ++i) 
        if(numConsts[i] == n)
            return i;
    numConsts = realloc(numConsts, sizeof(double)*(totalNumConsts+1));
    numConsts[totalNumConsts++] = n;
    return totalNumConsts;
}

unsigned libfuncs_newused(char* str){
    for(int i=0; i<totalNamedLibfuncs; ++i) 
        if(!strcmp(namedLibfuncs[i],str))
            return i;
    namedLibfuncs = realloc(namedLibfuncs, sizeof(char*)*(totalNamedLibfuncs+1));
    namedLibfuncs[totalNamedLibfuncs++] = strdup(str);
    return totalNamedLibfuncs;
}

unsigned userfunc_newfunc(expr *e){
    for(int i = 0 ; i < totalUserFuncs;i++)
        if(!strcmp(userFuncs[i].id,e->sym->name))
            return i;
    userFuncs = realloc(userFuncs, sizeof(userfunc)*(totalUserFuncs+1));
    userfunc_constractor(&userFuncs[totalUserFuncs],e->sym->fuctionAddress,e->sym->totalLocals,e->sym->name);
    return totalUserFuncs++;
}

extern var_table* table ;

//STACK

avm_memcell  stack[AVM_STACKSIZE];                  //the stack
unsigned  globmem;                                  //the first global's variable oofset on mem
unsigned  callmem;
unsigned  datamem;                   

//contractors

vmarg  vmarg_constractor(vmarg* v,vmarg_t type,unsigned val){
    v->type = type;
    v->val = val;
}

avm_memcell * avm_memcell_constractor(avm_memcell_t type,data_union data){
    avm_memcell* cell = (avm_memcell*) malloc (sizeof(avm_memcell*));
    cell->data = data;
    cell->type = type;
    return cell;
}

userfunc userfunc_constractor(userfunc* func,unsigned ad,unsigned ls,char* id){
    func->address = ad;
    func->id = strdup(id);
    func->localSize = ls;
}

instruction  instruction_constractor(instruction *i,vmopcode op, unsigned scrLine){ 
     vmarg_constractor(&i->arg1,-1,-1);
     vmarg_constractor(&i->arg2,-1,-1);
     vmarg_constractor(&i->result,-1,-1);
     i->opcode = op;
     i->scrLine = scrLine;
}


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
            /*
            avm_memcellclear (&del->key);
            avm_memcellclear (&del->value);
            */
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



unsigned int saveStringToUnsigned(const char* str) {
    return strtoul(str, NULL, 0);
}

char* translateUnsignedToString(unsigned int value) {
    char* str = malloc(sizeof(char*));  // Adjust the size as per your requirements
    sprintf(str, "%u", value);
    return str;
}
//generate staf

void make_operand(expr* e,vmarg* arg){
        if(!e)  { 
            arg->type = nil_a; 
            arg->val = -1; 
            return;
        }
        switch(e->type){
            case var_e:
            case tableitem_e:
            case arithexpr_e:
            case assignexpr_e:
            case boolexpr_e:
            case newtable_e:
                arg->val = e->sym->offset;
                switch (e->sym->space){
                    case programvar:        arg->type = global_a ; break;
                    case functionlocal :    arg->type = local_a; break;
                    case formalarg :        arg->type = formal_a; break; 
                    default : assert (0);
                }
                break;
            
            case constbool_e: {
                arg->val = (unsigned)e->boolConst;
                arg->type = bool_a ; 
                break;
            }
            case conststring_e : {
                arg->val = consts_newstring(e->strConst);
                arg->type = string_a; break; 
            }
            case constnum_e : {
                arg->val = consts_newnumber(e->numConst);
                arg->type = number_a; break ;
            } 
            case nil_e : {arg->type = nil_a;arg->val = -1; break;}
            case programfunc_e: {
                arg->type = userfunc_a;
                arg->val = e->sym->fuctionAddress;
                arg->val = userfunc_newfunc(e);
                break;
            }
            case libraryfunc_e : {
                arg->type = libfunc_a; 
                arg->val = libfuncs_newused(e->sym->name);
            break;
            }
            default : assert(0);
        }
}

extern instruction* instructions ; 
extern int total_i;
extern int curr_i;

void expand_i(){
    instruction* i=(instruction*)malloc(NEW_SIZE_I);
    if(instructions){
        memcpy(i,instructions,CURR_SIZE_I);
        free(instructions);
    }
    instructions = i;
    total_i +=EXPAND_SIZE_I;
}

int nextinstructionlabel(){return curr_i;}

void reset_operand(vmarg * arg){
    vmarg_constractor(arg,-1,-1);
}

//try

/*gia function thelw 
       sto start einai to old_topsp
    1. old_top(102) -> meta_args
    2. ->acutal_args tis function
    3. ret_adress (to ret adress estw 11)
    4. top(102) //save to top
    5. top_sp   //save to top_sp
    6. local var tis function
*/


void init_globals(){
    var *v = get_scope_var(0);
    data_union u;
    while(v){
        v = v->next;
    }
}

