#ifndef avm_H
#define avm_H

#include "vm.h"
#include <stdio.h>
#define AVM_STACKSIZE       4096
#define AVM_WIPEOUT(m)      memset (&(m),  0,  sizeof(m))

#define AVM_TABLE_HASHSIZE 211


#define AVM_NUMACTUALS_OFFSET  4
#define AVM_SAVEDPC_OFFSET  3
#define AVM_SAVEDTOP_OFFSET 2
#define AVM_SAVEDTOPSP_OFFSET  1
#define AVM_STACKENV_SIZE	4


#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_div execute_arithmetic
#define execute_mod execute_arithmetic
#define execute_jeq execute_bool
#define execute_jle execute_bool
#define execute_jne execute_bool
#define execute_jge execute_bool
#define execute_jlt execute_bool
#define execute_jgt execute_bool

typedef struct  avm_table avm_table;
typedef struct avm_table_bucket  avm_table_bucket;
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

#define AVM_ENDING_PC codeSize
extern char* typeStrings[];
extern avm_memcell cx,stack[],ax,bx,retval;
extern unsigned char executionFinished ;
extern unsigned pc,top,topsp;
extern int N;
/**
unsigned currLine = 0 ;*/
extern unsigned codeSize;
extern instruction * code ;
extern unsigned currLine ;
extern unsigned totalActuals;
extern int global_vars;


avm_memcell * avm_memcell_constractor(avm_memcell_t type,data_union data);
void avm_callsaveenvironment(void);
void execute_funcexit(instruction* unused);
//TABLES

avm_table* avm_table_new(void);
void       avm_table_destroy(avm_table* t);

avm_memcell* avm_getactual(unsigned i);
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
void execute_bool (instruction* instr);
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

double consts_getnumber (unsigned i);
char* consts_getstring (unsigned i);
char* libfuncs_getused (unsigned i);
unsigned userfuncs_getaddress(unsigned i);


void avm_initialize();
avm_memcell* avm_translate_operand (vmarg* arg, avm_memcell* reg);
void avm_calllibfunc(char* id);


void avm_tablesetelem(avm_table* table,avm_memcell* index,avm_memcell* content);
avm_memcell* avm_tablegetelem (avm_table* table,avm_memcell* index);
void avm_assign(avm_memcell* lv, avm_memcell* rv);
char* avm_tostring (avm_memcell* m);
unsigned avm_get_envvalue(unsigned i);
static void avm_initstack(void);

void execute_cycle(void);

void execute_nop(instruction *);

#endif /*avm_H*/