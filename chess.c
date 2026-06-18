#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char board[8][8];

void init_board();
void print_board();
void user_query();
int is_valid_move(int from_row, int from_col, int to_row, int to_col);
int is_valid_knight(int from_row, int from_col, int to_row, int to_col);
int is_valid_bishop(int from_row, int from_col, int to_row, int to_col);
int is_valid_rook(int from_row, int from_col, int to_row, int to_col);

int main(){
    init_board();
    print_board();
    for (int i = 0; i < 10; i++){
        user_query();}
    return 0;
}

void init_board(){
    memset(board, '_', sizeof(board));

    char black_back_row[] = "RNBQKBNR";
    memcpy(board[0], black_back_row, 8);
    memset(board[1],'P', 8);

    char white_back_row[] = "rnbqkbnr";
    memset(board[6], 'p', 8);
    memcpy(board[7], white_back_row, 8);
}

void print_board() {
    int rows = sizeof(board) / sizeof(board[0]);
    int columns = sizeof(board[0]) / sizeof(board[0][0]);

    printf("     a   b   c   d   e   f   g   h\n");
    printf("   +--------------------------------+\n");

    for (int i = 0; i < rows; i++) {
        printf("%d  |", 8-i);
        for (int j = 0; j < columns; j++) {
            if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                printf("\033[48;5;230m %c  \033[m", board[i][j]);
            } else {
                printf(" %c  ", board[i][j]);
            }
        }
        printf("|\n");
    }
    printf("   +--------------------------------+\n");
}

void user_query (){
    char from[3], to[3];
    int invalid = 0;
    int from_col, from_row, to_col, to_row;
    do {
        do {
            if (invalid) printf("Invalid move. Try again.\n");
            printf("From: ");
            scanf("%s", from);
            invalid = 1;
        } while (strlen(from) != 2 || from[0] < 'a' || from[0] > 'h' || from[1] < '1' || from[1] > '8' || board['8' - from[1]][from[0] - 'a'] == '_');

        invalid = 0;
        do {
            if (invalid) printf("Invalid move. Try again.\n");
            printf("To: ");
            scanf("%s", to);
            invalid = 1;
        } while (strlen(to) != 2 || to[0] < 'a' || to[0] > 'h' || to[1] < '1' || to[1] > '8');
        
        from_col = from[0] - 'a';
        from_row = '8' - from[1];
        to_col = to[0] - 'a';
        to_row = '8' - to[1];

    } while (!is_valid_move(from_row, from_col, to_row, to_col));
    board[to_row][to_col] = board[from_row][from_col];
    board[from_row][from_col] = '_';

    print_board();
    }

int is_valid_move(int from_row, int from_col, int to_row, int to_col) {
    char piece = board[from_row][from_col];
    if (piece == 'n' || piece == 'N') {
        return is_valid_knight(from_row, from_col, to_row, to_col);
    }
    if (piece == 'b' || piece == 'B') {
        return is_valid_bishop(from_row, from_col, to_row, to_col);
    }
    if (piece == 'r' || piece == 'R') {
        return is_valid_rook(from_row, from_col, to_row, to_col);
    }
    return 1;
}

int is_valid_knight(int from_row, int from_col, int to_row, int to_col){
    if (!(abs(from_row - to_row) == 2 && abs(from_col - to_col) == 1 || abs(from_row - to_row) == 1 && abs(from_col - to_col) == 2)){
        return 0;
    }
    if (isupper(board[to_row][to_col]) && isupper(board[from_row][from_col]) || islower(board[to_row][to_col]) && islower(board[from_row][from_col])){
        printf("Same color piece\n");
        return 0;
    }
    return 1;
}

int is_valid_bishop(int from_row, int from_col, int to_row, int to_col){
    if (!(abs(from_row - to_row) == abs(from_col - to_col))){
        return 0;
    }

    if (isupper(board[to_row][to_col]) && isupper(board[from_row][from_col]) || islower(board[to_row][to_col]) && islower(board[from_row][from_col])){
        printf("Same color piece\n");
        return 0;
    }

    int stepX = from_col < to_col ? 1: from_col > to_col ? -1 : 0;
    int stepY = from_row < to_row ? 1: from_row > to_row ? -1 : 0;

    int current_pos_x = from_col + stepX;
    int current_pos_y = from_row + stepY;

    while (current_pos_x != to_col && current_pos_y != to_row){
        if (board[current_pos_y][current_pos_x] != '_'){
            return 0;
        }
        current_pos_x += stepX;
        current_pos_y += stepY;
    }

    return 1;
}

int is_valid_rook(int from_row, int from_col, int to_row, int to_col){
    if (!(from_row == to_row || from_col == to_col)){
        return 0;
    }

    if (isupper(board[to_row][to_col]) && isupper(board[from_row][from_col]) || islower(board[to_row][to_col]) && islower(board[from_row][from_col])){
        printf("Same color piece\n");
        return 0;
    }

    int stepX = from_col < to_col ? 1: from_col > to_col ? -1 : 0;
    int stepY = from_row < to_row ? 1: from_row > to_row ? -1 : 0;

    int current_pos_x = from_col + stepX;
    int current_pos_y = from_row + stepY;

    while (current_pos_x != to_col && current_pos_y != to_row){
        if (board[current_pos_y][current_pos_x] != '_'){
            return 0;
        }
        current_pos_x += stepX;
        current_pos_y += stepY;
    }

    return 1;
}