#include "vm.h"

#define BINARY_FILE "binary.abc"

extern quad* quads;
extern double*     numConsts           ;
extern unsigned    totalNumConsts      ;
extern char**      stringConsts        ;       
extern unsigned    totalStringConsts   ;
extern char**       namedLibfuncs      ;
extern unsigned    totalNamedLibfuncs  ;
extern userfunc*   userFuncs           ;
extern unsigned    totalUserFuncs      ;

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


extern void generate_ADD (quad * );
extern void generate_SUB (quad * );
extern void generate_MUL (quad  *);
extern void generate_DIV (quad  *);
extern void generate_MOD (quad *);
extern void generate_NEWTABLE (quad*  );
extern void generate_TABLEGETELEM (quad*  );
extern void generate_TABLESETELEM (quad * );
extern void generate_ASSIGN (quad * );
extern void generate_NOP (quad * );
extern void generate_JUMP (quad*  );
extern void generate_IF_EQ (quad*  );
extern void generate_IF_NOTEQ (quad*  );
extern void generate_IF_GREATER (quad*  );
extern void generate_IF_GREATEREQ (quad*  );
extern void generate_IF_LESS (quad*  );
extern void generate_IF_LESSEQ (quad*  );
extern void generate_NOT (quad * );
extern void generate_AND (quad * );
extern void generate_OR (quad  *);
extern void generate_PARAM (quad * );
extern void generate_CALL (quad * );
extern void generate_GETRETVAL (quad*  );
extern void generate_FUNCSTART (quad * );
extern void generate_RETURN (quad  *);
extern void generate_FUNCEND (quad  *);


void (*const  generators[26]) (quad *) = {
    generate_ASSIGN ,   //0
    generate_ADD,       //1
    generate_SUB ,      //2
    generate_MUL,       //3
    generate_DIV,       //4
    generate_MOD ,      //5
    generate_IF_EQ ,    //6
    generate_IF_NOTEQ , //7
    generate_IF_LESSEQ ,//8 
    generate_IF_GREATEREQ ,//9
    generate_IF_LESS ,  //10
    generate_IF_GREATER ,//11
    generate_CALL ,     //12
    generate_PARAM ,    //13
    generate_RETURN ,   //14
    generate_GETRETVAL ,//15
    generate_FUNCSTART ,//16
    generate_FUNCEND ,  //17
    generate_NEWTABLE , //18
    generate_TABLEGETELEM ,//19
    generate_TABLESETELEM ,//20
    generate_JUMP ,     //21
    generate_AND ,      //22
    generate_OR ,       //23
    generate_NOT ,      //24
    generate_NOP ,      //25
};

extern int currQuad;
int curr_pro_quad = 0;
extern incomplete_jump * ij_head  ;

void generateInstructions (void){
    for (curr_pro_quad ; curr_pro_quad< currQuad ;  ++curr_pro_quad)
        (*generators[quads[curr_pro_quad].op]) (&quads[curr_pro_quad]);
    patch_incomplete_jumps(&ij_head,quads);
        
}

extern int curr_i;
extern instruction *instructions;

void write_all_data(FILE* file) {

    // Write numConsts
    fprintf(file, "Num Consts:\n");
    for (unsigned i = 0; i < totalNumConsts; i++) {
        fprintf(file, "%.2f\n", numConsts[i]);
    }

    // Write stringConsts
    fprintf(file, "\nString Consts:\n");
    for (unsigned i = 0; i < totalStringConsts; i++) {
        fprintf(file, "%s\n", stringConsts[i]);
    }

    // Write namedLibfuncs
    fprintf(file, "\nNamed Libfuncs:\n");
    for (unsigned i = 0; i < totalNamedLibfuncs; i++) {
        fprintf(file, "%s\n", namedLibfuncs[i]);
    }

    // Write userFuncs
    fprintf(file, "\nUser Funcs:\n");
    for (unsigned i = 0; i < totalUserFuncs; i++) {
        fprintf(file, "Address: %d, Local Vars: %u, ID: %s\n", userFuncs[i].address, userFuncs[i].localSize,userFuncs[i].id);
    }
}


void write_vmarg_formal(FILE* file, vmarg* e) {
    if (!e) {
        fprintf(file, "%-14s", "NULL");
        return;
    }
    switch (e->type) {
        case bool_a:
            if (e->val == 0)
                fprintf(file, "%-14s", "False");
            else
                fprintf(file, "%-14s", "True");
            break;
        case string_a:
            fprintf(file, "%-14d", e->val);
            break;
        case number_a:
            fprintf(file, "%-14.2f", (double)e->val);
            break;
        case label_a:
            fprintf(file, "%-14d", e->val);
            break;
        default : fprintf(file,"%-14s",get_varg_t_name(e->type));
    }
}

void write_i_formal(FILE* file, instruction* q) {
    fprintf(file, "%-14s", getOpcodeString(q->opcode));
    write_vmarg_formal(file, &q->arg1);
    write_vmarg_formal(file, &q->arg2);
    write_vmarg_formal(file, &q->result);
    fprintf(file, "[%d]\n", q->scrLine);
}

void write_all_i(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }
    fprintf(file, "%-8d\n\n", getglobalvars());
    fprintf(file, "\n\n/---------------------------------   PRINTING ALL INSTRUCTIONS    ----------------------------------/\n\n");
    fprintf(file, "%-8s%-14s%-14s%-14s%-14s%-14s\n\n", "NO", "OP", "ARG1", "ARG2", "RESULT", "LINE");

    int i = 0;
    while (i < curr_i) {
        fprintf(file, "%-8d", i);
        write_i_formal(file, &instructions[i]);
        i++;
    }
    
    write_all_data(file);

    fclose(file);
}

void writeInstructions(FILE* file){
    fwrite(&curr_i, sizeof(unsigned), 1, file);
    for(int i = 0;i<curr_i;i++){
        fwrite(&instructions->opcode, sizeof(vmopcode),1, file); 
        fwrite(&instructions->arg1.type, sizeof(vmarg_t),1, file);  
        fwrite(&instructions->arg1.val, sizeof(unsigned),1, file);   
        fwrite(&instructions->arg2.type, sizeof(vmarg_t),1, file);  
        fwrite(&instructions->arg2.val, sizeof(unsigned),1, file);
        fwrite(&instructions->result.type, sizeof(vmarg_t),1, file);  
        fwrite(&instructions->result.val, sizeof(unsigned),1, file); 
        fwrite(&instructions->scrLine, sizeof(unsigned),1, file); 
        instructions++;  
    }
}


void writeBinaryFile(const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Failed to open the file for writing.\n");
        return;
    }
    // Write the magic number
    unsigned MAGIC_NUMBER = 340200501;
    fwrite(&MAGIC_NUMBER, sizeof(unsigned), 1, file);
    int i = getglobalvars();
    fwrite(&i, sizeof(int), 1, file);
    // Write the instructions
    writeInstructions(file);
    //Write lists
    write_all_data_binary(file);

    fclose(file);
}


void write_all_data_binary(FILE* file) {
        // Write the numConsts
    fwrite(&totalNumConsts, sizeof(unsigned), 1, file);
    for(unsigned i = 0 ;i < totalNumConsts;i++){
        fwrite(&numConsts[i], sizeof(double), 1, file);
    }
    // Write the stringConsts
    fwrite(&totalStringConsts, sizeof(unsigned), 1, file);
    for (unsigned i = 0; i < totalStringConsts; i++) {
        unsigned stringLength = strlen(stringConsts[i]);
        fwrite(&stringLength, sizeof(unsigned), 1, file);
        fwrite(stringConsts[i], sizeof(char), stringLength, file);
    }

    // Write the namedLibfuncs
    fwrite(&totalNamedLibfuncs, sizeof(unsigned), 1, file);
    for (unsigned i = 0; i < totalNamedLibfuncs; i++) {
        unsigned stringLength = strlen(namedLibfuncs[i]);
        fwrite(&stringLength, sizeof(unsigned), 1, file);
        fwrite(namedLibfuncs[i], sizeof(char), stringLength, file);
    }

    // Write the userFuncs
    fwrite(&totalUserFuncs, sizeof(unsigned), 1, file);
    for (unsigned i = 0; i < totalUserFuncs; i++) {
        fwrite(&userFuncs[i].address, sizeof(unsigned), 1, file);
        fwrite(&userFuncs[i].localSize, sizeof(unsigned), 1, file);
        unsigned stringLength = strlen(userFuncs[i].id);
        fwrite(&stringLength, sizeof(unsigned), 1, file);
        fwrite(userFuncs[i].id, sizeof(char), stringLength, file);

    }
}
//-------------------EXECUTES---------------------------------------------
