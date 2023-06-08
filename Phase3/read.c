#include "vm.h"

extern int curr_i;
extern instruction* instructions;
extern double*     numConsts           ;
extern unsigned    totalNumConsts      ;
extern char**      stringConsts        ;       
extern unsigned    totalStringConsts   ;
extern char**       namedLibfuncs      ;
extern unsigned    totalNamedLibfuncs  ;
extern userfunc*   userFuncs           ;
extern unsigned    totalUserFuncs      ;

void readInstructionsFromFile(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Failed to open the file for reading.\n");
        return;
    }

    // Read the instruction count
    fread(&curr_i, sizeof(size_t), 1, file);

    // Allocate memory for the instructions
    instructions = malloc(sizeof(instruction) * (curr_i));
    if (instructions == NULL) {
        printf("Failed to allocate memory for instructions.\n");
        fclose(file);
        return;
    }

    // Read the instructions
    fread(&instructions, sizeof(instruction), curr_i, file);

    // Read the totalNumConsts
    fread(&totalNumConsts, sizeof(unsigned), 1, file);

    // Allocate memory for numConsts
    numConsts = malloc(sizeof(double) * (totalNumConsts));
    if (numConsts == NULL) {
        printf("Failed to allocate memory for numConsts.\n");
        fclose(file);
        free(instructions);
        return;
    }

    // Read numConsts
    fread(&numConsts, sizeof(double), totalNumConsts, file);

    // Read the totalStringConsts
    fread(&totalStringConsts, sizeof(unsigned), 1, file);

    // Allocate memory for stringConsts
    stringConsts = malloc(sizeof(char*) * (totalStringConsts));
    if (stringConsts == NULL) {
        printf("Failed to allocate memory for stringConsts.\n");
        fclose(file);
        free(instructions);
        free(numConsts);
        return;
    }

    // Read stringConsts
    for (unsigned i = 0; i < totalStringConsts; i++) {
        size_t stringLength;
        fread(&stringLength, sizeof(size_t), 1, file);
        (stringConsts)[i] = malloc(sizeof(char) * (stringLength + 1));
        fread((stringConsts)[i], sizeof(char), stringLength, file);
        (stringConsts)[i][stringLength] = '\0';
    }

    // Read the totalNamedLibfuncs
    fread(&totalNamedLibfuncs, sizeof(unsigned), 1, file);

    // Allocate memory for namedLibfuncs
    namedLibfuncs = malloc(sizeof(char*) * (totalNamedLibfuncs));
    if (namedLibfuncs == NULL) {
        printf("Failed to allocate memory for namedLibfuncs.\n");
        fclose(file);
        free(instructions);
        free(numConsts);
        for (unsigned i = 0; i < totalStringConsts; i++) {
            free((stringConsts)[i]);
        }
        free(stringConsts);
        return;
    }

    // Read namedLibfuncs
    for (unsigned i = 0; i < totalNamedLibfuncs; i++) {
        size_t stringLength;
        fread(&stringLength, sizeof(size_t), 1, file);
        (namedLibfuncs)[i] = malloc(sizeof(char) * (stringLength + 1));
        fread(&(namedLibfuncs)[i], sizeof(char), stringLength, file);
        (namedLibfuncs)[i][stringLength] = '\0';
    }

    // Read the totalUserFuncs
    fread(&totalUserFuncs, sizeof(unsigned), 1, file);

    // Allocate memory for userFuncs
    userFuncs = malloc(sizeof(userfunc) * (totalUserFuncs));
    if (userFuncs == NULL) {
        printf("Failed to allocate memory for userFuncs.\n");
        fclose(file);
        free(instructions);
        free(numConsts);
        for (unsigned i = 0; i < totalStringConsts; i++) {
            free((stringConsts)[i]);
        }
        free(stringConsts);
        for (unsigned i = 0; i < totalNamedLibfuncs; i++) {
            free((namedLibfuncs)[i]);
        }
        free(namedLibfuncs);
        return;
    }

    // Read userFuncs
    fread(&userFuncs, sizeof(userfunc), totalUserFuncs, file);

    fclose(file);
}
