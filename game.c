#include <ncurses.h>
#include <stdlib.h>
#include "game.h"

int checkForCollision(Bird bird)
{
    if(bird.birdY < 0 || bird.birdY > LINES){return 1;}
    for(int y = 0; y < birdH; y++)
    {
        for(int x = 0; x < birdW; x++)
        {
            char current = mvinch(y + bird.birdY, x + bird.birdX) & A_CHARTEXT;
            if(bird.bird[y][x] == BRID && (current == PIPE || current == WALL))
            {
                return 1;
            }
        }
    }
    return 0;
}

void drawSimplePipes(float distanceSinceStart)
{
    //simple algorithm
    //pipes are spaced evenly from one another
    float distanceBetweenPipes = COLS/3;
    int firstPipeDistance = 1*COLS;
    if(distanceSinceStart + COLS < firstPipeDistance){return;}
    int backIndex = (distanceSinceStart - PIPE_WIDTH*COLS - firstPipeDistance)/distanceBetweenPipes;
    int frontIndex = (distanceSinceStart + COLS - firstPipeDistance)/distanceBetweenPipes;
    for(int i = (backIndex > 0 ? backIndex : 0); i <= frontIndex; i++)
    {
        drawPipe(firstPipeDistance + i*distanceBetweenPipes - distanceSinceStart, LINES/2, 1);
        drawPipe(firstPipeDistance + i*distanceBetweenPipes - distanceSinceStart, (int)(LINES*(0.5 - PIPE_GAP)), 0);
    }
}

void drawBetterPipes(Map map, float distanceSinceStart)
{
    float progress = distanceSinceStart/(float)map.finalDifficultyDistance;
    if(progress > 1){progress = 1;}
    int backIndex = (distanceSinceStart - PIPE_WIDTH*COLS - map.firstPipeDistance)/(float)map.distanceBetweenPipes;
    int frontIndex = (distanceSinceStart + COLS - map.firstPipeDistance)/(float)map.distanceBetweenPipes;
    
    for(int i = (backIndex > 0 ? backIndex : 0); i <= frontIndex; i++)
    {

    }
}

void drawBird(Bird bird)
{
    for(int y = 0; y < birdH; y++)
    {
        for(int x = 0; x < birdW; x++)
        {
            mvaddch(y + (int)bird.birdY, x + (int)bird.birdX, bird.bird[y][x]);
        }
    }
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