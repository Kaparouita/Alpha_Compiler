#include "vm.h"

extern unsigned codeSize ;
extern instruction* code ;
extern double*     numConsts           ;
extern unsigned    totalNumConsts      ;
extern char**      stringConsts        ;       
extern unsigned    totalStringConsts   ;
extern char**       namedLibfuncs      ;
extern unsigned    totalNamedLibfuncs  ;
extern userfunc*   userFuncs           ;
extern unsigned    totalUserFuncs      ;
extern int    global_vars;     


void readInstractions(FILE* file){
        fread(&codeSize, sizeof(unsigned), 1, file);
        code = malloc(codeSize * sizeof(instruction));
        if (code == NULL) {
            printf("Failed to allocate memory for instructions array.\n");
            return;
        }
        for (unsigned i = 0; i < codeSize; i++) {
            fread(&(code[i].opcode), sizeof(vmopcode), 1, file);
            fread(&(code[i].arg1.type), sizeof(vmarg_t), 1, file);
            fread(&(code[i].arg1.val), sizeof(unsigned), 1, file);
            fread(&(code[i].arg2.type), sizeof(vmarg_t), 1, file);
            fread(&(code[i].arg2.val), sizeof(unsigned), 1, file);
            fread(&(code[i].result.type), sizeof(vmarg_t), 1, file);
            fread(&(code[i].result.val), sizeof(unsigned), 1, file);
            fread(&(code[i].scrLine), sizeof(unsigned), 1, file);
    }
}

void read_all_data_binary(FILE* file) {
    // Read the numConsts
    fread(&totalNumConsts, sizeof(unsigned), 1, file);
    numConsts = malloc(totalNumConsts * sizeof(double));
    if (numConsts == NULL) {
        printf("Failed to allocate memory for numConsts array.\n");
        return;
    }
    for (unsigned i = 0; i < totalNumConsts; i++) {
        fread(&numConsts[i], sizeof(double), 1, file);
    }

    // Read the stringConsts
    fread(&totalStringConsts, sizeof(unsigned), 1, file);
    stringConsts = malloc(totalStringConsts * sizeof(char*));
    if (stringConsts == NULL) {
        printf("Failed to allocate memory for stringConsts array.\n");
        return;
    }
    for (unsigned i = 0; i < totalStringConsts; i++) {
        unsigned stringLength;
        fread(&stringLength, sizeof(unsigned), 1, file);
        stringConsts[i] = malloc((stringLength + 1) * sizeof(char));
        fread(stringConsts[i], sizeof(char), stringLength, file);
        stringConsts[i][stringLength] = '\0';  // Null-terminate the string
    }

    // Read the namedLibfuncs
    fread(&totalNamedLibfuncs, sizeof(unsigned), 1, file);
    namedLibfuncs = malloc(totalNamedLibfuncs * sizeof(char*));
    if (namedLibfuncs == NULL) {
        printf("Failed to allocate memory for namedLibfuncs array.\n");
        return;
    }
    for (unsigned i = 0; i < totalNamedLibfuncs; i++) {
        unsigned stringLength;
        fread(&stringLength, sizeof(unsigned), 1, file);
        namedLibfuncs[i] = malloc((stringLength + 1) * sizeof(char));
        fread(namedLibfuncs[i], sizeof(char), stringLength, file);
        namedLibfuncs[i][stringLength] = '\0';  // Null-terminate the string
    }

    // Read the userFuncs
    fread(&totalUserFuncs, sizeof(unsigned), 1, file);
    userFuncs = malloc(totalUserFuncs * sizeof(userfunc));
    if (userFuncs == NULL) {
        printf("Failed to allocate memory for userFuncs array.\n");
        return;
    }
    for (unsigned i = 0; i < totalUserFuncs; i++) {
        fread(&userFuncs[i].address, sizeof(unsigned), 1, file);
        fread(&userFuncs[i].localSize, sizeof(unsigned), 1, file);
        unsigned stringLength;
        fread(&stringLength, sizeof(unsigned), 1, file);
        userFuncs[i].id = malloc((stringLength + 1) * sizeof(char));
        fread(userFuncs[i].id, sizeof(char), stringLength, file);
        userFuncs[i].id[stringLength] = '\0';  // Null-terminate the string
    }
}


void readBinary(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Failed to open the file for reading.\n");
        return;  // Return an error code
    }
    unsigned magicNumber;
    fread(&magicNumber, sizeof(unsigned), 1, file);
    // Verify the magic number if needed
    if (magicNumber != 340200501) {
        printf("Invalid magic number. The file is not in the expected format.\n");
        fclose(file);
        return;
    }
    fread(&global_vars, sizeof(int), 1, file);
    // Read each instruction from the file
    readInstractions(file);

    // Read all lists
    read_all_data_binary(file);

    fclose(file);
}
