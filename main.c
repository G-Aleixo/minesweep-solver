#include <stdio.h>

#define INRANGE(number, range_lower, range_upper) (((number) <= range_upper) && ((number) > range_lower))

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
the minesweeper solver will return  the following data to the game handler
do nothing
reveal tile
flag tile

so we can compress a 2x2 area into a single UByte, going in this pattern
1 2
3 4
halving memory usage twice
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
    if (x%2){
        board[index] &= 0b00001111;
        board[index] |= data << 4;
    }else{
        board[index] &= 0b11110000;
        board[index] |= data;
    }
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

UByte* ResolveSolvedPositions(UByte *board, int grid_size){
    UByte return_data[(grid_size+1)/2][(grid_size+1)/2];
    for (size_t y = 0; y < grid_size; y++){
        for (size_t x = 0; x < grid_size; x++){
            UByte position_data = GetPositionData(board, grid_size, x, y); // get the current positions data
            if ((position_data <= 11) && (position_data >= 3)){ // if the position is not a mine or unknown
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
                
                if (surrouding_empty + surrouding_mines - surrouding_filled >= (position_data - 3)){
                    // TODO: Flag all surrouding tiles, the game handler will handle it
                    printf("Flag positions surrouding %d : %d\n", (int)x, (int)y);
                };

                if (surrouding_mines >= (position_data - 3)){ // if every mine is met
                    // TODO: Uncover all surrouding positions
                    printf("Uncover positions surrouding %d : %d\n", (int)x, (int)y);
                };
            } else if (position_data == 2){ // if there are 0 mines nearby, only uncover positions
                // uncover positions
                printf("Uncover positions surrouding %d : %d\n", (int)x, (int)y);
            };
        };
    };
    return 0;
};

void PrintBoard(UByte *board, int grid_size){
    for (size_t y = 0; y < grid_size; y++){
        for (size_t x = 0; x < grid_size; x++){
            printf("%d ", GetPositionData(board, grid_size, (int)x, (int)y));
        };
        printf("\n");
    };
};

int main(){
    int grid_size = 3;

    /*
    make a grid of the mines
    first size is the y and the next is x

    the x size is actually duplicated due to the way the data is stored, and the y value is normal
    */
    UByte board[grid_size][(grid_size + 1) / 2];
    
    /* grid state
    1 4
    4 0
    */
    UByte loc1 = 1; // flagged as mine
    UByte loc2 = 4; // 2 mines nearby
    UByte loc3 = 4; // 2 mines nearby
    UByte loc4 = 0; // unknown

    board[0][0] = loc1 | loc2 << 4;
    board[1][0] = loc3 | loc4 << 4;

    SetPositionData((UByte *)board, grid_size, 2, 0, 2);
    SetPositionData((UByte *)board, grid_size, 2, 1, 4);

    /*
    1 4 2
    4 0 4
    0 0 0
    */

    PrintBoard((UByte *)board, grid_size);

    printf("%d\n", IsSurroudingData((UByte *)board, grid_size, 1, 1, 4));
    ResolveSolvedPositions((UByte *)board, grid_size);


    return 0;
};