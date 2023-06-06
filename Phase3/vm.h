#ifndef vm_H
#define vm_H

#include "Stack.h"
#include "table_k_quads/Symbol_Table.h"
#include "table_k_quads/quads.h"
#include <assert.h>

#define AVM_STACKSIZE       4096
#define AVM_WIPEOUT(m)      memset (&(m),  0,  sizeof(m))

#define AVM_TABLE_HASHSIZE 211

typedef struct  avm_table avm_table;
typedef struct avm_table_bucket  avm_table_bucket;

typedef enum vmopcode{
    assign_v,   add_v ,   sub_v,
    mul_v,      n_div_v,  mod_v,
    uminus_v,   and_v,    or_v,
    not_v,      jeq_v,    jnoteq_v,
    jlesseq_v,  jgreatereq_v, jless_v,
    jgreater_v, call_v,   pusharg_v,
    funcstart_v, funcend_v, newtable_v,  
    tablegetelem_v, tablesetelem_v ,nop_v,    
}vmopcode;

typedef enum vmarg_t{       //TO VAL STO VMARG EINAI

    label_a  = 0,           //jump instr
    global_a = 1,           //thesh sto stack
    formal_a = 2,           // ^
    local_a  = 3,           // ^
    number_a = 4,           // ola ta ypoloipa einai -> value
    string_a = 5,
    bool_a   = 6,
    nil_a    = 7,
    userfunc_a  = 8,
    libfunc_a   = 9,
    retval_a    = 10,
}vmarg_t;

typedef struct vmarg {
    vmarg_t type;
    unsigned val;
}vmarg;

typedef struct instruction {
    vmopcode    opcode;
    vmarg       result;
    vmarg       arg1;
    vmarg       arg2;
    unsigned    scrLine;
}instruction;

typedef struct userfunc {
    unsigned    address;
    unsigned    localSize;
    char*       id;
}userfunc;


//TO STACK THS MHXANHS
typedef enum avm_memcell_t{
    number_m   = 0,
    string_m   = 1,
    bool_m     = 2,
    table_m    = 3,
    userfunc_m = 4,
    libfunc_m  = 5,
    nil_m      = 6,
    undef_m    = 7,
}avm_memcell_t;

typedef union {
        double          numVal;
        char*           strVal;
        unsigned char   boolVal;
        avm_table*      tableVal;
        unsigned        funcVal;
        char*           libfuncVal;
} data_union;


typedef struct avm_memcell{
    avm_memcell_t type;
    data_union data;
}avm_memcell;


 struct avm_table_bucket{
    avm_memcell         key;
    avm_memcell         value;
    avm_table_bucket*   next;
};

 struct  avm_table{
    unsigned            refCounter;
    avm_table_bucket *  strIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket *  numIndexed[AVM_TABLE_HASHSIZE];
    unsigned            totalStr;
    unsigned            totanNum;
};


static void avm_initstack (void);

//TABLES

avm_table* avm_table_new(void);
void       avm_table_destroy(avm_table* t);
avm_table* avm_table_getelem(avm_memcell* key);
void       avm_table_setelem(avm_memcell* key,avm_memcell value);


/**
 * @brief increase the refcounter by 1
 * 
 * @param t 
 */
void avm_table_increfcounter (avm_table* t);

/**
 * @brief dec the refcounter and destroy the last t
 * 
 * @param t 
 */
void avm_table_decrefcounter (avm_table* t);

/**
 * @brief init the bucket of a table (str,int)
 * 
 * @param p 
 */
void avm_table_buckets_init (avm_table_bucket** p);
/**
 * @brief create a new avm table
 * 
 * @return avm_table* 
 */

avm_table* avm_tablenew(void );

/**
 * @brief clear all dynamic data or reset its reference to a table
 * 
 * @param m 
 */
void avm_memcellclear (avm_memcell* m);

/**
 * @brief destroy buckets of a table
 * 
 * @param p 
 */
void avm_tablebucketsdestroy (avm_table_bucket** p);

void avm_table_destroy (avm_table* t);


#endif /*vm_H*/