# Minesweeper (2026)

A terminal-based Minesweeper game written in C++ by Christopher A Low.

---

## Abstract

v1.0 I want to recreate classic Minesweeper(1990) for command line. Users will reveal squares on a grid, 
trying to clear every safe square without detonating a mine. Users will use keyboard input to select 
cells on a 9x9 grid.

v1.1 I will expand the gameplay with common and unique modifiers detailed here. Users will have four 
levels of difficulty to select (Lieutenant, Major, Colonel, General). 

I will also build an "Adventure" game mode by forcing a start on easy mode. Upon completion, users will be 
"promoted" and immediately prompted to continue playing on a harder level that continues to "General" 
difficulty. This is introduced alongside "Casual" game mode where players can pick a desired game difficulty
to play.

---

## Requirements

**Must do:**
- Generate a 9x9 grid with set number of mines placed at random positions
- Let the player choose a square by row and column
- Reveal the number of mines adjacent to the chosen square
- Auto-reveal connected empty regions when a zero-adjacent square is picked
- Let the player flag squares they suspect contain mines
- Print the board between turns
- End the game on a mine hit (loss) or when all safe squares are revealed (win)
- Reveal all mines on loss
- Prevent first turn loss

**Out of scope for v1.0:**
- Saving games to a file
- Timer
- Provide a score based on time and turncounter, and number of mines revealed (General Difficulty only)
- Provide a leaderboard
- Difficulty Levels
- Adventure Mode (With possibly short RPG text as well)
- Solver to produce guaranteed solvable logic grids for 3 of 4 game difficulties. 
- Pretty mode: Utilizing UTF-8 for board but may not render so will have a toggle to activate

---

## Approach

### Levels & Game Balance

Density Difficulty Scale (from minesweeperblast.com):
10-15%: Casual, most logical flow
15-20%: Challenging, requires pattern recognition
20-25%: Expert, high probability constraints
25+%: Extreme, frequent guessing required

This game will follow classic curves with an added challenge mode

**Lieutenant** - Easy -  9 * 9 Board and 10 Mines
**Major** - Intermediate - 16 * 16 Board and 40 Mines
**Colonel** - Hard -  22 * 22 Board and 101 Mines
**General** - Challenge - 22 * 22 Board and 125 Mines

Level: Total Cells, Total Mines, Density
Lieutenant:	81, 10, 12.3% (Classic Curve)
Major: 256, 40, 15.6% (Classic Curve)
Colonel: 484, 101, 20.8% (Classic Curve)
General: 484, 125, 25.8% (Advanced Difficulty)

Note: Square grids were selected for hard mode to help fit comfortably in a terminal window since 
30 columns or rows could be hard to see in a single window on some devices. Mine density
is consistent with classic 1990 version offering similar difficulty but alternative gameplay 
to competitive players.  

Note: General mode will not lose when mine is found. Instead score will be deducted and a life is
deducted from finite amount. If all lives are used, the game is over. 

### Data

I will use parallel 2D arrays of fixed size:

| Array | Purpose |
|---|---|
| `mineGrid` | Where the mines actually are (hidden state) |
| `displayGrid` | What the user sees (visible state) |

Using two grids keeps hidden state separate from visible state, so printing
the board never risks leaking mine positions.

**Cell values in `displayGrid`:**

| Symbol | Meaning |
|---|---|
| `#` | Unrevealed |
| `F` | Flagged by player |
| `1`–`8` | Revealed, showing adjacent mine count |
| `*` | Mine (only drawn at game over) |


### Default Display and User Input
```
   1  2  3  4  5  6  7  8  9
A  #  #  #           2  #  #
B  #  #  #     F  #  #  #  #
C  #  #  #        2  #  #  #
D  #  #  #     1  #  #  #  #
E  #  #  #  #  #  #  #  #  #
F  #  #  #  #  #  #  #  #  #
G  #  #  #  #  #  #  #  #  #
H  #  #  #  #  #  #  #  #  #
I  #  #  #  #  #  #  #  #  #

Select a cell to reveal:
(use ^ to flag) 
```

Rows are categorized by character
Columns are categorized by number

User will be prompted to begin the game by typing a Row Column input 
ie A1, A2,...

User can place flag on cell or remove flag from cell at any point. This does not
increment turn counter. Flags are toggled by typing '^' before any Row Column input. 

### Game Loop

```
Initialize grids
Get first pick from player
Place mines (avoiding first pick)
Reveal first pick

while (game not over)
    Print board
    Get player input: reveal or flag
    Validate input
    If reveal and square is a mine  -> loss, break
    If reveal                       -> uncover square, increment turn counter
    If flag                         -> toggle flag
    If all safe squares revealed    -> win, break

Print final board with mines shown and turn counter
Print result & score based on time (v1.1)
Ask to save high score with name on leaderboard (v1.1)
Ask to play new game 
```

---

### Open Questions
I need to eventually work out how to build 100% solvable boards. 
No guess logic generation will be deferred to later build. 
Solver architecture: Tier 1/2/3 
(Not applicable to Tier 4 by density design. 'Lives' mechanic exist for this mode)
