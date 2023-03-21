calc: parser.c scanner.l
	gcc -o calc scanner.c parser.c

scanner.c: scanner.l
	flex --outfile=scanner.c scanner.l

parser.c: parser.y
	bison --yacc --defines --output=parser.c parser.y

clean:
	rm -f calc scanner.c parser.c parser.h