#include "vm.h"


const char* getOpcodeString(vmopcode code) {
    switch (code) {
        case assign_v:       return "assign_v";
        case add_v:          return "add_v";
        case sub_v:          return "sub_v";
        case mul_v:          return "mul_v";
        case n_div_v:        return "n_div_v";
        case mod_v:          return "mod_v";
        case jump_v:          return "jump_v";
        case jeq_v:          return "jeq_v";
        case jnoteq_v:       return "jnoteq_v";
        case jlesseq_v:      return "jlesseq_v";
        case jgreatereq_v:   return "jgreatereq_v";
        case jless_v:        return "jless_v";
        case jgreater_v:     return "jgreater_v";
        case call_v:         return "call_v";
        case pusharg_v:      return "pusharg_v";
        case funcstart_v:    return "funcstart_v";
        case funcend_v:      return "funcend_v";
        case newtable_v:     return "newtable_v";
        case tablegetelem_v: return "tablegetelem_v";
        case tablesetelem_v: return "tablesetelem_v";
        case nop_v:          return "nop_v";
        default:             return "Unknown opcode";
    }
}

const char* get_varg_t_name(vmarg_t type) {
    switch(type) {
        case 0 : return "label_a";
        case 1: return "global_a";
        case 2: return "formal_a";
        case 3: return "local_a";
        case 4: return "number_a";
        case 5: return "string_a";
        case 6: return "bool_a";
        case 7: return "nil_a";
        case 8: return "userfunc_a";
        case 9: return "libfunc_a";
        case 10: return "retval_a";
        default: return "unknown";
    }
}

void print_vmarg_formal(vmarg *e) {
    if(!e)
       { printf("%-14s", "NULL");
        return;}
    switch(e->type){
            case(bool_a) :
                if(e->val == 0)
                    printf("%-14s/", "False");
                else 
                    printf("%-14s/", "True");
                break;
            case(string_a) :
                printf("s: %-14d/", e->val);
                break;
            case(number_a) :
                printf("%-14.2f/",(double) e->val);
                break;
            default : printf("%-14s",get_varg_t_name(e->type));
        }
}
void print_i_formal(instruction *q) {
    printf("%-14s", getOpcodeString(q->opcode));
    print_vmarg_formal(&q->arg1);
    print_vmarg_formal(&q->arg2);
    print_vmarg_formal(&q->result);
    printf("[%d]\n",q->scrLine);
}

void print_all_lists(double*     numConsts,
unsigned    totalNumConsts,
char**      stringConsts,       
unsigned    totalStringConsts,
char**       namedLibfuncs,
unsigned    totalNamedLibfuncs,
userfunc*   userFuncs,
unsigned    totalUserFuncs){
    // Print numConsts
    printf("Num Consts:\n");
    for (unsigned i = 0; i < totalNumConsts; i++) {
        printf("%.2f\n", numConsts[i]);
    }

    // Print stringConsts
    printf("\nString Consts:\n");
    for (unsigned i = 0; i < totalStringConsts; i++) {
        printf("%s\n", stringConsts[i]);
    }

    // Print namedLibfuncs
    printf("\nNamed Libfuncs:\n");
    for (unsigned i = 0; i < totalNamedLibfuncs; i++) {
        printf( "%s\n", namedLibfuncs[i]);
    }

    // Print userFuncs
    printf( "\nUser Funcs:\n");
    for (unsigned i = 0; i < totalUserFuncs; i++) {
        printf("Address: %d, Local Vars: %u, ID: %s\n", userFuncs[i].address, userFuncs[i].localSize,userFuncs[i].id);
    }

}

void print_all_i(instruction *instructions,unsigned total){
    int i = 0;
    printf("\n\n/---------------------------------   PRINTING ALL INSTRUCTIONS    ----------------------------------/\n\n");
    printf("%-8s%-14s%-14s%-14s%-14s%-14s\n\n","NO","OP","ARG1","ARG2","RESULT","LINE");
    while(i < total){
        printf("%-8d",i);
        print_i_formal(&instructions[i]);
        i++;
    }
}