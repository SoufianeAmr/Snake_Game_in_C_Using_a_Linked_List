#define _XOPEN_SOURCE 700  // Must be defined before any includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>        // Add this to declare usleep()
#include "game.h"          // Include your custom header after system headers

// Replace usleep(100000) with:

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
        struct timespec delay = {0, 100000000};  // 0 seconds, 100,000,000 nanoseconds
        nanosleep(&delay, NULL);

    }

    printf("Game Over! Final Score: %d\n", snake.length * 100);
    return 0;
}
