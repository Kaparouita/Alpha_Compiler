
#include    "vm.h"

int curr_i = 0;
instruction* instructions = (instruction*) 0;
double*     numConsts           = (double*)0;
unsigned    totalNumConsts      = 0;
char**      stringConsts        = (char**) 0;       
unsigned    totalStringConsts   = 0;
char**       namedLibfuncs       = (char **)0;
unsigned    totalNamedLibfuncs  = 0;
userfunc*   userFuncs           = (userfunc*)0;
unsigned    totalUserFuncs      = 0;



int main(){
    readBinary("binary.abc");
    print_all_i(instructions,curr_i);
    return 0;
}