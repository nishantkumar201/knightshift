#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void init_board(char board[][8]);
void print_board(char board[][8]);
void user_query(char board[][8]);
int is_valid_move(char board[][8], int from_row, int from_col, int to_row, int to_col);
int is_valid_knight(char board[][8], int from_row, int from_col, int to_row, int to_col);
int is_valid_bishop(char board[][8], int from_row, int from_col, int to_row, int to_col);
int is_valid_rook(char board[][8], int from_row, int from_col, int to_row, int to_col);
int is_valid_queen(char board[][8], int from_row, int from_col, int to_row, int to_col);
int is_valid_pawn(char board[][8], int from_row, int from_col, int to_row, int to_col);
int is_valid_king(char board[][8], int from_row, int from_col, int to_row, int to_col);
int is_valid(char board[][8], int from_row, int from_col, int to_row, int to_col);
int is_obstacles_in_way(char board[][8], int from_row, int from_col, int to_row, int to_col);
void promotion(char board[][8], int row, int col);
void update_castled_flags(char board[][8], int from_row, int from_col);

int white_king_move = 0;
int black_king_move = 0;
int white_kingside_rook_move = 0;
int white_queenside_rook_move = 0;
int black_kingside_rook_move = 0;
int black_queenside_rook_move = 0;
int castled = 0;
    

int main(){
    char board[8][8];
    init_board(board);
    print_board(board);
    for (int i = 0; i < 10; i++){
        user_query(board);}
    return 0;
}

void init_board(char board[][8]){
    memset(board, '_', sizeof(board[0][0]) * 8 * 8);

    char black_back_row[] = "RNBQKBNR";
    memcpy(board[0], black_back_row, 8);
    memset(board[1],'P', 8);

    char white_back_row[] = "rnbqkbnr";
    memset(board[6], 'p', 8);
    memcpy(board[7], white_back_row, 8);
}

void print_board(char board[][8]) {
    printf("     a   b   c   d   e   f   g   h\n");
    printf("   +--------------------------------+\n");
    for (int i = 0; i < 8; i++) {
        printf("%d  |", 8-i);
        for (int j = 0; j < 8; j++) {
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

void user_query (char board[][8]){
    char from[3], to[3];
    int invalid = 0;
    int from_col, from_row, to_col, to_row;
    do {
        do {
            if (invalid) printf("Invalid move. Try again.\n");
            printf("From: ");
            scanf("%2s", from);
            invalid = 1;
        } while (strlen(from) != 2 || from[0] < 'a' || from[0] > 'h' || from[1] < '1' || from[1] > '8' || board['8' - from[1]][from[0] - 'a'] == '_');

        invalid = 0;
        do {
            if (invalid) printf("Invalid move. Try again.\n");
            printf("To: ");
            scanf("%2s", to);
            invalid = 1;
        } while (strlen(to) != 2 || to[0] < 'a' || to[0] > 'h' || to[1] < '1' || to[1] > '8');
        
        from_col = from[0] - 'a';
        from_row = '8' - from[1];
        to_col = to[0] - 'a';
        to_row = '8' - to[1];

    } while (!is_valid_move(board, from_row, from_col, to_row, to_col));

    update_castled_flags(board, from_row, from_col);
    if (!castled){
        board[to_row][to_col] = board[from_row][from_col];
        board[from_row][from_col] = '_';
    }
    castled = 0;
    promotion(board, to_row, to_col);

    print_board(board);
    }

int is_valid_move(char board[][8], int from_row, int from_col, int to_row, int to_col) {
    char piece = board[from_row][from_col];
    if (piece == 'n' || piece == 'N') {
        return is_valid_knight(board, from_row, from_col, to_row, to_col);
    }
    if (piece == 'b' || piece == 'B') {
        return is_valid_bishop(board, from_row, from_col, to_row, to_col);
    }
    if (piece == 'r' || piece == 'R') {
        return is_valid_rook(board, from_row, from_col, to_row, to_col);
    }
    if (piece == 'q' || piece == 'Q') {
        return is_valid_queen(board, from_row, from_col, to_row, to_col);
    }
    if (piece == 'p' || piece == 'P') {
        return is_valid_pawn(board, from_row, from_col, to_row, to_col);
    }
    if (piece == 'k' || piece == 'K') {
        return is_valid_king(board, from_row, from_col, to_row, to_col);
    }
    return 1;
}

int is_valid_knight(char board[][8], int from_row, int from_col, int to_row, int to_col){
    if (!(abs(from_row - to_row) == 2 && abs(from_col - to_col) == 1 || abs(from_row - to_row) == 1 && abs(from_col - to_col) == 2)){
        return 0;
    }

    return is_valid(board, from_row, from_col, to_row, to_col);
}

int is_valid_bishop(char board[][8], int from_row, int from_col, int to_row, int to_col){
    if (!(abs(from_row - to_row) == abs(from_col - to_col))){
        return 0;
    }
    return is_obstacles_in_way(board, from_row, from_col, to_row, to_col);
}

int is_valid_rook(char board[][8], int from_row, int from_col, int to_row, int to_col){
    if (!(from_row == to_row || from_col == to_col)){
        return 0;
    }
    return is_obstacles_in_way(board, from_row, from_col, to_row, to_col);
}

int is_valid_queen(char board[][8], int from_row, int from_col, int to_row, int to_col){
    if (!(abs(from_row - to_row) == abs(from_col - to_col) || from_row == to_row || from_col == to_col)){
        return 0;
    }
    return is_obstacles_in_way(board, from_row, from_col, to_row, to_col);
}

int is_valid_pawn(char board[][8], int from_row, int from_col, int to_row, int to_col){
    if (!is_valid(board, from_row, from_col, to_row, to_col)){
        return 0;
    }

    if (board[from_row][from_col] == 'P'){
        int one_space_movement = to_row - from_row == 1 && from_col == to_col && board[to_row][to_col] == '_';
        int two_space_movement = from_row == 1 && to_row - from_row == 2 && from_col == to_col && board[to_row][to_col] == '_' && board[from_row+1][to_col] == '_';
        int diagonal_movement = to_row - from_row == 1 && (from_col - to_col == 1 || from_col - to_col == -1) && islower(board[to_row][to_col]);

        if (!(one_space_movement || two_space_movement || diagonal_movement)){
            return 0;
        }

    }

    if (board[from_row][from_col] == 'p'){
        int one_space_movement = from_row - to_row == 1 && from_col == to_col && board[to_row][to_col] == '_';
        int two_space_movement = from_row == 6 && from_row - to_row == 2 && from_col == to_col && board[to_row][to_col] == '_' && board[from_row-1][to_col] == '_';
        int diagonal_movement = from_row - to_row == 1 && (from_col - to_col == 1 || from_col - to_col == -1) && isupper(board[to_row][to_col]);

        if (!(one_space_movement || two_space_movement || diagonal_movement)){
            return 0;
        }
    }
    // // En passant needs to be implemented
    return 1;
}

int is_valid_king(char board[][8], int from_row, int from_col, int to_row, int to_col){
    if (board[from_row][from_col] == 'K'){
            if (from_row == to_row && (from_col - to_col == 2) && board[to_row][0] == 'R' && !black_king_move && !black_queenside_rook_move){
                if (board[to_row][1] != '_' || board[to_row][2] != '_' || board[to_row][3] != '_') return 0;
                black_king_move = 1;
                board[to_row][to_col] = 'K';
                board[to_row][to_col+1] = 'R';
                board[from_row][from_col] = '_';
                board[to_row][0] = '_';
                castled = 1;
                return 1;
            }
            if (from_row == to_row && (to_col - from_col == 2) && board[to_row][7] == 'R' && !black_king_move && !black_kingside_rook_move){
                if (board[to_row][6] != '_' || board[to_row][5] != '_') return 0;
                black_king_move = 1;
                board[to_row][to_col] = 'K';
                board[to_row][to_col-1] = 'R';
                board[from_row][from_col] = '_';
                board[to_row][7] = '_';
                castled = 1;
                return 1;
            }
    }

    if (board[from_row][from_col] == 'k'){
            if (from_row == to_row && (from_col - to_col == 2) && board[to_row][0] == 'r' && !white_king_move && !white_queenside_rook_move){
                if (board[to_row][1] != '_' || board[to_row][2] != '_' || board[to_row][3] != '_') return 0;
                white_king_move = 1;
                board[to_row][to_col] = 'k';
                board[to_row][to_col+1] = 'r';
                board[from_row][from_col] = '_';
                board[to_row][0] = '_';
                castled = 1;
                return 1;
            }
            if (from_row == to_row && (to_col - from_col == 2) && board[to_row][7] == 'r' && !white_king_move && !white_kingside_rook_move){
                if (board[to_row][6] != '_' || board[to_row][5] != '_') return 0;
                white_king_move = 1;
                board[to_row][to_col] = 'k';
                board[to_row][to_col-1] = 'r';
                board[from_row][from_col] = '_';
                board[to_row][7] = '_';
                castled = 1;
                return 1;
            }
    }
    if (abs(from_row - to_row) > 1 || abs(from_col - to_col) > 1){
        return 0;
    }
    return is_obstacles_in_way(board, from_row, from_col, to_row, to_col); 
}


int is_valid(char board[][8], int from_row, int from_col, int to_row, int to_col){
    if (isupper(board[to_row][to_col]) && isupper(board[from_row][from_col]) || islower(board[to_row][to_col]) && islower(board[from_row][from_col])){
        printf("Same color piece\n");
        return 0;
    }
    return 1;
}

int is_obstacles_in_way(char board[][8], int from_row, int from_col, int to_row, int to_col){
    if(!is_valid(board, from_row, from_col, to_row, to_col)){
        return 0;
    };

    int stepX = from_col < to_col ? 1: from_col > to_col ? -1 : 0;
    int stepY = from_row < to_row ? 1: from_row > to_row ? -1 : 0;

    int current_pos_x = from_col + stepX;
    int current_pos_y = from_row + stepY;

    while (current_pos_x != to_col || current_pos_y != to_row){
        if (board[current_pos_y][current_pos_x] != '_'){
            return 0;
        }
        current_pos_x += stepX;
        current_pos_y += stepY;
    }
    return 1;
}

void promotion(char board[][8], int row, int col){
    if (board[row][col] == 'P' && row == 7){
        printf("Promote\n");
        printf("To:");
        char to[2];
        scanf("%1s", to);
        char piece = toupper(to[0]);
        if (piece == 'Q' || piece == 'R' || piece == 'B' || piece == 'N'){
            board[row][col] = piece;
        }
    }
    if (board[row][col] == 'p' && row == 0){
        printf("Promote\n");
        printf("To:");
        char to[2];
        scanf("%1s", to);
        char piece = tolower(to[0]);
        if (piece == 'q' || piece == 'r' || piece == 'b' || piece == 'n'){
            board[row][col] = piece;
        }
    }
}

void update_castled_flags(char board[][8], int from_row, int from_col){
    if (board[from_row][from_col] == 'K') black_king_move = 1;
    if (board[from_row][from_col] == 'k') white_king_move = 1;
    if (board[from_row][from_col] == 'R' && from_col == 7) black_kingside_rook_move = 1;
    if (board[from_row][from_col] == 'R' && from_col == 0) black_queenside_rook_move = 1;
    if (board[from_row][from_col] == 'r' && from_col == 7) white_kingside_rook_move = 1;
    if (board[from_row][from_col] == 'r' && from_col == 0) white_queenside_rook_move = 1;
}

int is_in_check(char board[][8], int row, int col, int color){
    return 0;
}