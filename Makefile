CC = gcc
//CFLAGS = -Wall -Wextra -pedantic -g
OBJ = graph.o llm.o user_input.o main.o
TARGET = graph
LIBS = -lcurl -lcjson
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET) 
