CC=gcc

al: al.o 
	$(CC) -o al al.c

al.c: al.l
	flex --outfile=al.c al.l

clean:
	rm -f al.c
	rm -f *.o al
