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
        }
        cout << "\n";
    }
}

// Randomly assigns mines to actual Grid
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

// For use inside GetCellVal to check for mines within the area of the actual Grid
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

// Determines value of cells based on neighboring mines and assigns value on actual Grid
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

            // Int -> Char
            hiddenBoard[r][c] = char('0' + cellVal);
        }
    } 
}

void RevealCell(char displayBoard[SML][SML], char hiddenBoard[SML][SML], int rowIndex, int colIndex) {

    // Prevent checking outside area of actual grid
    if (rowIndex < 0 || rowIndex > SML|| colIndex < 0 || colIndex > SML) {
        return;
    }
    // Recursion stops if displayed board is already revealed at location
    if (displayBoard[rowIndex][colIndex] != '#') {
        return;
    }
    // Reveal Cell at user selected location
    displayBoard[rowIndex][colIndex] = hiddenBoard[rowIndex][colIndex];

    if (hiddenBoard[rowIndex][colIndex] == '0') {
        RevealCell(displayBoard, hiddenBoard, rowIndex - 1, colIndex);
        RevealCell(displayBoard, hiddenBoard, rowIndex + 1, colIndex);
        RevealCell(displayBoard, hiddenBoard, rowIndex, colIndex - 1);
        RevealCell(displayBoard, hiddenBoard, rowIndex, colIndex + 1);
    } 
}

void PlayerTurn(char displayBoard[SML][SML], char hiddenBoard [SML][SML]) {
    char userRow;
    int userCol;

    //NOTE : Want to combine input on a single string then parse
    cout << "Select a row: ";
    cin >> userRow;
    cout << "Select a column: ";
    cin >> userCol;

    int rowIndex = char(userRow - 'A'); // Char val -> intended row val
    int colIndex = userCol - 1;

    RevealCell(displayBoard, hiddenBoard, rowIndex, colIndex);
    // Change display board to reflect hidden board at specified cell
    // displayBoard[rowIndex][colIndex] = hiddenBoard[rowIndex][colIndex];
}

void PlayGame(char displayBoard[SML][SML], char hiddenBoard[SML][SML]) {
    int turnCounter = 0; //for turn counter not built yet
    bool gameOver = false; // NOTE: need to build CheckWin function

    PrintBoard(displayBoard);
    DeployMines(hiddenBoard);
    GetCellVal(hiddenBoard);
    
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
     
    // Initialize Boards
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