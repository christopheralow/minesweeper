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
            if (displayBoard[r][c] == '*') {
                cout << "* ";
            }
            else if (displayBoard[r][c] == '0') {
                cout << "  ";
            }
            else if (displayBoard[r][c] == 'F') {
                cout << "F ";
            }
            else if (displayBoard[r][c] >= '1' && displayBoard[r][c] <= '8') {
                cout << displayBoard[r][c] << " ";
            }
            else if (displayBoard[r][c] == '#') {
                cout << "# ";
            }
        }
        cout << "\n";
    }
}

// Randomly assigns mines to actual Grid, first pick has safezone
void DeployMines(char hiddenBoard[SML][SML], int safeRow, int safeCol) {
    int numMinesPlaced = 0;

    while (numMinesPlaced < MINES_SML) {
        // Select a random row and column
        int row = rand() % SML;
        int col = rand() % SML;

        // Safe Zone starts out with flood fill on first turn
        bool inSafeZone = row >= safeRow - 1 && row <= safeRow + 1 &&
                           col >= safeCol - 1 && col <= safeCol + 1;

        // Check cell status and drop a mine
        if (hiddenBoard[row][col] != '*' && !inSafeZone) {
            hiddenBoard[row][col] = '*';
            numMinesPlaced++;
        }
    }
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

void RevealCell(char displayBoard[SML][SML], const char hiddenBoard[SML][SML], int rowIndex, int colIndex) {
    // Prevent checking outside area of actual grid
    if (rowIndex < 0 || rowIndex >= SML || colIndex < 0 || colIndex >= SML) {
        return;
    }
    // Recursion stops if displayed board is already revealed at location
    if (displayBoard[rowIndex][colIndex] != '#') {
        return;
    }
    // Reveal Cell at user selected location
    displayBoard[rowIndex][colIndex] = hiddenBoard[rowIndex][colIndex];

    // Reveal all 8 surrounding cells and repeat for any revealed 0 cells.
    if (hiddenBoard[rowIndex][colIndex] == '0') {
        RevealCell(displayBoard, hiddenBoard, rowIndex - 1, colIndex);
        RevealCell(displayBoard, hiddenBoard, rowIndex + 1, colIndex);
        RevealCell(displayBoard, hiddenBoard, rowIndex, colIndex - 1);
        RevealCell(displayBoard, hiddenBoard, rowIndex, colIndex + 1);
        RevealCell(displayBoard, hiddenBoard, rowIndex - 1, colIndex - 1);
        RevealCell(displayBoard, hiddenBoard, rowIndex - 1, colIndex + 1);
        RevealCell(displayBoard, hiddenBoard, rowIndex + 1, colIndex - 1);
        RevealCell(displayBoard, hiddenBoard, rowIndex + 1, colIndex + 1);
    } 
}

string GetValidInput() {
    string userInput;
    bool validInput = false;

    while (!validInput) {
        cout << "Enter a cell (^ to flag): ";
        cin >> userInput;
        
        string addFlag = userInput;
        string userCell = userInput;

        //Checks for flag input and passes the coordinates to next check
        if (addFlag.size() == 3 && addFlag[0] == '^') {
            userCell = addFlag.substr(1);
        }

        // Check for valid cell input
        if (userCell.size() == 2 && 
            userCell[0] >= 'A' && userCell[0] < 'A' + SML && 
            userCell[1] > '0' && userCell[1] <= '0' + SML) {
                validInput = true;
        }
        else {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please try again.\n";
        }
    }

    return userInput;

}

void ToggleFlag(char displayBoard[SML][SML], int row, int col) {
    if (displayBoard[row][col] == '#') {
        displayBoard[row][col] = 'F';
    }
    else if (displayBoard[row][col] == 'F') {
        displayBoard[row][col] = '#';
    }
}

void PlayerTurn(char displayBoard[SML][SML], char hiddenBoard [SML][SML], int& rowIndex, int& colIndex) {
    char userRow;
    int userCol;
    bool addFlag = false;
    bool firstTurn = true;

    do { // Flagging does not consume a player turn
        string userInput = GetValidInput();
        addFlag = false;

        if (userInput.size() == 3) {
            addFlag = true;
            userInput = userInput.substr(1);
        }
        
        userRow = userInput.at(0);
        userCol = userInput.at(1);

        rowIndex = char(userRow - 'A'); // Char val -> user intended row val
        colIndex = userCol - '1'; // Char val -> user intended col val
        
        if (addFlag) {
            ToggleFlag(displayBoard, rowIndex, colIndex);
            PrintBoard(displayBoard);
        } 
        else {
            if (firstTurn) { // Loop Flag to prevent first turn loss
                DeployMines(hiddenBoard, rowIndex, colIndex);
                GetCellVal(hiddenBoard);
                RevealCell(displayBoard, hiddenBoard, rowIndex, colIndex);
                firstTurn = false;
            }
            else {
                RevealCell(displayBoard, hiddenBoard, rowIndex, colIndex);
            }
        }
    } while (addFlag);
}

// Shows mine location on display grid at loss
void RevealMines(char displayBoard[SML][SML], char hiddenBoard[SML][SML]) {
    for (int r = 0; r < SML; r++) {
        for (int c = 0; c < SML; c++) {
            if (hiddenBoard[r][c] == '*') {
                displayBoard[r][c] = hiddenBoard[r][c];
            }
        }
    }
    PrintBoard(displayBoard);
}

bool CheckWin(char displayBoard[SML][SML], char hiddenBoard[SML][SML]) {
    bool winFound = true;

    for (int r = 0; r < SML; r++) {
        for (int c = 0; c < SML; c++) {
            bool isMine = hiddenBoard[r][c] == '*';
            bool isHidden = displayBoard[r][c] == '#' || displayBoard[r][c] == 'F';

            if (!isMine && isHidden) {
                winFound = false;
            }
        }
    }

    return winFound; 
}

void PlayGame(char displayBoard[SML][SML], char hiddenBoard[SML][SML]) {
    int rowIndex;
    int colIndex;
    bool gameOver = false; // NOTE: need to build CheckWin function
    
    while (!gameOver) {
        PrintBoard(displayBoard);
        PlayerTurn(displayBoard, hiddenBoard, rowIndex, colIndex);

        // Win & Lose Conditions
        if (displayBoard[rowIndex][colIndex] == '*') {
            gameOver = true;
            RevealMines(displayBoard, hiddenBoard);
            cout << "*BOOM* You Lose. Hit a mine at " << char('A' + rowIndex) << colIndex + 1 << ".\n";
            break;
        }
        gameOver = CheckWin(displayBoard, hiddenBoard);
        if (gameOver) {
            PrintBoard(displayBoard);
            cout << "You WIN!";
            break;
        }
    }       
}

int main() {
    srand(time(0)); // for mine placement
    char displayGrid[SML][SML];
    char hiddenGrid[SML][SML];
    
    // Initialize Boards
    for (int r = 0; r < SML; r++) {
        for (int c = 0; c < SML; c++) {
            displayGrid[r][c] = '#';
        }
    }
    for (int r = 0; r < SML; r++) {
        for (int c = 0; c < SML; c++) {
            hiddenGrid[r][c] = '0';
        }
    }

    PlayGame(displayGrid, hiddenGrid);

    
    return 0;

}