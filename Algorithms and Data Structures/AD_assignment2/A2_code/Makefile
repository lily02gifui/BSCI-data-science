all: map1 map2
map1: main.o list.o dict.o inout.o tree.o map1.o
	gcc -g -o map1 main.o list.o dict.o inout.o tree.o map1.o
map2: main.o list.o dict.o inout.o tree.o map2.o
	gcc -g -o map2 main.o list.o dict.o inout.o tree.o map2.o
list.o: list.c list.h inout.h
	gcc -g -o list.o -c -Wall list.c
dict.o: dict.c dict.h list.h inout.h
	gcc -g -o dict.o -c -Wall dict.c
inout.o: inout.c inout.h list.h
	gcc -g -o inout.o -c -Wall inout.c
tree.o: tree.c tree.h list.h dict.h
	gcc -g -o tree.o -c -Wall tree.c
main.o: main.c list.h dict.h tree.h map1.h map2.h
	gcc -g -o main.o -c -Wall main.c
map1.o: map1.c map1.h list.h tree.h
	gcc -g -o map1.o -c -Wall map1.c
map2.o: map2.c map2.h list.h tree.h
	gcc -g -o map2.o -c -Wall map2.c