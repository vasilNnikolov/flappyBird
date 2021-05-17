CC = gcc

flappyBird: flappyBird.c game.c
	$(CC) -o flappyBird flappyBird.c game.c -lncurses -lm

clean:
	rm flappyBird 

