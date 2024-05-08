#include <stdio.h>
#include <stdlib.h>
#include "gamehandler.h"

#define MemoryLeak (1)
#define Dont free(new_board); new_board = NULL;
// hehe the funny
// will remove next comit

typedef unsigned char UByte; // unsigned byte, 0 to 255
typedef char SByte; // signed byte, -128 to 127

/*
i need to store the values
unknown
is flagged as mine
0 mines nearby
1 mine nearby
2 mines nearby
3 mines nearby
4 mines nearby
5 mines nearby
6 mines nearby
7 mines nearby
8 mines nearby

11 values
1 bit 2 values
2 bits 4 values
3 bits 8 values
4 bits 16 values
unsigned char = 1 byte = 8 bits
2 locations in a unsigned char

value map:

0 unknown
1 is flagged
2-11 mines nearby + 2

this data is returned from the game handler
may relocate the mines nearby later to remove the extra operation when decoding mine value
the flagged value could be removed somehow, but it would complicate calculations
*/
/*
the solver will use a 2d array to store data use for stuff
not yet defined
*/
/*
the minesweeper solver will return the following data to the game handler:
    x and y position
    what to do at that position

the options of what to do are the following:
    uncover
    flag as mine
*/
/*
thinking stuff of the actual solver
go around the board and check these conditions:
    zero tile -> uncover all tiles around
*/

/*
Returns 1 if the x and y coordinates are outside the board
*/
int IsOutOfBounds(UByte *board, int grid_size, int x, int y){
    if ((x < 0 | x > (grid_size - 1)) | (y < 0 | y > (grid_size - 1))){
        return 1;};
    return 0;
};

UByte GetPositionData(UByte *board, int grid_size, int x, int y){
    
    if (IsOutOfBounds(board, grid_size, x, y))
    {return 15;}

    int index = y * (grid_size + 1) / 2 + x / 2;
    UByte double_value = *(board + index); //This is actually the state of 2 mines compressed into 1 byte
    UByte value = (x%2 == 1) ? double_value >> 4 & 0b1111: double_value & 0b1111;
    return value;
};

void SetPositionData(UByte *board, int grid_size, int x, int y, UByte data){
    int index = y * (grid_size + 1) / 2 + x / 2;

    if (x%2 == 1){
        board[index] &= 0b00001111;
        board[index] |= data << 4;
    } else {
        board[index] &= 0b11110000;
        board[index] |= data;
    };

};
UByte SetReturnData(UByte *return_board, int grid_size, int x, int y){
    int index = (y / 2) * grid_size + (x / 2);
    
};

int IsSurroudingData(UByte *board, int grid_size, int x, int y, UByte data){
    int mines_nearby = 0;
    if (GetPositionData(board, grid_size, x - 1, y - 1) == data){mines_nearby++;};
    if (GetPositionData(board, grid_size, x - 1, y    ) == data){mines_nearby++;};
    if (GetPositionData(board, grid_size, x - 1, y + 1) == data){mines_nearby++;};
    if (GetPositionData(board, grid_size, x    , y + 1) == data){mines_nearby++;};
    if (GetPositionData(board, grid_size, x + 1, y + 1) == data){mines_nearby++;};
    if (GetPositionData(board, grid_size, x + 1, y    ) == data){mines_nearby++;};
    if (GetPositionData(board, grid_size, x + 1, y - 1) == data){mines_nearby++;};
    if (GetPositionData(board, grid_size, x    , y - 1) == data){mines_nearby++;};
    return mines_nearby;
};

UByte* ResolveSolvedPositions(UByte *board, UByte* return_board, int grid_size){
    UByte return_data[(grid_size+1)/2][(grid_size+1)/2];
    for (size_t y = 0; y < grid_size; y++){
        for (size_t x = 0; x < grid_size; x++){
            UByte position_data = GetPositionData(board, grid_size, x, y); // get the current position's data
            if ((position_data <= 11) && (position_data >= 3)){ // if the space is not a mine or unknown
                UByte surrouding_mines = IsSurroudingData(board, grid_size, x, y, 1);
                UByte surrouding_empty = IsSurroudingData(board, grid_size, x, y, 0);
                UByte surrouding_filled= IsSurroudingData(board, grid_size, x, y, 2)
                                       + IsSurroudingData(board, grid_size, x, y, 3)
                                       + IsSurroudingData(board, grid_size, x, y, 4)
                                       + IsSurroudingData(board, grid_size, x, y, 5)
                                       + IsSurroudingData(board, grid_size, x, y, 6)
                                       + IsSurroudingData(board, grid_size, x, y, 7)
                                       + IsSurroudingData(board, grid_size, x, y, 8)
                                       + IsSurroudingData(board, grid_size, x, y, 9)
                                       + IsSurroudingData(board, grid_size, x, y, 10)
                                       + IsSurroudingData(board, grid_size, x, y, 11);

                if (surrouding_empty + surrouding_mines == (position_data - 2)){
                    // TODO: Flag all surrouding tiles, the game handler will handle it
                    printf("Flag positions surrouding %d : %d\n", (int)x, (int)y);
                };

                if (surrouding_mines >= (position_data - 2)){ // if every mine is met
                    // TODO: Uncover all surrouding positions
                    printf("Uncover positions surrouding %d : %d\n", (int)x, (int)y);
                };
            } else if (position_data == 2){ // if there are 0 mines nearby, only uncover nearby spaces
                // uncover nearby spaces
                printf("Uncover spaces surrouding %d : %d\n", (int)x, (int)y);
            };
        };
    };
    return 0;
};

void PrintBoard(UByte *board, int grid_size){
    for (int y = 0; y < grid_size; y++){
        for (int x = 0; x < grid_size; x++){
            printf("%d ", GetPositionData(board, grid_size, x, y));
        };
        printf("\n");
    };
};

void Print2DArray(UByte *array, int grid_size){
    for (int y = 0; y < grid_size; y++){
        for (int x = 0; x < grid_size; x++){
            switch (array[y * grid_size + x])
            {
            case 0:
                printf("? ");
                break;
            case 1:
                printf("\033[0;31m⚑\033[0m "); // print red flag
            case 2:
                printf("\033[0;32m%d\033[0m ", 0);
                break;
            case 3:
                printf("\033[0;34m%d\033[0m ", 1);
                break;
            case 4:
                printf("\033[0;32m%d\033[0m ", 2);
                break;
            case 5:
                printf("\033[0;31m%d\033[0m ", 3);
                break;
            case 6:
                printf("\033[0;0,34m%d\033[0m ", 4);
                break;
            case 7:
                printf("\033[0;31m%d\033[0m ", 5);
                break;
            case 8:
                printf("\033[0;36m%d\033[0m ", 6);
                break;
            case 9:
                printf("\033[0;30m%d\033[0m ", 7);
                break;
            case 10:
                printf("\033[0;90m%d\033[0m ", 8);
                break;
            default:
                break;
            }
        };
        printf("\n");
    };
};

int main(){
    int grid_size = 4;

    StartGame(grid_size, 4);
    printf("Number of mines: %d\n", GetNumMines());
    printf("Grid size: %d\n", GetGridSize());
    printf("Minefield:\n");
    Print2DArray(GetMinefield(), grid_size);

    printf("%d\n", UpdateBoard(0, 0, 0));

    Print2DArray(GetMinefield(), grid_size);

    EndGame(); // frees pointers
    return 0;
};