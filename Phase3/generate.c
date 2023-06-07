#include "vm.h"


extern quad* quads;
instruction* instructions = (instruction*) 0; // opws k ta quads
int curr_i  = 0;
int total_i = 0;

void emit_i(instruction t) {

    if (curr_i == total_i)
        expand_i();
    
    instructions[curr_i].opcode = t.opcode;
    instructions[curr_i].result = t.result;
    instructions[curr_i].arg1 = t.arg1;
    instructions[curr_i].arg2 = t.arg2;
    instructions[curr_i].scrLine = t.scrLine;
    ++curr_i;
printf("\n%d",curr_i);
}

void generate (vmopcode op,quad quad) {
    instruction i = instruction_constractor(&i,op,quad.line);
    //make_operand(quad.arg1, &i.arg1);
    make_operand(quad.arg2, &i.arg2);
    make_operand(quad.result, &i.result);
    quad.label = nextinstructionlabel();
    emit_i(i);
}



void generate_ADD (quad q) { generate(add, q); }
void generate_SUB (quad q) { generate(sub, q); }
void generate_MUL (quad q) { generate(mul, q); }
void generate_DIV (quad q) { generate(n_div, q); }
void generate_MOD (quad q) { generate(mod, q); }

void generate_NEWTABLE       (quad q) { generate(newtable_e, q); }
void generate_TABLEGETELEM   (quad q) { generate(tablegetelem, q); }
void generate_TABLESETELEM   (quad q) { generate(tablesetelem, q); }
void generate_ASSIGN         (quad q) { generate(assign, q); }
void generate_NOP            () { instruction t; t.opcode = nop_v; emit_i(t); }


//auth thelei ftiaksimo
void generate_relational (vmopcode op,quad quad) {
    instruction i = instruction_constractor(&i,op,quad.line);
    make_operand(quad.arg1, &i.arg1);
    make_operand(quad.arg2, &i.arg2);
    i.result.type = label_a;
    if (quad.label < nextquadlabel()) 
        i.result.val = quads[quad.label].label;
    else
        {}//add_incomplete_jump(nextinstructionlabel(), quad.label); //thelei ftiaksimo auth
    quad.label = nextinstructionlabel();
    emit_i(i);
} 


void generate_JUMP           (quad q) { generate_relational(jump, q); }
void generate_IF_EQ          (quad q) { generate_relational(jeq_v, q); }
void generate_IF_NOTEQ       (quad q) { generate_relational(jnoteq_v, q); }
void generate_IF_GREATER     (quad q) { generate_relational(jgreater_v, q); }
void generate_IF_GREATEREQ   (quad q) { generate_relational(jgreatereq_v, q); }
void generate_IF_LESS        (quad q) { generate_relational(jless_v, q); }
void generate_IF_LESSEQ      (quad q) { generate_relational(jlesseq_v, q); }


void generate_NOT (quad quad) {
    quad.label = nextinstructionlabel();
    instruction i = instruction_constractor(&i,jeq_v,quad.line);
    make_operand(quad.arg1, &i.arg1);
    make_booloperand(&i.arg2, 0);
    i.result.type = label_a;
    i.result.val = nextinstructionlabel()+3;
    emit_i(i);
    i.opcode = assign;
    make_booloperand(&i.arg1, 0);
    reset_operand(&i.arg2);
    make_operand(quad.result, &i.result);
    emit_i(i); 
    i.opcode = jump;
    reset_operand (&i.arg1);
    reset_operand(&i.arg2);
    i.result.type = label_a;
    i.result.val = nextinstructionlabel()+2;
    emit_i(i);
    i.opcode = assign;
    make_booloperand(&i.arg1, 1);
    reset_operand(&i.arg2);
    make_operand(quad.result, &i.result);
    emit_i(i);
} 

void generate_OR (quad quad) {
    quad.label = nextinstructionlabel();
    instruction t;
    t.opcode = jeq_v;
    make_operand(quad.arg1, &t.arg1);
    make_booloperand(&t.arg2,1);
    t.result.type = label_a;
    t.result.val = nextinstructionlabel()+4;
    emit_i(t);
    make_operand(quad.arg2, &t.arg1);
    t.result.val = nextinstructionlabel()+3;
    emit_i(t);
    t.opcode = assign;
    make_booloperand(&t.arg1, 0);
    reset_operand(&t.arg2);
    make_operand(quad.result, &t.result);
    emit_i(t);
    t.opcode = jump;
    reset_operand (&t.arg1);
    reset_operand(&t.arg2);
    t.result.type = label_a;
    t.result.val = nextinstructionlabel()+2;
    emit_i(t);
    t.opcode = assign;
    make_booloperand(&t.arg1,1);
    reset_operand(&t.arg2);
    make_operand(quad.result, &t.result);
    emit_i(t);
}  

//++ to generate tis AND alla katse na douleuoun sgra ta apo panw k meta tha to ftiaksw


void generate_PARAM(quad quad) {
    quad.label = nextinstructionlabel();
    instruction t;
    t.opcode = pusharg_v;
    make_operand(quad.arg1, &t.arg1);
    emit_i(t);
}
void generate_CALL(quad quad) {
    quad.label = nextinstructionlabel();
    instruction t;
    t.opcode = call_v;
    make_operand(quad.arg1, &t.arg1);
    emit_i(t);
}
void generate_GETRETVAL(quad quad) {
    quad.label = nextinstructionlabel();
    instruction t;
    t.opcode = assign;
    make_operand(quad.result, &t.result);
    make_retvaloperand(&t.arg1);
    emit_i(t);
} 

void generate_FUNCSTART(quad q) {
    instruction i = instruction_constractor(&i,funcstart_v,q.line);
    make_operand(q.result, &i.result);
    emit_i(i);
}

void generate_FUNCEND(quad q) {
    instruction i = instruction_constractor(&i,funcend_v,q.line);
    make_operand(q.result, &i.result);
    emit_i(i);
}

void generate_RETURN(quad q) {
    instruction i = instruction_constractor(&i,assign_v,q.line);
    i.result.type = retval_a;
    make_operand(q.result, &i.arg1);
    emit_i(i);
}