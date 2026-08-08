#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

const int SML = 9;
const int MINES_SML = 10;

void PrintBoard(const char displayBoard[SML][SML]) {

    cout << "  ";
    for (int n = 0; n < SML; n++) {
        cout << n + 1 << " ";
    }
    cout << endl;

    for (int r = 0; r < SML; r++) {
        cout << char('A' + r) << " ";
        for (int c = 0; c < SML; c++) {
            if (displayBoard[r][c] == '0') {
                cout << "  ";
            }
            else if (displayBoard[r][c] == 'F') {
                cout << "F ";
            }
            else if (displayBoard[r][c] >= '1' && displayBoard [r][c] <= '8') {
                cout << displayBoard[r][c] << " ";
            }
            else if (displayBoard[r][c] == '#' || displayBoard[r][c] == '*') {
                cout << "# ";
            }
            
            // NOTE: Need to build and cout number of nearby mines logic
        }
        cout << "\n";
    }
}


void DeployMines(char hiddenBoard[SML][SML]) {
    int numMinesPlaced = 0;

    while (numMinesPlaced < MINES_SML) {
        // Select a random row and column 
        int row = rand() % SML;
        int col = rand() % SML;
        // Check cell status and drop a mine
        if (hiddenBoard[row][col] != '*') {
            hiddenBoard[row][col] = '*'; 
            numMinesPlaced++;
        }
    }
    // NOTE: Must add a check to prevent first turn loss
}

bool IsMine(const char hiddenBoard[SML][SML], int row, int col) {
    bool mineFound = false;
    
    // Prevent checking outside range of board
    if (row < 0 || row >= SML || col < 0 || col >= SML) {
        mineFound = false;
    }
    else if (hiddenBoard[row][col] == '*') {
        mineFound = true;
    }

    return mineFound;
    // NOTE: bit redundant on the statement here could use logic refactor later
}

void GetCellVal(char hiddenBoard[SML][SML]) {
    for (int r = 0; r < SML; r++) {
        for (int c = 0; c < SML; c++) {
            // Skip '*' cells
            if (hiddenBoard[r][c] == '*') {
                continue;
            } 
            // Sum of 8 bool statments makes cell value
            int cellVal = IsMine(hiddenBoard, r - 1, c - 1) + IsMine(hiddenBoard, r - 1, c + 0)
                        + IsMine(hiddenBoard, r - 1, c + 1) + IsMine(hiddenBoard, r + 0, c - 1)
                        + IsMine(hiddenBoard, r + 0, c + 1) + IsMine(hiddenBoard, r + 1, c - 1)
                        + IsMine(hiddenBoard, r + 1, c - 0) + IsMine(hiddenBoard, r + 1, c + 1);
            hiddenBoard[r][c] = char('0' + cellVal);
        }
    } 
}

void PlayerTurn(char displayBoard[SML][SML], const char hiddenBoard [SML][SML]) {
    char userRow;
    int userCol;

    cout << "Select a row: ";
    cin >> userRow;
    cout << "Select a column: ";
    cin >> userCol;

    int rowIndex = userRow - 'A';
    displayBoard[rowIndex][userCol] = hiddenBoard[rowIndex][userCol];

}

void PlayGame(char displayBoard[SML][SML], char hiddenBoard[SML][SML]) {
    int turnCounter = 0;
    bool gameOver = false;

    PrintBoard(displayBoard);
    PlayerTurn(displayBoard, hiddenBoard);
    DeployMines(hiddenBoard);
    GetCellVal(hiddenBoard);
    PrintBoard(displayBoard);
    
    while (!gameOver) {
        PlayerTurn(displayBoard, hiddenBoard);
        PrintBoard(displayBoard);
        //gameOver = CheckLoss()
            //if gameOver
                //break
        //gameOver = CheckWin()
            //if gameOver
                //break
    }       
    //PlayerTurn;
}

int main() {

    srand(time(0)); // for mine placement
    char displayGrid[SML][SML];
    char actualGrid[SML][SML];
     
    // Initialize Board
    for (int r = 0; r < SML; r++) {
        for (int c = 0; c < SML; c++) {
        displayGrid[r][c] = '#';
        }
    }
    for (int r = 0; r < SML; r++) {
        for (int c = 0; c < SML; c++) {
        actualGrid[r][c] = '0';
        }
    }
    PlayGame(displayGrid, actualGrid);

    
    return 0;

}