#include "vm.h"


extern quad* quads;
instruction* intructions = (instruction*) 0; // opws k ta quads
int curr_i  = 0;
int total_i = 0;

void emit_i(instruction t) {

    if (curr_i == total_i)
        expand_instructions();

    intructions[curr_i].opcode = t.opcode;
    intructions[curr_i].result = t.result;
    intructions[curr_i].arg1 = t.arg1;
    intructions[curr_i].arg2 = t.arg2;
    intructions[curr_i].scrLine = t.scrLine;
    ++curr_i;
}

generate (vmopcode op,quad quad) {
    instruction t;
    t.opcode = op;
    make_operand(quad.arg1, &t.arg1);
    make_operand(quad.arg2, &t.arg2);
    make_operand(quad.result, &t.result);
    quad.label = nextinstructionlabel();
    emit_i(t);
}



generate_ADD (quad q) { generate(add, q); }
generate_SUB (quad q) { generate(sub, q); }
generate_MUL (quad q) { generate(mul, q); }
generate_DIV (quad q) { generate(div, q); }
generate_MOD (quad q) { generate(mod, q); }

generate_NEWTABLE       (quad q) { generate(newtable_e, q); }
generate_TABLEGETELM    (quad q) { generate(tablegetelem, q); }
generate_TABLESETELEM   (quad q) { generate(tablesetelem, q); }
generate_ASSIGN         (quad q) { generate(assign, q); }
generate_NOP            () { instruction t; t.opcode = nop_v; emit_i(t); }


//auth thelei ftiaksimo
generate_relational (vmopcode op,quad quad) {
    instruction t;
    t.opcode = op;
    make_operand(quad.arg1, &t.arg1);
    make_operand(quad.arg2, &t.arg2);
    t.result.type = label_a;
    if (quad.label < nextquadlabel()) 
        t.result.val = quads[quad.label].label;
    else
        add_incomplete_jump(nextinstructionlabel(), quad.label);
    quad.label = nextinstructionlabel();
    emit_i(t);
} 


generate_JUMP           (quad q) { generate_relational(jump, q); }
generate_IF_EQ          (quad q) { generate_relational(jeq_v, q); }
generate_IF_NOTEQ       (quad q) { generate_relational(jnoteq_v, q); }
generate_IF_GREATER     (quad q) { generate_relational(jgreater_v, q); }
generate_IF_GREATEREQ   (quad q) { generate_relational(jgreatereq_v, q); }
generate_IF_LESS        (quad q) { generate_relational(jless_v, q); }
generate_IF_LESSEQ      (quad q) { generate_relational(jlesseq_v, q); }


generate_NOT (quad quad) {
    quad.label = nextinstructionlabel();
    instruction t;
    t.opcode = jeq_v;
    make_operand(quad.arg1, &t.arg1);
    make_booloperand(&t.arg2, 0);
    t.result.type = label_a;
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
    make_booloperand(&t.arg1, 1);
    reset_operand(&t.arg2);
    make_operand(quad.result, &t.result);
    emit_i(t);
} 

generate_OR (quad quad) {
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


generate_PARAM(quad quad) {
    quad.label = nextinstructionlabel();
    instruction t;
    t.opcode = pusharg_v;
    make_operand(quad.arg1, &t.arg1);
    emit_i(t);
}
generate_CALL(quad quad) {
    quad.label = nextinstructionlabel();
    instruction t;
    t.opcode = call_v;
    make_operand(quad.arg1, &t.arg1);
    emit_i(t);
}
generate_GETRETVAL(quad quad) {
    quad.label = nextinstructionlabel();
    instruction t;
    t.opcode = assign;
    make_operand(quad.result, &t.result);
    make_retvaloperand(&t.arg1);
    emit_i(t);
} 