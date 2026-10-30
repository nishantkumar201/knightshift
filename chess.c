#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

typedef struct GameState GameState;
typedef struct MoveStructure MoveStructure;

void init_board(GameState *game);
void print_board(GameState *game);
void user_query(GameState *game, MoveStructure *move);
bool is_move_valid(const GameState *game, const MoveStructure *move);
void update_game_state (GameState *game, const MoveStructure *move);
bool is_path_clear(const GameState *game, const MoveStructure *move);
bool is_valid_knight(const GameState *game, const MoveStructure *move);
bool is_valid_bishop(const GameState *game, const MoveStructure *move);
bool is_valid_rook(const GameState *game, const MoveStructure *move);
bool is_valid_queen(const GameState *game, const MoveStructure *move);
bool is_valid_pawn(const GameState *game, const MoveStructure *move);
bool is_valid_king(const GameState *game, const MoveStructure *move);
bool is_king_in_check(const GameState *game, bool white_king);
bool is_move_king_safe(const GameState *game, const MoveStructure *move);
bool is_square_attacked(const GameState *game, int row, int col, bool by_white);
void test_is_king_in_check();

struct GameState {
    char board[8][8];
    bool white_turn;

    int white_king_row;
    int white_king_col;
    int black_king_row;
    int black_king_col;
};

struct MoveStructure{
    int from_row;
    int from_col;
    int to_row;
    int to_col;
};

int main() {
    GameState gameState;
    MoveStructure moveStructure;
    init_board(&gameState);

    for (int i = 0; i < 10; i++) {
    user_query(&gameState, &moveStructure);

    if (is_move_valid(&gameState, &moveStructure)) {
        update_game_state(&gameState, &moveStructure);
        print_board(&gameState);

        if (is_king_in_check(&gameState, gameState.white_turn)) {
            printf("Check!\n");
        }

    } else {
        printf("Invalid Move... Try Again\n");
        i--;
    }
    return 0;
    }
}

void init_board(GameState *game) {
    memset(game->board, '_', sizeof(game->board));

    char black_back_row[] = "RNBQKBNR";
    memcpy(game->board[0], black_back_row, 8);
    memset(game->board[1], 'P', 8);

    char white_back_row[] = "rnbqkbnr";
    memset(game->board[6], 'p', 8);
    memcpy(game->board[7], white_back_row, 8);

    game->white_king_row = 7;
    game->white_king_col = 4;

    game->black_king_row = 0;
    game->black_king_col = 4;

    game->white_turn = true;
}

void print_board(GameState *game) {
    printf("     a   b   c   d   e   f   g   h\n");
    printf("   +--------------------------------+\n");

    for (int i = 0; i < 8; i++) {
        printf("%d  |", 8 - i);
        for (int j = 0; j < 8; j++) {
            if ((i % 2 == 0 && j % 2 == 0) ||
                (i % 2 == 1 && j % 2 == 1)) {
                printf("\033[48;5;230m %c  \033[m", game->board[i][j]);
            } else {
                printf(" %c  ", game->board[i][j]);
            }
        }
        printf("|\n");
    }

    printf("   +--------------------------------+\n");
}

void user_query(GameState *game, MoveStructure *move){
    char from[3], to[3];
    int invalid = 0;
    do {
        if (invalid) printf("Invalid move. Try again.\n");
        printf("From: ");
        scanf("%2s", from);
        invalid = 1;
    } while (strlen(from) != 2 || from[0] < 'a' || from[0] > 'h' || from[1] < '1' || from[1] > '8');

    invalid = 0;
    do {
        if (invalid) printf("Invalid move. Try again.\n");
        printf("To: ");
        scanf("%2s", to);
        invalid = 1;
    } while (strlen(to) != 2 || to[0] < 'a' || to[0] > 'h' || to[1] < '1' || to[1] > '8');
    
    move->from_row = '8' - from[1];
    move->from_col = from[0] - 'a';
    move->to_row = '8' - to[1];
    move->to_col = to[0] - 'a';
}

void update_game_state(GameState *game, const MoveStructure *move){
    game->board[move->to_row][move->to_col] = game->board[move->from_row][move->from_col];
    game->board[move->from_row][move->from_col] = '_';
    game->white_turn = !game->white_turn;

    if (game->board[move->to_row][move->to_col] == 'k'){
        game->white_king_row = move->to_row;
        game->white_king_col = move->to_col;
    }
    if (game->board[move->to_row][move->to_col] == 'K'){
        game->black_king_row = move->to_row;
        game->black_king_col = move->to_col;
    }
}

bool is_move_valid(const GameState *game, const MoveStructure *move) {
    bool valid_move = false;
    char from_piece = game->board[move->from_row][move->from_col];
    char to_piece = game->board[move->to_row][move->to_col];

    if (from_piece == '_') {
        return valid_move; // No piece to move
    }

    if (move->from_row == move->to_row && move->from_col == move->to_col) {
        return valid_move; // Cannot move to the same square
    }

    if (islower(from_piece) && game->white_turn == false 
    || isupper(from_piece) && game->white_turn == true){
        printf("Wrong person's turn\n");
        return valid_move;
    }

    if ((isupper(from_piece) && isupper(to_piece)) ||
        (islower(from_piece) && islower(to_piece))) {
        printf("Same color from_piece... Invalid\n");
        return valid_move;
    }

    if (to_piece == 'k' || to_piece == 'K'){
        printf("Cannot capture king\n");
        return valid_move;
    }

    if (from_piece == 'n' || from_piece == 'N'){
        valid_move = is_valid_knight(game, move);
    }
    if (from_piece == 'b' || from_piece == 'B') {
        if(is_valid_bishop(game, move)){
            valid_move = is_path_clear(game, move);
        }
    }
    if (from_piece == 'r' || from_piece == 'R') {
        if(is_valid_rook(game, move)){
            valid_move = is_path_clear(game, move);
        }
    }
    if (from_piece == 'q' || from_piece == 'Q') {
        if(is_valid_queen(game, move)){
            valid_move = is_path_clear(game, move);
        }
    }
    if (from_piece == 'p' || from_piece == 'P') {
        valid_move = is_valid_pawn(game, move);
    }
    if (from_piece == 'k' || from_piece == 'K') {
        valid_move = is_valid_king(game, move);
    }
    
    if (!valid_move){
        return false;
    }

    if (is_move_king_safe(game, move) == false){
        printf("Move puts king in check\n");
        valid_move = false;
    }

    return valid_move;
}

bool is_path_clear(const GameState *game, const MoveStructure *move){
    int from_col = move->from_col;
    int from_row = move->from_row;
    int to_col = move->to_col;
    int to_row = move->to_row;
    int stepX = from_col < to_col ? 1: from_col > to_col ? -1 : 0;
    int stepY = from_row < to_row ? 1: from_row > to_row ? -1 : 0;

    int current_pos_x = from_col + stepX;
    int current_pos_y = from_row + stepY;

    while (current_pos_x != to_col || current_pos_y != to_row){
        if (game->board[current_pos_y][current_pos_x] != '_'){
            return false;
        }
        current_pos_x += stepX;
        current_pos_y += stepY;
    }
    return true;
}

bool is_valid_knight(const GameState *game, const MoveStructure *move){
    if (!(abs(move->from_row - move->to_row) == 2 && abs(move->from_col - move->to_col) == 1 
    || abs(move->from_row - move->to_row) == 1 && abs(move->from_col - move->to_col) == 2)){
        return false;
    }
    return true;
}

bool is_valid_bishop(const GameState *game, const MoveStructure *move){
    if (!(abs(move->from_row - move->to_row) == abs(move->from_col - move->to_col))){
        return false;
    }
    return true;
}

bool is_valid_rook(const GameState *game, const MoveStructure *move){
    if (!(move->from_row == move->to_row || move->from_col == move->to_col)){
        return false;
    }
    return true;
}

bool is_valid_queen(const GameState *game, const MoveStructure *move){
    if (!(abs(move->from_row - move->to_row) == abs(move->from_col - move->to_col) 
    || move->from_row == move->to_row || move->from_col == move->to_col)){
        return false;
    }
    return true;
}

bool is_valid_pawn(const GameState *game, const MoveStructure *move){
    if (game->board[move->from_row][move->from_col] == 'P'){
        int one_space_movement = move->to_row - move->from_row == 1 && move->from_col == move->to_col 
        && game->board[move->to_row][move->to_col] == '_';
        int two_space_movement = move->from_row == 1 && move->to_row - move->from_row == 2 
        && move->from_col == move->to_col && game->board[move->to_row][move->to_col] == '_' 
        && game->board[move->from_row+1][move->to_col] == '_';
        int diagonal_movement = move->to_row - move->from_row == 1 && (move->from_col - move->to_col == 1 
            || move->from_col - move->to_col == -1) && islower(game->board[move->to_row][move->to_col]);

        if (!(one_space_movement || two_space_movement || diagonal_movement)){
            return false;
        }
    }

    if (game->board[move->from_row][move->from_col] == 'p'){
        int one_space_movement = move->from_row - move->to_row == 1 && move->from_col == move->to_col 
        && game->board[move->to_row][move->to_col] == '_';
        int two_space_movement = move->from_row == 6 &&  move->from_row - move->to_row == 2 
        && move->from_col == move->to_col && game->board[move->to_row][move->to_col] == '_' 
        && game->board[move->from_row-1][move->to_col] == '_';
        int diagonal_movement = move->from_row - move->to_row == 1 && (move->from_col - move->to_col == 1 
            || move->from_col - move->to_col == -1) && isupper(game->board[move->to_row][move->to_col]);

        if (!(one_space_movement || two_space_movement || diagonal_movement)){
            return false;
        }
    }
    // // En passant needs to be implemented
    return true;
}

bool is_valid_king(const GameState *game, const MoveStructure *move){
    if (abs(move->from_row - move->to_row) > 1 || abs(move->from_col - move->to_col) > 1){
        return false;
    }

    return true;
}

bool is_king_in_check(const GameState *game, bool white_king){
    if (white_king){
        return is_square_attacked(game, game->white_king_row, game->white_king_col, false);
    } else{
        return is_square_attacked(game, game->black_king_row, game->black_king_col, true);
    }
}

bool is_move_king_safe(const GameState *game, const MoveStructure *move){
    GameState simulated_game = *game;
    update_game_state(&simulated_game, move);

    return !is_king_in_check(&simulated_game, game->white_turn);
}

bool is_square_attacked(const GameState *game, int row, int col, bool by_white){
    int diagonal_row[4] = {-1, -1, 1, 1};
    int diagonal_col[4] = {1, -1, -1, 1};

    for (int i=0; i<4; i++){
        int current_row = row + diagonal_row[i];
        int current_col = col + diagonal_col[i];

        while (current_row >= 0 && current_row < 8 && current_col >= 0 && current_col < 8){
            char piece = game->board[current_row][current_col];

            if (by_white && (i== 2 || i == 3) && current_row == row +1 && piece == 'p'){
                return true;
            }

            if (!by_white && (i== 0 || i == 1) && current_row == row - 1 && piece == 'P'){
                return true;
            }

            if (by_white && abs(current_row - row) == 1 && piece == 'k'){
                return true;
            }

            if (!by_white && abs(current_row - row) == 1 && piece == 'K'){
                return true;
            }

            if (piece != '_'){
                if (by_white && (piece == 'b' || piece == 'q')){
                    return true;
                }
                if (!by_white && (piece == 'B' || piece == 'Q')){
                    return true;
                }
                break;
            }
            current_row += diagonal_row[i];
            current_col += diagonal_col[i];
        }
    }

    int straight_row[4]= {1, -1, 0, 0};
    int straight_col[4]= {0, 0, 1, -1};

    for (int i = 0; i<4; i++){
        int current_row = row + straight_row[i];
        int current_col = col + straight_col[i];
        
        while (current_row >= 0 && current_row <8 && current_col >= 0 && current_col < 8){
            char piece = game->board[current_row][current_col];

            if (by_white && (abs(current_row - row) == 1 || abs(current_col - col) == 1) && piece == 'k'){
                return true;
            }
            
            if (!by_white && (abs(current_row - row) == 1 || abs(current_col - col) == 1) && piece == 'K'){
                return true;
            }

            if (piece != '_'){
                if (by_white && (piece == 'r' || piece == 'q')){
                    return true;
                }
                if (!by_white && (piece == 'R' || piece == 'Q')){
                    return true;
                }
                break;
            };

            current_row += straight_row[i];
            current_col += straight_col[i];
        };
    };

    int knight_row[8] = {-2, -1, 1, 2, 1, 2, -1, -2};
    int knight_col[8] = {-1, -2, -2, -1, 2, 1, 2, 1};

    for (int i = 0; i<8; i++){
        int current_row = row + knight_row[i];
        int current_col = col + knight_col[i];
        
        if (current_row >= 0 && current_row < 8 && current_col >= 0 && current_col < 8){
            char piece = game->board[current_row][current_col];
            if (by_white && piece == 'n'){
                return true;
            }
            if (!by_white && piece == 'N'){
                return true;
            }
        }
    }
    return false;
}