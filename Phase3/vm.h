#ifndef vm_H
#define vm_H

#include "Stack.h"


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

typedef enum vmarg_t{
    label_a  = 0,
    global_a = 1,
    formal_a = 2,
    local_a  = 3,
    number_a = 4,
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


#endif /*vm_H*/