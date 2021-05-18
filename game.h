#ifndef GAME_H
#define GAME_H

#define PIPE_WIDTH 0.08f
#define PIPE_GAP 0.2f
#define dt 33333

#define WALL '@' 
#define PIPE '#'
#define BRID 'o'
#define birdW 3
#define birdH 2

typedef struct Bird 
{
    float birdX, birdY, birdVX, birdVY;
    char bird[birdH][birdW];
} Bird;

typedef struct Map
{
    int distanceBetweenPipes, firstPipeDistance, finalDifficultyDistance, maxHeightDifference;
    int seed;
} Map;

int checkForCollision(Bird bird);

void drawSimplePipes(float distanceSinceStart);

void drawBetterPipes(Map map, float distanceSinceStart);

void drawBird(Bird bird);

void drawWalls();

void drawPipe(int xCoord, int height, int isTopPipe);

void drawGameOver();

void drawTitleScreen();

#endif