#define PIPE_WIDTH 0.1f
#define PIPE_GAP 0.3f 
#define dt 30000

#define WALL '@' 
#define PIPE '#'
#define BRID 'o'
#define birdW 3
#define birdH 2


typedef struct Bird 
{
    float birdX, birdY, birdVX, birdVY;
    char bird[birdH][birdW];
}Bird;

int checkForCollision(Bird bird);

void drawSimplePipes(float distanceSinceStart);

void drawBird(Bird bird);

void drawWalls();

void drawPipe(int xCoord, int height, int isTopPipe);

void drawGameOver();

void drawTitleScreen();