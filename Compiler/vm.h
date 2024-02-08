#ifndef vm_H
#define vm_H

#include "Stack.h"
#include "table_k_quads/Symbol_Table.h"
#include "table_k_quads/quads.h"
#include <assert.h>


#define EXPAND_SIZE_I 1024
#define CURR_SIZE_I (total_i * sizeof(instruction))
#define NEW_SIZE_I (EXPAND_SIZE_I* sizeof(instruction)+CURR_SIZE_I)



typedef enum vmopcode{
    assign_v,   add_v ,   sub_v,
    mul_v,      n_div_v,  mod_v,
    jeq_v,    jnoteq_v,
    jlesseq_v,  jgreatereq_v, jless_v,
    jgreater_v, call_v,   pusharg_v,
    funcstart_v, funcend_v, newtable_v,  
    tablegetelem_v, tablesetelem_v ,
    jump_v,nop_v,    
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



//incomplete_jumps to generate
typedef struct incomplete_jump{
    unsigned instrNo;
    unsigned iaddress;
    struct incomplete_jump * next;
}incomplete_jump;

/**
 * @brief given and instruction i init its vmargs with -1
 * 
 * @param i  = instruction
 * @param op  = op
 * @param scrLine  = scrLine
 * @return instruction 
 */
instruction  instruction_constractor(instruction *i,vmopcode op, unsigned scrLine);


userfunc userfunc_constractor(userfunc* func,unsigned ad,unsigned ls,char* id);

static void avm_initstack (void);

void malloc_all_lists();

const char* getOpcodeString(vmopcode code);
void add_incomplete_jump(incomplete_jump** head, unsigned instrNo, unsigned iaddress) ;
void patch_incomplete_jumps(incomplete_jump** head,quad *quads);


int nextinstructionlabel();

void make_operand();

int nextinstructionlabel();

void generateInstructions (void);

void make_numberoperand( vmarg * arg ,double val);

void make_booloperand( vmarg * arg ,unsigned val);

void make_retvaloperand( vmarg * arg );

void reset_operand(vmarg * arg);

unsigned consts_newnumber(double n);
unsigned libfuncs_newused(char* str);
unsigned userfunc_newfunc(expr *func);
unsigned consts_newstring(char* str);

void expand_i();

const char* get_varg_t_name(vmarg_t type);
void print_i_formal(instruction *q);
void print_vmarg_formal(vmarg *e) ;
void print_all_i(instruction *instructions,unsigned total);
void print_all_lists(double* numConsts,unsigned totalNumConsts,char** stringConsts,unsigned totalStringConsts,char** namedLibfuncs,unsigned totalNamedLibfuncs,userfunc* userFuncs,unsigned totalUserFuncs);

void write_all_i(const char* filename);


unsigned  saveStringToUnsigned(const char* str);
char* translateUnsignedToString(unsigned  value);

void write_all_data_binary(FILE* file);
void writeBinaryFile(const char* filename);

void readBinary(char* filename);

#endif /*vm_H*/