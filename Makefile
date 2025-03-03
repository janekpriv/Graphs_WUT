CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
OBJ = graph.o main.o 
TARGET = graph

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)