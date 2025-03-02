CC = gcc
CFLAGS = -Wall -Wextra -O2

all: program.exe

program.exe: main.o graph_generator.o
	$(CC) -o program.exe main.o graph_generator.o

main.o: main.c graph_generator.h
	$(CC) -c main.c -o main.o

graph_generator.o: graph_generator.c graph_generator.h
	$(CC) -c graph_generator.c -o graph_generator.o

clean:
	del /F /Q main.o graph_generator.o program.exe