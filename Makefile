CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
OBJ = graph.o main.o
TARGET = graph
LIBS = -lcurl -lcjson
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) 

llm : llm.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET) llm
