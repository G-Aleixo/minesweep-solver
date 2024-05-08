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

static int IsOutOfBounds(int grid_size, int x, int y){
    if ((x < 0 | x > (grid_size - 1)) | (y < 0 | y > (grid_size - 1))){
        return 1;};
    return 0;
};

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
        } while (minefield[y * grid_size + x] == 1); // Ensure the cell is not already a mine
        minefield[y * grid_size + x] = 1; // Mark the cell as a mine
    }
    return minefield;
}

static void FillNonMineSpaces(UByte* minefield, int grid_size) {
    for (int y = 0; y < grid_size; y++) {
        for (int x = 0; x < grid_size; x++) {
            if (minefield[y * grid_size + x] != 1) {
                int count = 0;
                // Check the surrounding cells for mines
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (dx == 0 && dy == 0) {
                            continue;
                        }
                        if (!IsOutOfBounds(grid_size, nx, ny) && minefield[ny * grid_size + nx] == 1) {
                            count++;
                        }
                    }
                }
                minefield[y * grid_size + x] = count + 2;
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
            if (minefield[y * grid_size + x] == 1 && uncovered_minefield[y * grid_size + x] == 0) {
                return -1; // Game over
            }
            uncovered_minefield[y * grid_size + x] = minefield[y * grid_size + x];
            break;
        case 1: // Flag
            if (uncovered_minefield[y * grid_size + x] != 0) {
                return 0; // Cell is already uncovered or flagged, do nothing
            }
            uncovered_minefield[y * grid_size + x] = 1; // Mark the cell as flagged
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

void EndGame() {
    free(minefield);
    minefield = NULL;
    free(uncovered_minefield);
    uncovered_minefield = NULL;
}