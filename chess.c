char board[8][8];

char* init_board(){
    memset(board, 0, sizeof(board));

    char black_back_row[] = "RBHQKHBR";
    memcpy(board[0], black_back_row, 8);
    memset(board[1],'P', 8);

    char white_back_row[] = "rbhkqhbr";
    memset(board[6], 'P', 8);
    memcpy(board[7], white_back_row, 8);

    return board;
}

// void printArray (int *arr, int size){

// }

void main(){
    init_board();
}