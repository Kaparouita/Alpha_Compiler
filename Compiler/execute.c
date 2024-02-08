#include "avm.h"
#include "vm.h"



unsigned currLine = 0;
unsigned codeSize = 0;
instruction* code = (instruction*) 0;
double*     numConsts           = (double*)0;
unsigned    totalNumConsts      = 0;
char**      stringConsts        = (char**) 0;       
unsigned    totalStringConsts   = 0;
char**       namedLibfuncs       = (char **)0;
unsigned    totalNamedLibfuncs  = 0;
userfunc*   userFuncs           = (userfunc*)0;
unsigned    totalUserFuncs      = 0;
int global_vars = 0;

userfunc* avm_getfuncinfo(unsigned address) {
    for(int i=0; i<totalUserFuncs ;++i)
        if(userFuncs[i].address == address)
            return &userFuncs[i];
    return NULL;
}

typedef void (*execute_func_t)(instruction * );

#define AVM_MAX_INSTRUCTIONS (unsigned) nop_v

extern void execute_assign (instruction *);//
extern void execute_add (instruction *);//
extern void execute_sub (instruction *);//
extern void execute_mul (instruction *);//
extern void execute_div (instruction *);//
extern void execute_mod (instruction *);//
extern void execute_jeq (instruction *);
extern void execute_jne (instruction *);
extern void execute_jle (instruction *);
extern void execute_jge (instruction *);
extern void execute_jlt (instruction *);
extern void execute_jgt (instruction *);
extern void execute_call (instruction *);//
extern void execute_pusharg (instruction *);//
extern void execute_funcenter (instruction *);//
extern void execute_funcexit (instruction *);//
extern void execute_newtable (instruction *);//
extern void execute_tablegetelem (instruction *);//
extern void execute_tablesetelem (instruction *);//
extern void execute_jump (instruction *);
extern void execute_nop (instruction *);



execute_func_t executeFuncs[] = {
    execute_assign , //0
    execute_add ,   //1
    execute_sub ,   //2
    execute_mul ,   //3
    execute_div ,   //4
    execute_mod ,   //5
    execute_jeq ,   //6
    execute_jne ,   //7
    execute_jle ,   //8
    execute_jge ,   //9
    execute_jlt ,   //10
    execute_jgt ,   //11
    execute_call ,  //12
    execute_pusharg ,//13
    execute_funcenter ,//14
    execute_funcexit ,//15
    execute_newtable ,//16
    execute_tablegetelem ,//17
    execute_tablesetelem ,//18
    execute_jump,//19
    execute_nop ,//20

};



void execute_cycle(void){
    while(!executionFinished){
    
        if (pc == AVM_ENDING_PC){
            printf("DONE : %u\n",pc);
            executionFinished =1 ;
            return;
        }
        else{
            printf("PC : %u  ,",pc);
            assert(pc < AVM_ENDING_PC);
            instruction * instr = code + pc;
            assert(instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);
            printf("OpCode : %s  ,",getOpcodeString(instr->opcode));
            if(instr->scrLine)
                currLine = instr->scrLine;
            unsigned oldPc = pc;
            (*executeFuncs[instr->opcode]) (instr);
            if(pc == oldPc) //an den exei ginei jump
                ++pc;
            printf("PC_A : %u\n",pc);
        }    
    }
}

int main(){
    readBinary("binary.abc");
    avm_initialize();
    execute_cycle();
    //print_all_i(code,codeSize);
    printf("%d",codeSize);
    //print_all_lists(numConsts,totalNumConsts,stringConsts,totalStringConsts,namedLibfuncs,totalNamedLibfuncs,userFuncs,totalUserFuncs);
    
    return 0;
}