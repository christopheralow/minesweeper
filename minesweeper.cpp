#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

const int SML = 9;
const int MINES_SML = 10;

void PrintBoard(const char board[SML][SML]) {
    cout << "  ";
    for (int n = 0; n < SML; n++) {
        cout << n + 1 << " ";
    }
    cout << endl;

    for (int r = 0; r < SML; r++) {
        cout << char('A' + r) << " ";
        for (int c = 0; c < SML; c++) {
                if (board[r][c] == '0') {
                    cout << "  ";
                }
                else if (board[r][c] == 'F') {
                    cout << "F "; 
                }
                // Needs -> || board[r][c] == '*' but ommitted for testing
                else if (board[r][c] == '#') {
                    cout << "# ";
                }
                // For testing only
                else if (board[r][c] == '*') {
                    cout << "* ";
                }
                // NOTE: Need to build and cout number of nearby mines logic
        }
        cout << "\n";
    }
}

void DeployMines(char board[SML][SML]) {
    int minePlaced = 0;

    while (minePlaced < MINES_SML) {
        // Select a random row and column 
        int row = rand() % SML;
        int col = rand() % SML;
        // Check cell status and drop a mine
        if (board[row][col] != '*' && board[row][col] != '0') {
            board[row][col] = '*'; 
            minePlaced++;
        }
    }
    cout << "\nMines placed: " << minePlaced << " (expected " << MINES_SML << ")\n"; //test
    // NOTE: Must add a check to prevent first turn loss
}

int main() {

    srand(time(0)); // for mine placement

    char gridBoard[SML][SML];

    for (int r = 0; r < SML; r++) {
        for (int c = 0; c < SML; c++) {
        gridBoard[r][c] = '#';
        }
    }

    DeployMines(gridBoard);
    PrintBoard(gridBoard);
    
    return 0;

}