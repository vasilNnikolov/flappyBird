CC = gcc

flappyBird: flappyBird.o game.o 
	$(CC) -o flappyBird flappyBird.o game.o -lncurses -lm

flappyBird.o: flappyBird.c
	$(CC) -c flappyBird.c -lncurses -lm

game.o: game.c
	$(CC) -c game.c -lncurses -lm

clean:
	rm flappyBird game.o flappyBird.o

