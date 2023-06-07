
calc: parser.c scanner.c
	gcc -o calc scanner.c parser.c table_k_quads/hash_table.c table_k_quads/quads.c Stack.c vm.c generate.c generate2.c

scanner.c: scanner.l
	flex --outfile=scanner.c scanner.l

parser.c: parser.y
	bison --yacc --defines --output=parser.c parser.y

table.o: hash_table.c quads.c
	gcc table_k_quads/hash_table.c table_k_quads/quads.c

clean:
	rm -f calc scanner.c parser.c parser.h *o