#define _XOPEN_SOURCE 700
#include "game.h"


Cell gameBoard[HEIGHT][WIDTH];
Food food[FOODS];
int isGameOver = 0;


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
        return; // Prevent drawing out-of-bounds head
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
    int ch = getch();

    
    if (ch == 27) {
        int next1 = getch(); 
        if (next1 == 91) {
            int next2 = getch(); 
            switch (next2) {
                case 65: return 'W'; 
                case 66: return 'S'; 
                case 67: return 'D'; 
                case 68: return 'A'; 
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
