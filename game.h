
#define _XOPEN_SOURCE 700
#ifndef GAME_H
#define GAME_H

#define BASE_DELAY 100000000  
#define MIN_DELAY  30000000   
#define SPEED_STEP 3000000   

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define WIDTH 30
#define HEIGHT 15
#define FOODS 50
#define MAX_SNAKE_LENGTH (WIDTH * HEIGHT)


typedef struct {
    char type;
} Cell;

typedef struct Snake {
    int bodyX[MAX_SNAKE_LENGTH];
    int bodyY[MAX_SNAKE_LENGTH];
    int length;
    char direction; // 'W','A','S','D'
} Snake;

typedef struct {
    int x, y;
    int consumed;
} Food;


extern Cell gameBoard[HEIGHT][WIDTH];
extern Food food[FOODS];
extern int isGameOver;


int  getch(void);
void fill_board(void);
void draw_food(void);
void draw_snake(const Snake *snake);
void clear_screen(void);
void print_board(void);
void drawGameBoard(const Snake *snake);

char getInput(void);
void moveSnake(Snake *snake, char input);
void checkCollisions(Snake *snake);

void setup_food(void);
void setup_snake(Snake *snake);

#endif
