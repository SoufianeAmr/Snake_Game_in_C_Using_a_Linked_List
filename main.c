#define _XOPEN_SOURCE 700  
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>        
#include "game.h"          

int main() {
    
    srand(time(NULL));

    Snake snake;
    setup_snake(&snake);
    setup_food();

    while (!isGameOver) {
        drawGameBoard(&snake);
        char input = getInput();
        moveSnake(&snake, input);
        checkCollisions(&snake);


        long current_delay = BASE_DELAY - (snake.length - 1) * SPEED_STEP;
        if (current_delay < MIN_DELAY) current_delay = MIN_DELAY;
        
        struct timespec delay = {0, current_delay};
        nanosleep(&delay, NULL);
    }

    printf("Game Over! Final Score: %d\n", snake.length * 100);
    return 0;
}