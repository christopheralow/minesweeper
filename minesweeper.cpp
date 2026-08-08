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
            else if (board[r][c] >= '1' && board [r][c] <= '8') {
                cout << board[r][c] << " ";
            }
            else if (board[r][c] == 'F') {
                cout << "F "; 
            }
            else if (board[r][c] == '#' || board[r][c] == '*') {
                cout << "# ";
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
    // NOTE: Must add a check to prevent first turn loss
}

bool IsMine(const char board[SML][SML], int row, int col) {
    bool mineFound = false;
    
    // Prevent checking outside range of board
    if (row < 0 || row >= SML || col < 0 || col >= SML) {
        mineFound = false;
    }
    else if (board[row][col] == '*') {
        mineFound = true;
    }

    return mineFound;
    // NOTE: bit redundant on the statement here could use logic refactor later
}

void GetCellVal(char board[SML][SML]) {
    for (int r = 0; r < SML; r++) {
        for (int c = 0; c < SML; c++) {
            // Skip '*' cells
            if (board[r][c] == '*') {
                continue;
            } 
            // Sum of 8 bool statments makes cell value
            int cellVal = IsMine(board, r - 1, c - 1) + IsMine(board, r - 1, c + 0)
                        + IsMine(board, r - 1, c + 1) + IsMine(board, r + 0, c - 1)
                        + IsMine(board, r + 0, c + 1) + IsMine(board, r + 1, c - 1)
                        + IsMine(board, r + 1, c - 0) + IsMine(board, r + 1, c + 1);
            board[r][c] = char('0' + cellVal);
        }
    } 
}

void PlayerTurn(char board[SML][SML]) {
    char userRow;
    int userCol;

    cout << "Select a row: ";
    cin >> userRow;
    cout << "Select a column: ";
    cin >> userCol;

    int rowIndex = userRow - 'A';
    board[rowIndex][userCol] = '0';

}

void PlayGame(char board[SML][SML]) {
    int turnCounter = 0;
    bool gameOver = false;

    PrintBoard(board);
    PlayerTurn(board);
    DeployMines(board);
    GetCellVal(board);
    PrintBoard(board);
    
    while (!gameOver) {
        PlayerTurn(board);
        PrintBoard(board);
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
    char initGrid[SML][SML];
     
    // Initialize Board
    for (int r = 0; r < SML; r++) {
        for (int c = 0; c < SML; c++) {
        initGrid[r][c] = '#';
        }
    }
    PlayGame(initGrid);

    
    return 0;

}