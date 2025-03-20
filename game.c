#define _XOPEN_SOURCE 700
#include "game.h"

// Variables globales
Cell gameBoard[HEIGHT][WIDTH];
Food food[FOODS];
int isGameOver = 0;

// getch() pour lire un caractère sans écho ni buffering
int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

/* 1) Dessin du plateau */
void fill_board() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1) {
                gameBoard[y][x].type = '#';
            } else {
                gameBoard[y][x].type = ' ';
            }
        }
    }
}

void draw_food() {
    for (int i = 0; i < FOODS; i++) {
        if (!food[i].consumed) {
            gameBoard[food[i].y][food[i].x].type = 'F';
        }
    }
}

void draw_snake(const Snake *snake) {
    // Tête
    gameBoard[snake->bodyY[0]][snake->bodyX[0]].type = 'S';
    // Corps
    for (int i = 1; i < snake->length; i++) {
        gameBoard[snake->bodyY[i]][snake->bodyX[i]].type = 'o';
    }
}

void clear_screen() {
    // Pour Windows, utiliser system("cls");
    system("clear");
}

void print_board() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(gameBoard[y][x].type);
        }
        putchar('\n');
    }
}

void drawGameBoard(const Snake *snake) {
    fill_board();
    draw_food();
    draw_snake(snake);
    clear_screen();
    print_board();
}

/* 2) Récupérer la commande de l'utilisateur */
char getInput() {
    int ch = getch();

    // Flèches: 3 octets = 27, 91, XX
    if (ch == 27) {
        int next1 = getch(); // normalement 91
        if (next1 == 91) {
            int next2 = getch(); // 65=Haut,66=Bas,67=Droite,68=Gauche
            switch (next2) {
                case 65: return 'W'; // flèche haut
                case 66: return 'S'; // flèche bas
                case 67: return 'D'; // flèche droite
                case 68: return 'A'; // flèche gauche
                default: return '\0';
            }
        }
        return '\0'; 
    }

    // WASD
    switch (ch) {
        case 'w': return 'W';
        case 'a': return 'A';
        case 's': return 'S';
        case 'd': return 'D';
        default:  return '\0';
    }
}

/* 3) Déplacer le serpent */
void moveSnake(Snake *snake, char input) {
    if (input == 'W' || input == 'A' || input == 'S' || input == 'D') {
        // Empêche le demi-tour instantané
        if ((snake->direction == 'W' && input == 'S') ||
            (snake->direction == 'S' && input == 'W') ||
            (snake->direction == 'A' && input == 'D') ||
            (snake->direction == 'D' && input == 'A')) {
            // Ignorer
        } else {
            snake->direction = input;
        }
    }

    // Déplacement du corps (de la queue vers la tête)
    for (int i = snake->length - 1; i > 0; i--) {
        snake->bodyX[i] = snake->bodyX[i - 1];
        snake->bodyY[i] = snake->bodyY[i - 1];
    }

    // Déplacement de la tête
    switch (snake->direction) {
        case 'W': snake->bodyY[0]--; break;
        case 'S': snake->bodyY[0]++; break;
        case 'A': snake->bodyX[0]--; break;
        case 'D': snake->bodyX[0]++; break;
    }
}

/* 4) Vérifier les collisions */
void checkCollisions(Snake *snake) {
    // Vérifier si le serpent mange un aliment
    for (int i = 0; i < FOODS; i++) {
        if (!food[i].consumed &&
            snake->bodyX[0] == food[i].x &&
            snake->bodyY[0] == food[i].y) {
            food[i].consumed = 1;
            // Grandir
            if (snake->length < MAX_SNAKE_LENGTH) {
                snake->length++;
            }
        }
    }

    // Collisions avec les murs
    if (snake->bodyX[0] <= 0 || snake->bodyX[0] >= WIDTH - 1 ||
        snake->bodyY[0] <= 0 || snake->bodyY[0] >= HEIGHT - 1) {
        isGameOver = 1;
    }

    // Collisions avec soi-même
    for (int i = 1; i < snake->length; i++) {
        if (snake->bodyX[0] == snake->bodyX[i] &&
            snake->bodyY[0] == snake->bodyY[i]) {
            isGameOver = 1;
            break;
        }
    }
}

/* Fonctions de configuration (initialisation) */
static int isCellTakenByFood(int x, int y, int upToIndex) {
    for (int i = 0; i < upToIndex; i++) {
        if (!food[i].consumed && food[i].x == x && food[i].y == y) {
            return 1;
        }
    }
    return 0;
}

void setup_food() {
    for (int i = 0; i < FOODS; i++) {
        food[i].consumed = 0;
        int fx, fy;
        do {
            fx = 1 + rand() % (WIDTH - 2);
            fy = 1 + rand() % (HEIGHT - 2);
        } while (isCellTakenByFood(fx, fy, i));
        food[i].x = fx;
        food[i].y = fy;
    }
}

void setup_snake(Snake *snake) {
    snake->bodyX[0] = WIDTH / 2;
    snake->bodyY[0] = HEIGHT / 2;
    snake->length = 1;
    snake->direction = 'D'; // vers la droite par défaut
}
