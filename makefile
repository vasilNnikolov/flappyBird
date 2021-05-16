CC=gcc

flappyBird: flappyBird.o
	$(CC) -o flappyBird flappyBird.o -lncurses -lm

clean:
	rm flappyBird flappyBird.o