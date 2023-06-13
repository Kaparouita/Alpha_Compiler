
calc: parser.c scanner.c
	gcc -o calc scanner.c parser.c table_k_quads/hash_table.c table_k_quads/quads.c Stack.c vm.c generate.c write.c print.c

main: avm.c read.c print.c execute.c executes/assign.c executes/call.c executes/arithmetics.c executes/jumps.c executes/table.c alpha_libs.c tostring.c
	gcc -o main avm.c read.c print.c execute.c executes/assign.c executes/call.c executes/arithmetics.c executes/jumps.c executes/table.c alpha_libs.c tostring.c

scanner.c: scanner.l
	flex --outfile=scanner.c scanner.l

parser.c: parser.y
	bison --yacc --defines --output=parser.c parser.y

table.o: hash_table.c quads.c
	gcc table_k_quads/hash_table.c table_k_quads/quads.c

clean:
	rm -f calc scanner.c parser.c parser.h main *o 