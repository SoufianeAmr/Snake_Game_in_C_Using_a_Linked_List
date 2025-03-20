#define _XOPEN_SOURCE 700
#include "game.h"

int main() {
    srand(time(NULL));

    Snake snake;
    setup_snake(&snake);
    setup_food();

    while (!isGameOver) {
        // 1) Dessiner
        drawGameBoard(&snake);

        // 2) Lire l'entrée
        char input = getInput();

        // 3) Déplacer
        moveSnake(&snake, input);

        // 4) Vérifier collisions
        checkCollisions(&snake);

        // Petite pause (100 ms)
        usleep(100000);
    }

    printf("Game Over! Score final: %d\n", snake.length * 100);
    return 0;
}
