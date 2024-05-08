#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

typedef unsigned char UByte; // unsigned byte, 0 to 255

UByte* InitializeMinefield(int grid_size);
UByte* InitializeUncoveredMines(int grid_size);
UByte* PopulateMinefield(UByte* board, int grid_size, int num_mines);

UByte* GetMinefield();
int GetNumMines();
int GetGridSize();

int UpdateBoard(UByte action, int x, int y);

void StartGame(int param_grid_size, int param_num_mines);

#endif