$(CC) = gcc
# last: final library.o
# 	./project
final: test.o library.o 
	$(CC) test.o library.o -o project -I/usr/include/postgresql -lpq -g
library.o: library.c library.h 
	$(CC) -c library.c -I/usr/include/postgresql -lpq -g
clear:
	rm *.o project

