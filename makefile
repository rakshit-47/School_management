CC = gcc
DB = -I/usr/include/postgresql 
json = -lpq -lcjson -g

final: main_project_file.o library.o 
	$(CC) main_project_file.o library.o -o project $(json)

main_project_file.o: main_project_file.c library.h
	$(CC) -c main_project_file.c $(DB)

library.o: library.c library.h 
	$(CC) -c library.c $(DB)

clean:
	rm -f *.o project
