
#define _XOPEN_SOURCE 700
#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define WIDTH 25
#define HEIGHT 25
#define FOODS 50
#define MAX_SNAKE_LENGTH (WIDTH * HEIGHT)

// Structures
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

// Variables globales (déclarées dans game.c)
extern Cell gameBoard[HEIGHT][WIDTH];
extern Food food[FOODS];
extern int isGameOver;

// Prototypes des fonctions
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
