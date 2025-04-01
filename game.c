#define _XOPEN_SOURCE 700
#include <fcntl.h>
#include "game.h"

// Defines the grid 
Cell gameBoard[HEIGHT][WIDTH];
Food food;
int isGameOver = 0;
int isGameWon = 0;

int getch(void) {
    struct termios oldt, newt;
    unsigned char ch;
    int n;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    n = read(STDIN_FILENO, &ch, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return (n <= 0) ? -1 : ch;
}

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

void draw_snake(const Snake *snake) {
    SnakeSegment *current = snake->head;
    int isHead = 1;
    while (current != NULL) {
        if (current->x >= 0 && current->x < WIDTH &&
            current->y >= 0 && current->y < HEIGHT) {
            if (isHead) {
                gameBoard[current->y][current->x].type = 'S';
                isHead = 0;
            } else {
                gameBoard[current->y][current->x].type = 'o';
            }
        }
        current = current->next;
    }
}

void clear_screen() {
    system("clear");
}

void print_board(const Snake *snake) {
    
    printf("Current Score : %d\n", snake->length * 100 - 100);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(gameBoard[y][x].type);
        }
        putchar('\n');
    }
}

void drawGameBoard(const Snake *snake) {
    fill_board();
    draw_snake(snake);
    draw_food();
    clear_screen();
    print_board(snake);
}

char getInput() {
    static int escape = 0;
    static int bracket = 0;
    int ch;

    while ((ch = getch()) != -1) {
        if (escape) {
            if (ch == '[') {
                bracket = 1;
                escape = 0;
            } else {
                escape = 0;
            }
        } else if (bracket) {
            bracket = 0;
            switch (ch) {
                case 'A': return 'W';
                case 'B': return 'S';
                case 'C': return 'D';
                case 'D': return 'A';
                default: break;
            }
        } else if (ch == 27) {
            escape = 1;
        } else {
            switch (ch) {
                case 'w': return 'W';
                case 'a': return 'A';
                case 's': return 'S';
                case 'd': return 'D';
                default: break;
            }
        }
    }

    return '\0';
}


void moveSnake(Snake *snake, char input) {
    if (input == 'W' || input == 'A' || input == 'S' || input == 'D') {
        
        if (snake->head && snake->head->next) {
            if ((snake->direction == 'W' && input == 'S') ||
                (snake->direction == 'S' && input == 'W') ||
                (snake->direction == 'A' && input == 'D') ||
                (snake->direction == 'D' && input == 'A')) {
                isGameOver = 1;
                return;
            }
        }
        snake->direction = input;
    }
    
    
    SnakeSegment *newHead = malloc(sizeof(SnakeSegment));
    if (!newHead) {
        perror("Failed to allocate memory for new snake segment");
        exit(EXIT_FAILURE);
    }
    newHead->x = snake->head->x;
    newHead->y = snake->head->y;
    switch (snake->direction) {
        case 'W': newHead->y--; break;
        case 'S': newHead->y++; break;
        case 'A': newHead->x--; break;
        case 'D': newHead->x++; break;
    }
    newHead->next = snake->head;
    snake->head = newHead;
    snake->length++;
}


void checkCollisions(Snake *snake) {
    if (snake->length == MAX_SNAKE_LENGTH) {
        isGameWon = 1;
        isGameOver = 1;
        return;
    }


    if (!food.consumed &&
        snake->head->x == food.x &&
        snake->head->y == food.y) {
        food.consumed = 1;
       
    } else {
        
        if (snake->length > 1) {
            SnakeSegment *current = snake->head;
           
            while (current->next && current->next->next != NULL) {
                current = current->next;
            }
            if (current->next) {
                free(current->next);
                current->next = NULL;
                snake->length--;
            }
        }
    }

    if (snake->head->x <= 0 || snake->head->x >= WIDTH - 1 ||
        snake->head->y <= 0 || snake->head->y >= HEIGHT - 1) {
        isGameOver = 1;
        return;
    }
    

    SnakeSegment *current = snake->head->next;
    while (current) {
        if (snake->head->x == current->x && snake->head->y == current->y) {
            isGameOver = 1;
            break;
        }
        current = current->next;
    }
}

static int isCellTaken(int x, int y) {
    if (gameBoard[y][x].type != ' ') {
        return 1;
    }
    return 0;
}

void draw_food() {
    if (food.consumed) {
        int fx, fy;
        do {
            fx = 1 + rand() % (WIDTH - 2);
            fy = 1 + rand() % (HEIGHT - 2);
        } while (isCellTaken(fx, fy));
        food.x = fx;
        food.y = fy;
        food.consumed = 0;
    }
    gameBoard[food.y][food.x].type = 'F';
}

void setup_food() {
    food.consumed = 1;
    draw_food();
}


void setup_snake(Snake *snake) {
    snake->head = malloc(sizeof(SnakeSegment));
    if (!snake->head) {
        perror("Failed to allocate memory for snake head");
        exit(EXIT_FAILURE);
    }
    snake->head->x = WIDTH / 2;
    snake->head->y = HEIGHT / 2;
    snake->head->next = NULL;
    snake->length = 1;
    snake->direction = 'D'; 
}

int loadHighScore() {
    FILE *fin = fopen("highscore.txt", "r");
    if (fin == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }
    int highscore;
    fscanf(fin, "%d", &highscore);
    fclose(fin);
    return highscore;
}

void saveScore(int highscore, Snake *snake) {
    FILE *fin = fopen("highscore.txt", "w");
    if (highscore > (snake->length * 100 - 100)) {
        printf("Highscore: %d\n", highscore);
    } else {
        highscore = snake->length * 100 - 100;
        fprintf(fin, "%d", highscore);
        printf("New highscore of %d!\n", highscore);
    }
    fclose(fin);
}
