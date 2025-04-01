#define _XOPEN_SOURCE 700  
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>      
#include "game.h"         


void freeSnake(Snake *snake) {
    SnakeSegment *current = snake->head;
    while (current) {
        SnakeSegment *tmp = current;
        current = current->next;
        free(tmp);
    }
}

int main() {

    srand(time(NULL));

    Snake snake;
    setup_snake(&snake);
    drawGameBoard(&snake);
    setup_food();
    drawGameBoard(&snake);

    while (!isGameOver) {
        char input = getInput();
        moveSnake(&snake, input);
        checkCollisions(&snake);
        
        long current_delay = BASE_DELAY - (snake.length - 1) * SPEED_STEP;
        if (current_delay < MIN_DELAY)
            current_delay = MIN_DELAY;
        
        struct timespec delay = {0, current_delay};
        nanosleep(&delay, NULL);
        drawGameBoard(&snake);
    }

    if (isGameWon) {
        printf("Congratulations! You have won. Final Score: %d\n", snake.length * 100 - 100);
    } else {
        printf("Game Over! Final Score: %d\n", snake.length * 100 - 100);
    }
    
    int highscore = loadHighScore();
    saveScore(highscore, &snake);

    freeSnake(&snake); 

    return 0;
}
