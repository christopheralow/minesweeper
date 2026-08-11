#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iomanip>

using namespace std;

inline const char* TITLE = R"(
 __  __  ___  _  _  ___   ___ __      __ ___  ___  ___  ___  ___
|  \/  ||_ _|| \| || __| / __|\ \    / /| __|| __|| _ \| __|| _ \
| |\/| | | | |  ` || _|  \__ \ \ \/\/ / | _| | _| |  _/| _| |   /
|_|  |_||___||_|\_||___| |___/  \_/\_/  |___||___||_|  |___||_|_\
* * * * * * * * * * * * * * * * * * * * * * * by Christopher A Low
)";

inline const char* MENU = R"(
               +-------------------------------+
               |      G A M E  *  M O D E      |
               +-------------------------------+

            [1] LIEUTENANT   [2] MAJOR   [3] COLONEL

             Select a Game Mode (or '0' to Exit): )";

const int SML = 9;
const int MED = 16;
const int LRG = 22;
const int MINES_SML = 10;
const int MINES_MED = 40;
const int MINES_LRG = 101;

// Boards declared at LRG X LRG, dim controls utilized grid dimensions
struct Board {
    char displayBoard[LRG][LRG];
    char hiddenBoard[LRG][LRG];
    int dim;
    int numMines;
};

int LaunchMenu() {
    int gameMode;

    cout << MENU;
    cin >> gameMode;
    while (cin.fail() || gameMode != 0 && gameMode != 1
                      && gameMode != 2 && gameMode != 3) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Select a valid game mode: ";
        cin >> gameMode;
    }

    return gameMode;
}

bool EnablePrettyMod() {
    char enableMod;
    bool prettyMod;

    cout << endl;
    cout << "Enable improved graphics? (warning: can cause issues for certain OS): ";
    cin >> enableMod;

    while (cin.fail() || enableMod != 'Y' && enableMod != 'y'
                      && enableMod != 'N' && enableMod != 'n') {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter \"Y\" or \"N\": " << endl;
        cin >> enableMod;
    }

    if (enableMod == 'Y' || enableMod == 'y') {
        prettyMod = true;
    }
    else {
        prettyMod = false;
    }

    return prettyMod;
}

void PrintBoard(const Board& game, bool prettyMod) {
    cout << endl;
    cout << "  ";
    for (int n = 0; n < game.dim; n++) {
        cout << setw(2) << n + 1 << " ";
    }
    cout << endl;

    for (int r = 0; r < game.dim; r++) {
        cout << char('A' + r) << " ";
        for (int c = 0; c < game.dim; c++) {
            if (game.displayBoard[r][c] == '*') {
                cout << (prettyMod ? " ✶ " : " * ");
            }
            else if (game.displayBoard[r][c] == '0') {
                cout << "   ";
            }
            else if (game.displayBoard[r][c] == 'F') {
                cout << (prettyMod ? " ⚑ " : " F ");
            }
            else if (game.displayBoard[r][c] >= '1' && game.displayBoard[r][c] <= '8') {
                cout << " " << game.displayBoard[r][c] << " ";
            }
            else if (game.displayBoard[r][c] == '#') {
                cout << (prettyMod ? " ■ " : " # ");
            }
        }
        cout << "\n";
    }
}

// Randomly assigns mines to actual Grid, first pick has safezone
void DeployMines(Board& game, int safeRow, int safeCol) {
    int numMinesPlaced = 0;

    while (numMinesPlaced < game.numMines) {
        // Select a random row and column
        int row = rand() % game.dim;
        int col = rand() % game.dim;

        // Safe Zone starts out with flood fill on first turn
        bool inSafeZone = row >= safeRow - 1 && row <= safeRow + 1 &&
                           col >= safeCol - 1 && col <= safeCol + 1;

        // Check cell status and drop a mine
        if (game.hiddenBoard[row][col] != '*' && !inSafeZone) {
            game.hiddenBoard[row][col] = '*';
            numMinesPlaced++;
        }
    }
}

// For use inside GetCellVal to check for mines within the area of the actual Grid
bool IsMine(const Board& game, int row, int col) {
    bool mineFound = false;

    // Prevent checking outside range of board
    if (row < 0 || row >= game.dim || col < 0 || col >= game.dim) {
        mineFound = false;
    }
    else if (game.hiddenBoard[row][col] == '*') {
        mineFound = true;
    }

    return mineFound;
}

// Determines value of cells based on neighboring mines and assigns value on actual Grid
void GetCellVal(Board& game) {
    for (int r = 0; r < game.dim; r++) {
        for (int c = 0; c < game.dim; c++) {
            // Skip '*' cells
            if (game.hiddenBoard[r][c] == '*') {
                continue;
            }
            // Sum of 8 bool statments makes cell value
            int cellVal = IsMine(game, r - 1, c - 1) + IsMine(game, r - 1, c + 0)
                        + IsMine(game, r - 1, c + 1) + IsMine(game, r + 0, c - 1)
                        + IsMine(game, r + 0, c + 1) + IsMine(game, r + 1, c - 1)
                        + IsMine(game, r + 1, c - 0) + IsMine(game, r + 1, c + 1);

            // Int -> Char
            game.hiddenBoard[r][c] = char('0' + cellVal);
        }
    }
}

void RevealCell(Board& game, int rowIndex, int colIndex) {
    // Prevent checking outside area of actual grid
    if (rowIndex < 0 || rowIndex >= game.dim || colIndex < 0 || colIndex >= game.dim) {
        return;
    }
    // Recursion stops if displayed board is already revealed at location
    if (game.displayBoard[rowIndex][colIndex] != '#') {
        return;
    }
    // Reveal Cell at user selected location
    game.displayBoard[rowIndex][colIndex] = game.hiddenBoard[rowIndex][colIndex];

    // Reveal all 8 surrounding cells and repeat for any revealed 0 cells.
    if (game.hiddenBoard[rowIndex][colIndex] == '0') {
        RevealCell(game, rowIndex - 1, colIndex);
        RevealCell(game, rowIndex + 1, colIndex);
        RevealCell(game, rowIndex, colIndex - 1);
        RevealCell(game, rowIndex, colIndex + 1);
        RevealCell(game, rowIndex - 1, colIndex - 1);
        RevealCell(game, rowIndex - 1, colIndex + 1);
        RevealCell(game, rowIndex + 1, colIndex - 1);
        RevealCell(game, rowIndex + 1, colIndex + 1);
    }
}

string GetValidInput(int dim) {
    string userInput;
    bool validInput = false;

    while (!validInput) {
        cout << "Enter a cell (^ to flag): ";
        cin >> userInput;

        string rawInput = userInput;

        if (rawInput[0] == '^') {
            rawInput = rawInput.substr(1);
        }
        if (rawInput.size() == 2 || rawInput.size() == 3) {
            bool validRow = rawInput[0] >= 'A' && rawInput[0] < 'A' + dim;
            bool validCol = rawInput[1] >= '0' && rawInput[1] < '0' + 9;
            if (rawInput.size() == 3) {
                validCol = validCol && rawInput[2] >= '0' && rawInput[2] <= '0' + 9;
            } 
            if (validRow && validCol) {
                validInput = true;
            }
            else {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Please try again.\n";
            }
        }
    }

    return userInput;

}

void ToggleFlag(Board& game, int row, int col) {
    if (game.displayBoard[row][col] == '#') {
        game.displayBoard[row][col] = 'F';
    }
    else if (game.displayBoard[row][col] == 'F') {
        game.displayBoard[row][col] = '#';
    }
}

void PlayerTurn(Board& game, int& rowIndex, int& colIndex, bool prettyMod, bool& isFirstTurn) {
    char userRow;
    int userCol;
    bool addFlag = false;

    do { // Flagging does not consume a player turn
        string userInput = GetValidInput(game.dim);
        addFlag = false;

        if (userInput[0] == '^') {
            addFlag = true;
            userInput = userInput.substr(1);
        }

        userRow = userInput.at(0);
        if (userInput.size() == 2) {
            userCol = userInput.at(1) - '0';
        }
        else if (userInput.size() == 3) {
            // Convert two character string to int
            userCol = (userInput[1] - '0') * 10 + (userInput[2] - '0');
        }

        rowIndex = char(userRow - 'A'); // Char val -> user intended row val
        colIndex = userCol - 1; // Char val -> user intended col val

        if (addFlag) {
            ToggleFlag(game, rowIndex, colIndex);
            PrintBoard(game, prettyMod);
        }
        else {
            if (isFirstTurn) { // Loop Flag to prevent first turn loss
                DeployMines(game, rowIndex, colIndex);
                GetCellVal(game);
                RevealCell(game, rowIndex, colIndex);
                isFirstTurn = false;
            }
            else {
                RevealCell(game, rowIndex, colIndex);
            }
        }
    } while (addFlag);
}

// Shows mine location on display grid at loss
void RevealMines(Board& game, bool prettyMod) {
    for (int r = 0; r < game.dim; r++) {
        for (int c = 0; c < game.dim; c++) {
            if (game.hiddenBoard[r][c] == '*') {
                game.displayBoard[r][c] = game.hiddenBoard[r][c];
            }
        }
    }
    PrintBoard(game, prettyMod);
}

bool CheckWin(const Board& game) {
    bool winFound = true;

    for (int r = 0; r < game.dim; r++) {
        for (int c = 0; c < game.dim; c++) {
            bool isMine = game.hiddenBoard[r][c] == '*';
            bool isHidden = game.displayBoard[r][c] == '#' || game.displayBoard[r][c] == 'F';

            if (!isMine && isHidden) {
                winFound = false;
            }
        }
    }

    return winFound;
}

void PlayGame(Board& game, bool prettyMod) {
    int rowIndex;
    int colIndex;
    bool gameOver = false;
    bool isFirstTurn = true;

    while (!gameOver) {
        PrintBoard(game, prettyMod);
        PlayerTurn(game, rowIndex, colIndex, prettyMod, isFirstTurn);

        // Win & Lose Conditions
        if (game.displayBoard[rowIndex][colIndex] == '*') {
            gameOver = true;
            RevealMines(game, prettyMod);
            cout << "*BOOM* You Lose. Hit a mine at " << char('A' + rowIndex) << colIndex + 1 << ".\n";
            break;
        }
        gameOver = CheckWin(game);
        if (gameOver) {
            PrintBoard(game, prettyMod);
            cout << "You WIN!";
            break;
        }
    }
}

int main() {
    srand(time(0)); // for mine placement

    cout << TITLE;
    bool prettyMod = EnablePrettyMod();

    Board game;
    int gameMode = LaunchMenu();

    while (gameMode != 0) {
        if (gameMode == 1) {
            game.dim = SML;
            game.numMines = MINES_SML;
        }
        else if (gameMode == 2) {
            game.dim = MED;
            game.numMines = MINES_MED;
        }
        else if (gameMode == 3) {
            game.dim = LRG;
            game.numMines = MINES_LRG;
        }
        // Initialize Boards
        for (int r = 0; r < game.dim; r++) {
            for (int c = 0; c < game.dim; c++) {
                game.displayBoard[r][c] = '#';
            }
        }
        for (int r = 0; r < game.dim; r++) {
            for (int c = 0; c < game.dim; c++) {
                game.hiddenBoard[r][c] = '0';
            }
        }

        PlayGame(game, prettyMod);
        gameMode = LaunchMenu();
    }

    return 0;

}
