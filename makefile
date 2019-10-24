SHA: SHA.o main.o
	gcc -o SHA SHA.o main.o
SHA.o: SHA.c SHA.h
	gcc -c SHA.c
main.o: main.c SHA.h
	gcc -c main.c

clear:
	rm *.o SHA
