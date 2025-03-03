CC = gcc
CFLAGS = -Wall -Wextra -O2

all: program.exe

program.exe: main.o graph_generator.o user_input.o
	$(CC) -o program.exe main.o graph_generator.o user_input.o

main.o: main.c graph_generator.h user_input.h
	$(CC) -c main.c -o main.o $(CFLAGS)

graph_generator.o: graph_generator.c graph_generator.h
	$(CC) -c graph_generator.c -o graph_generator.o $(CFLAGS)

user_input.o: user_input.c user_input.h graph_generator.h
	$(CC) -c user_input.c -o user_input.o $(CFLAGS)

clean:
	del /F /Q main.o graph_generator.o user_input.o program.exe