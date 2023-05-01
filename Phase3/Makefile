
calc: parser.c scanner.c
	gcc -o calc scanner.c parser.c hash_table.c quads.c

scanner.c: scanner.l
	flex --outfile=scanner.c scanner.l

parser.c: parser.y
	bison --yacc --defines --output=parser.c parser.y

table.o: hash_table.c quads.c
	gcc hash_table.c quads.c

clean:
	rm -f calc scanner.c parser.c parser.h *o