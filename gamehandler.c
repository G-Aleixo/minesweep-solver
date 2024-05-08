#include "gamehandler.h"
#include <stdlib.h>
#include <time.h>


/**
0 unknown
1 is flagged as mine
2-11 mines nearby + 2
*/
static UByte* minefield;

// this is the minefield that the player sees
static UByte* uncovered_minefield;

static int num_mines;
static int grid_size;

// returns a pointer to the first element of the array of a square array of size grid_size
UByte* InitializeMinefield(int grid_size){
    UByte* minefield = (UByte*)malloc(grid_size * grid_size * sizeof(UByte));
    for (int i = 0; i < grid_size * grid_size; i++){
        minefield[i] = 0;
    }
    return minefield; // board is a pointer to the first element of the arraye
}

UByte* PopulateMinefield(UByte* board, int grid_size, int num_mines){
    int x, y;
    srand(time(NULL));
    // Randomly place mines on the board
    for (int i = 0; i < num_mines; i++) {
        do {
            x = rand() % grid_size;
            y = rand() % grid_size;
        } while (minefield[x * grid_size + y] == 1); // Ensure the cell is not already a mine
        minefield[x * grid_size + y] = 1; // Mark the cell as a mine
    }
    return minefield;
}

static void FillNonMineSpaces(UByte* minefield, int grid_size) {
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            if (minefield[i * grid_size + j] != 1) {
                int count = 0;
                // Check the surrounding cells for mines
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = i + dx;
                        int ny = j + dy;
                        if (nx >= 0 && nx < grid_size && ny >= 0 && ny < grid_size && minefield[nx * grid_size + ny] == 1) {
                            count++;
                        }
                    }
                }
                minefield[i * grid_size + j] = count + 2;
            }
        }
    }
}

UByte* InitializeUncoveredMines(int grid_size){
    UByte* uncovered_minefield = (UByte*)malloc(grid_size * grid_size * sizeof(UByte));
    for (int i = 0; i < grid_size * grid_size; i++){
        uncovered_minefield[i] = 0;
    }
    return uncovered_minefield; // board is a pointer to the first element of the arraye
}

int GetGridSize() {
    return grid_size;
}

int GetNumMines() {
    return num_mines;
}

UByte* GetMinefield() {
    return uncovered_minefield;
}

int UpdateBoard(UByte action, int x, int y) {
    switch (action) {
        case 0: // Uncover
            if (minefield[x * grid_size + y] == 1 && uncovered_minefield[x * grid_size + y] != 2) {
                return -1; // Game over
            }
            uncovered_minefield[x * grid_size + y] = minefield[x * grid_size + y];
            break;
        case 1: // Flag
            uncovered_minefield[x * grid_size + y] = 2; // Mark the cell as flagged
            break;
    }
    return 0;
}

void StartGame(int param_grid_size, int param_num_mines) {
    num_mines = param_num_mines;
    grid_size = param_grid_size;
    // Initialize the minefield
    minefield = InitializeMinefield(param_grid_size);
    // Initialize the uncovered minefield
    uncovered_minefield = InitializeUncoveredMines(param_grid_size);
    // Populate the minefield with mines
    minefield = PopulateMinefield(minefield, grid_size, num_mines);
    // Fill the non-mine spaces with the number of surrounding mines
    FillNonMineSpaces(minefield, grid_size);
}