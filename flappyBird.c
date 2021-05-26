#include <unistd.h>
#include <ncurses.h>
#include <sys/time.h>
#include <math.h>
#include "game.h"

int main()
{
    //initialise the screen
    initscr();
    cbreak();
    curs_set(FALSE);
    noecho();
    nodelay(stdscr, TRUE);
    
    int startGame = 0;

    //physics parameters
    const float g = 3*LINES;
    const float birdVJump = -sqrt(2*g*LINES/7); 
    float distanceSinceStart;

    Bird bird;

    //initialise map parameters
    Map map;
    
    map.distanceBetweenPipes = COLS/3;
    map.finalDifficultyDistance = 10*COLS;
    map.firstPipeDistance = 1.5*COLS;
    map.maxHeightDifference = LINES/2;
    map.seed = 1000;
    int heights[10]; //an array of the heights of the pipes visible on the screen, with a bit of overflow
    for(int i = 0; i < sizeof(heights)/sizeof(heights[0]); i++)
    {
        heights[i] = -1;
    }
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
            bird.birdX = COLS/4;
            bird.birdY = LINES/2;
            bird.birdVY = 0;
            bird.birdVX = COLS/6;

            for(int h = 0; h < birdH; h++)
            {
                for(int w = 0; w < birdW; w++)
                {
                    bird.bird[h][w] = 'o';
                }
            }
            distanceSinceStart = 0;
            gettimeofday(&previousFrame, NULL);
        }
        while(startGame)
        {
            clear();
            drawWalls();
            //drawSimplePipes(distanceSinceStart);
            //draw more complex pipes
            drawBetterPipes(map, distanceSinceStart);

            //update position of brid and pipes
            c = getch();
            if(c == ' ')
            {
                //should jump
                bird.birdVY = birdVJump;
            }
            gettimeofday(&thisFrame, NULL);
            float timeElapsed = (thisFrame.tv_sec - previousFrame.tv_sec)*1000000 + thisFrame.tv_usec - previousFrame.tv_usec;
            timeElapsed /= 1000000;
            previousFrame = thisFrame;
            
            bird.birdVY += g*timeElapsed;
            bird.birdY += bird.birdVY*timeElapsed;
            distanceSinceStart += timeElapsed*bird.birdVX;

            if(checkForCollision(bird) == 1)
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
            drawBird(bird);
            refresh();
            //wait for next frame
            int timeLeft = dt - (int)timeElapsed*1000000;
            if(timeLeft > 0){usleep(timeLeft);}
        }
        refresh();
        usleep(dt);
    }
    
    endwin();
    return 0;
}