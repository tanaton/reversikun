# Xリバーシ君
PROGRAM = xreversikun
OBJ = main.o reversi.o display.o
CC = gcc
CFLAGS = -g -Wall
X = -lX11 -L/usr/X11R6/lib
RM = rm -rf

$(PROGRAM): $(OBJ)
	$(CC) $^ -o $(PROGRAM) $(CFLAGS) $(X)

main.o: main.c
	$(CC) $^ -c $(CFLAGS)

reversi.o: reversi.c
	$(CC) $^ -c $(CFLAGS)

display.o: display.c
	$(CC) $^ -c $(CFLAGS)

main.o: display.h reversi.h
reversi.o: reversi.h
display.o: display.h reversi.h

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJ) *.gch

