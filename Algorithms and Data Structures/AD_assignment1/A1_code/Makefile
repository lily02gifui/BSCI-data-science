dict: main.o list.o dict.o inout.o
	gcc -o dict main.o list.o dict.o inout.o 
list.o: list.c list.h
	gcc -o list.o -c -Wall list.c
dict.o: dict.c dict.h list.h inout.h
	gcc -o dict.o -c -Wall dict.c
inout.o: inout.c inout.h list.h
	gcc -o inout.o -c -Wall inout.c
main.o: main.c list.h dict.h
	gcc -o main.o -c -Wall main.c