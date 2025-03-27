#define _XOPEN_SOURCE 700
#include <fcntl.h>  
#include "game.h"

Cell gameBoard[HEIGHT][WIDTH];
Food food[FOODS];
int isGameOver = 0;

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

void draw_food() {
    for (int i = 0; i < FOODS; i++) {
        if (!food[i].consumed) {
            gameBoard[food[i].y][food[i].x].type = 'F';
        }
    }
}

void draw_snake(const Snake *snake) {
    if (snake->bodyX[0] < 0 || snake->bodyX[0] >= WIDTH ||
        snake->bodyY[0] < 0 || snake->bodyY[0] >= HEIGHT) {
        return; 
    }
    gameBoard[snake->bodyY[0]][snake->bodyX[0]].type = 'S';
    
    for (int i = 1; i < snake->length; i++) {
        if (snake->bodyX[i] < 0 || snake->bodyX[i] >= WIDTH ||
            snake->bodyY[i] < 0 || snake->bodyY[i] >= HEIGHT) {
            continue; // Skip invalid body segments
        }
        gameBoard[snake->bodyY[i]][snake->bodyX[i]].type = 'o';
    }
}

void clear_screen() {
    
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
       
        if ((snake->direction == 'W' && input == 'S') ||
            (snake->direction == 'S' && input == 'W') ||
            (snake->direction == 'A' && input == 'D') ||
            (snake->direction == 'D' && input == 'A')) {
           
        } else {
            snake->direction = input;
        }
    }

    
    for (int i = snake->length - 1; i > 0; i--) {
        snake->bodyX[i] = snake->bodyX[i - 1];
        snake->bodyY[i] = snake->bodyY[i - 1];
    }

    
    switch (snake->direction) {
        case 'W': snake->bodyY[0]--; break;
        case 'S': snake->bodyY[0]++; break;
        case 'A': snake->bodyX[0]--; break;
        case 'D': snake->bodyX[0]++; break;
    }
}

void checkCollisions(Snake *snake) {
    
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

   
    if (snake->bodyX[0] <= 0 || snake->bodyX[0] >= WIDTH - 1 ||
        snake->bodyY[0] <= 0 || snake->bodyY[0] >= HEIGHT - 1) {
        isGameOver = 1;
    }

    for (int i = 1; i < snake->length; i++) {
        if (snake->bodyX[0] == snake->bodyX[i] &&
            snake->bodyY[0] == snake->bodyY[i]) {
            isGameOver = 1;
            break;
        }
    }
}

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
    snake->direction = 'D'; 
}
