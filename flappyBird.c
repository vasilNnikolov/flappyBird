#include <unistd.h>
#include <ncurses.h>
#include <sys/time.h>
#include <math.h>

#define WALL '@' 
#define PIPE '#'
#define BRID 'o'
#define birdW 3
#define birdH 2

#define PIPE_WIDTH 0.1f
#define PIPE_GAP 0.3f 

#define dt 30000
const char brid[birdH][birdW] = {{'o', 'o', 'o'},
                                 {'o', 'o', 'o'}};


void drawBird(float birdX, float birdY)
{
    for(int y = 0; y < birdH; y++)
    {
        for(int x = 0; x < birdW; x++)
        {
            mvaddch(y + birdY, x + birdX, brid[y][x]);
        }
    }
}

int checkForCollision(int birdX, int birdY)
{
    if(birdX < 0 || birdX > LINES){return 1;}
    for(int y = 0; y < birdH; y++)
    {
        for(int x = 0; x < birdW; x++)
        {
            char current = mvinch(y + birdY, x + birdX) & A_CHARTEXT;
            if(brid[y][x] == BRID && (current == PIPE || current == WALL))
            {
                //detected collision
                return 1;
            }
        }
    }
    return 0;
}
void drawWalls()
{
    //draw roof and floor
    for(int i = 0; i < COLS; i++)
    {
        mvaddch(0, i, WALL);
        mvaddch(LINES - 1, i, WALL);
    }
}
void drawPipe(int xCoord, int height, int isTopPipe)
{
    for(int i = (0 > xCoord) ? 0 : xCoord; i < xCoord + PIPE_WIDTH*COLS && i < COLS; i++)
    {
        if(isTopPipe)
        {
            for(int j = 0; j < height; j++)
            {
                mvaddch(j, i, PIPE);
            }
        }    
        else
        {
            for(int j = LINES - 1; j > LINES - height; j--)
            {
                mvaddch(j, i, PIPE);
            }
        }
    }        
}


void drawSimplePipes(float distanceSinceStart)
{
    //simple algorithm
    //pipes are spaced evenly from one another
    if(distanceSinceStart < 0){return;}
    float distanceBetweenPipes = COLS/2;
    int nPipe = distanceSinceStart/distanceBetweenPipes;
    for(int x = nPipe - 1; x < nPipe + COLS/distanceBetweenPipes + 1; x++) 
    {
        drawPipe(x*distanceBetweenPipes - distanceSinceStart, LINES/2 - (int)(PIPE_GAP*LINES), 1);
        drawPipe(x*distanceBetweenPipes - distanceSinceStart, LINES/2, 0);
    }
}

void drawGameOver()
{
    mvaddstr(LINES/2, COLS/2, "GAME OVER");
    mvaddstr(LINES/2 + 1, COLS/2, "press space to restart game");
    refresh();
}

void drawTitleScreen()
{
    mvaddstr(LINES/2, COLS/2, "flappy");
    mvaddstr(LINES/2 + 1, COLS/2, "bird");

    mvaddstr(LINES - 1, 0, "press space to start, q to quit");
}

int main()
{
    //initialise the screen
    initscr();
    cbreak();
    curs_set(FALSE);
    noecho();
    nodelay(stdscr, TRUE);
    int startGame = 0;
    const float g = 2*LINES;
    const float birdVJump = -sqrt(2*g*LINES/5); 
    float birdX, birdY, birdVY, birdVX, distanceSinceStart;
    struct timeval thisFrame, previousFrame;
    while(1)
    {
        clear();
        drawTitleScreen();
        char c = getch();
        if(c == 'q')
        {
            break;
        }
        else if(c == ' ')
        {
            //start game
            startGame = 1;
            birdX = COLS/4;
            birdY = LINES/2;
            birdVY = 0;
            birdVX = COLS/4;
            distanceSinceStart = -2*COLS;
            gettimeofday(&previousFrame, NULL);
        }
        while(startGame)
        {
            clear();
            drawWalls();
            drawSimplePipes(distanceSinceStart);

            //update position of brid and pipes
            c = getch();
            if(c == ' ')
            {
                //should jump
                birdVY = birdVJump;
            }
            gettimeofday(&thisFrame, NULL);
            float timeElapsed = (thisFrame.tv_sec - previousFrame.tv_sec)*1000000 + thisFrame.tv_usec - previousFrame.tv_usec;
            timeElapsed /= 1000000;
            previousFrame = thisFrame;
            
            birdVY += g*timeElapsed;
            birdY += birdVY*timeElapsed;
            distanceSinceStart += timeElapsed*birdVX;

            if(checkForCollision(birdX, birdY) == 1)
            {
                //game over
                drawGameOver();
                
                //wait for refresh
                char exit;
                while(exit != ' ')
                {
                    exit = getch();
                }
                startGame = 0;
                break;
            }
            //no collision between bird and pipe or walls
            drawBird(birdX, birdY);
            refresh();
            //wait for next frame
            usleep(dt);
        }
        refresh();
        usleep(dt);
    }
    
    endwin();
    return 0;
}