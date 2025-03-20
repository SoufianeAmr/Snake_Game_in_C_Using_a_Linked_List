#define _XOPEN_SOURCE 700
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

        
        usleep(100000);
    }

    printf("Game Over! Score final: %d\n", snake.length * 100);
    return 0;
}
