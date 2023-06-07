#include "vm.h"

double*     numConsts           = 0;
unsigned    totalNumConsts      = 0;
char**      stringConsts;       
unsigned    totalStringConsts;
char*       namedLibfuncs;
unsigned    totalNamedLibfuncs;
userfunc*   userFuncs;
unsigned    totalUserFuncs;


extern var_table* table ;

//STACK

avm_memcell  stack[AVM_STACKSIZE];                  //the stack
unsigned  globmem;                                  //the first global's variable oofset on mem
unsigned  callmem;
unsigned  datamem;                   

//contractors

vmarg * vmarg_constractor(vmarg_t type,unsigned val){
    
}

avm_memcell * avm_memcell_constractor(avm_memcell_t type,data_union data){
    avm_memcell* cell = (avm_memcell*) malloc (sizeof(avm_memcell*));
    cell->data = data;
    cell->type = type;
    return cell;
}


instruction * instruction_constractor(vmopcode op,vmarg r,  vmarg arg1 , vmarg arg2, unsigned scrLine){
     instruction* i = (instruction*) malloc (sizeof(instruction*));
     i->result = r;
     i->arg1 = arg1;
     i->arg2 = arg2;
     i->scrLine = scrLine;
     i->opcode = op;
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


//gia to genarate
unsigned consts_newstring(char*);
unsigned consts_newnumber(double n);
unsigned libfuncs_newused(char*);

void make_operand(expr* e,vmarg* arg){
       switch(e->type){
            case var_e:
            case tableitem_e:
            case arithexpr_e:
            case boolexpr_e:
            case newtable_e:{
                arg->val    =   e->sym->offset;
                switch (e->sym->space){
                    case programvar: arg->type = global_a ; break;
                    case functionlocal : arg->type = local_a; break;
                    case formalarg : arg->type = formal_a; break; 
                    default : assert (0);
                }
            }
            case constbool_e: {
                arg->val = e->boolConst;
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
            case nil_e : arg->type = nil_a; break;
            case programfunc_e: {
                arg->type = userfunc_a;
                arg->val = e->sym->fuctionAddress;
                break;
            }
            case libraryfunc_e : {
                arg->type = libfunc_a; 
                arg->val = libfuncs_newused(e->sym->name);
            break;
            }
            default : assert (0);
        }
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
