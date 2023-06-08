
#include    "vm.h"

int curr_i;
instruction* instructions;
double*     numConsts           ;
unsigned    totalNumConsts      ;
char**      stringConsts        ;       
unsigned    totalStringConsts   ;
char**       namedLibfuncs      ;
unsigned    totalNamedLibfuncs  ;
userfunc*   userFuncs           ;
unsigned    totalUserFuncs      ;



int main(){
    readInstructionsFromFile("binary.abc");
    return 0;
}