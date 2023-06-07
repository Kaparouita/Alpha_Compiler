#include "vm.h"

extern quad* quads;

incomplete_jump * ij_head = (incomplete_jump * ) 0 ;
unsigned ij_total = 0;

void make_numberoperand( vmarg * arg ,double val){
    arg->val = consts_newnumber(val);
    arg->type = number_a;
}

void make_booloperand( vmarg * arg ,unsigned val){
    arg->val = val;
    arg->type = bool_a;
}


void make_retvaloperand( vmarg * arg ){
    arg->type = retval_a;
}

void add_incomplete_jump( unsigned instrNo , unsigned iadress);

extern void generate_ADD (quad * );
extern void generate_SUB (quad * );
extern void generate_MUL (quad * );
extern void generate_DIV (quad * );
extern void generate_MOD (quad * );
extern void generate_NEWTABLE (quad * );
extern void generate_TABLEGETELEM (quad * );
extern void generate_TABLESETELEM (quad * );
extern void generate_ASSIGN (quad * );
extern void generate_NOP (quad * );
extern void generate_JUMP (quad * );
extern void generate_IF_EQ (quad * );
extern void generate_IF_NOTEQ (quad * );
extern void generate_IF_GREATER (quad * );
extern void generate_IF_GREATEREQ (quad * );
extern void generate_IF_LESS (quad * );
extern void generate_IF_LESSEQ (quad * );
extern void generate_NOT (quad * );
extern void generate_OR (quad * );
extern void generate_PARAM (quad * );
extern void generate_CALL (quad * );
extern void generate_GETRETVAL (quad * );
extern void generate_FUNCSTART (quad * );
extern void generate_RETURN (quad * );
extern void generate_FUNCEND (quad * );

typedef void (* generator_func_t) (quad * ) ;

generator_func_t generators [25] = {
    generate_ASSIGN ,
    generate_ADD, 
    generate_SUB ,
    generate_MUL, 
    generate_DIV,
    generate_MOD ,
    generate_IF_EQ ,
    generate_IF_NOTEQ ,
    generate_IF_LESSEQ ,
    generate_IF_GREATEREQ ,
    generate_IF_LESS ,
    generate_IF_GREATER ,
    generate_CALL ,
    generate_PARAM ,
    generate_RETURN ,
    generate_GETRETVAL ,
    generate_FUNCSTART ,
    generate_FUNCEND ,
    generate_NEWTABLE ,
    generate_TABLEGETELEM ,
    generate_TABLESETELEM ,
    generate_JUMP ,
    generate_OR ,
    generate_NOT ,
    generate_NOP ,
};

extern int currQuad;

void generateInstructions (void){
    for (unsigned i=1 ; i< currQuad ;  ++i)
        (*generators[quads[i].op]) (quads+i);
        
}

//-------------------EXECUTES---------------------------------------------
/*
typedef void (*execute_func_t)(instruction * );

#define AVM_MAX_INSTRUCTIONS (unsigned) nop_v

extern void execute_assign (instruction *);
extern void execute_add (instruction *);
extern void execute_sub (instruction *);
extern void execute_mul (instruction *);
extern void execute_div (instruction *);
extern void execute_mod (instruction *);
extern void execute_uminus (instruction *); // !!! 100 line ews 103 o sav stis dialeksis tu ta exei me kokina den kserw ean xriazonte
extern void execute_and (instruction *);
extern void execute_or (instruction *);
extern void execute_not (instruction *);
extern void execute_jeq (instruction *);
extern void execute_jne (instruction *);
extern void execute_jle (instruction *);
extern void execute_jge (instruction *);
extern void execute_jlt (instruction *);
extern void execute_lgt (instruction *);
extern void execute_call (instruction *);
extern void execute_pusharg (instruction *);
extern void execute_funcenter (instruction *);
extern void execute_funcexit (instruction *);
extern void execute_newtable (instruction *);
extern void execute_tablegetelem (instruction *);
extern void execute_tablesetelem (instruction *);
extern void execute_nop (instruction *);



execute_func_t executeFuncs[] = {
    execute_assign ,
    execute_add ,
    execute_sub ,
    execute_mul ,
    execute_div ,
    execute_mod ,
    execute_uminus , // !!! 100 line ews 103 o sav stis dialeksis tu ta exei me kokina den kserw ean xriazonte
    execute_and ,
    execute_or ,
    execute_not ,
    execute_jeq ,
    execute_jne ,
    execute_jle ,
    execute_jge ,
    execute_jlt ,
    execute_lgt ,
    execute_call ,
    execute_pusharg ,
    execute_funcenter ,
    execute_funcexit ,
    execute_newtable ,
    execute_tablegetelem ,
    execute_tablesetelem ,
    execute_nop ,

};

unsigned char executionFinished = 0 ; 
unsigned pc = 0;
unsigned currLine = 0 ;
unsigned codeSize = 0 ; 
instruction * code = (instruction * ) 0 ; 
#define AVM_ENDING_PC    codeSize

void execute_cycle(void){
    if (executionFinished) return;
    else
        if (pc == AVM_ENDING_PC){
            executionFinished =1 ;
            return;
        }else{
            assert(pc < AVM_ENDING_PC);
            instruction * instr = code + pc;
            assert(instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);

            if(instr->scrLine)
                currLine = instr->scrLine;
            unsigned oldPc = pc;
            (*executeFuncs[instr->opcode]) (instr);
            if(pc == oldPc)
                ++pc;
        }    
}

*/
