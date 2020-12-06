CC=gcc
CFLAGS=-g -Wall
OBJS=main.o lexer.o parser.o run.o
TARGET=interpreter.out
 
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

main.o: total.h run.h main.c
lexer.o: total.h lexer.c
parser.o: total.h parser.c
run.o: total.h run.h run.c

clean:
	rm -f *.o
	rm -f $(TARGET)