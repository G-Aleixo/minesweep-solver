#include <stdio.h>

typedef unsigned char UByte; // unsigned byte, 0 to 255
typedef char SByte; // signed byte, -128 to 127


/*
i need to store the values
unknown
is mine
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
1 is mine
2-11 mines nearby + 2
12 uncover position
15 out of bounds

may relocate the mines nearby later to remove the extra operation when decoding mine value
*/

/*
Returns the state of a certain x, y position, returns 15 for outside board
*/
int IsOutOfBounds(UByte *board, int grid_size, int x, int y){
    if ((x < 0 | x > (grid_size - 1)) | (y < 0 | y > (grid_size - 1))) {
        return 1;}

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
}

int GetSurroudingMines(UByte *board, int grid_size, int x, int y){
    int mines_nearby = 0;
    if (GetPositionData(board, grid_size, x - 1, y - 1) == 1){mines_nearby++;};
    if (GetPositionData(board, grid_size, x - 1, y    ) == 1){mines_nearby++;};
    if (GetPositionData(board, grid_size, x - 1, y + 1) == 1){mines_nearby++;};
    if (GetPositionData(board, grid_size, x    , y + 1) == 1){mines_nearby++;};
    if (GetPositionData(board, grid_size, x + 1, y + 1) == 1){mines_nearby++;};
    if (GetPositionData(board, grid_size, x + 1, y    ) == 1){mines_nearby++;};
    if (GetPositionData(board, grid_size, x + 1, y - 1) == 1){mines_nearby++;};
    if (GetPositionData(board, grid_size, x    , y - 1) == 1){mines_nearby++;};
    return mines_nearby;
};

void PrintBoard(UByte *board, int grid_size){
    for (size_t y = 0; y < grid_size; y++){
        for (size_t x = 0; x < grid_size; x++){
            printf("%d ", GetPositionData(board, grid_size, x, y));
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
    UByte mines[grid_size][(grid_size + 1) / 2];
    
    /* grid state
    1 2
    3 4
    */
    UByte loc1 = 1; // mine
    UByte loc2 = 4; // 2 mines nearby
    UByte loc3 = 4; // 2 mines nearby
    UByte loc4 = 0; // unknown

    mines[0][0] = loc1 | loc2 << 4;
    mines[1][0] = loc3 | loc4 << 4;

    PrintBoard((UByte*)mines, grid_size);

    printf("\n");

    SetPositionData((UByte*)mines, grid_size, 0, 0, 3);
    printf("%d\n", GetPositionData((UByte*)mines, grid_size, 0, 0));

    return 0;
};