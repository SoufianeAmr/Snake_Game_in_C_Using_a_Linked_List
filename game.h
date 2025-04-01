#define _XOPEN_SOURCE 700
#ifndef GAME_H
#define GAME_H

#define BASE_DELAY 150000000  
#define MIN_DELAY   30000000   
#define SPEED_STEP   2500000  

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define WIDTH 30
#define HEIGHT 15
#define MAX_SNAKE_LENGTH ((WIDTH-2) * (HEIGHT-2))

typedef struct {
    char type;
} Cell;


typedef struct SnakeSegment {
    int x, y;
    struct SnakeSegment* next;
} SnakeSegment;


typedef struct Snake {
    SnakeSegment* head;
    int length;
    char direction; // 'W','A','S','D'
} Snake;

typedef struct {
    int x, y;
    int consumed;
} Food;

extern Cell gameBoard[HEIGHT][WIDTH];
extern Food food;
extern int isGameOver;
extern int isGameWon;

int  getch(void);
void fill_board(void);
void draw_food(void);
void draw_snake(const Snake *snake);
void clear_screen(void);
void print_board(const Snake *snake);
void drawGameBoard(const Snake *snake);

char getInput(void);
void moveSnake(Snake *snake, char input);
void checkCollisions(Snake *snake);

void setup_food(void);
void setup_snake(Snake *snake);
int loadHighScore(void);
void saveScore(int highscore, Snake *snake);

#endif
