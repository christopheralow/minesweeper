# Minesweeper

A terminal-based Minesweeper game written in C++. Clear every safe square without detonating a mine.

```
 __  __  ___  _  _  ___   ___ __      __ ___  ___  ___  ___  ___
|  \/  ||_ _|| \| || __| / __|\ \    / /| __|| __|| _ \| __|| _ \
| |\/| | | | |  ` || _|  \__ \ \ \/\/ / | _| | _| |  _/| _| |   /
|_|  |_||___||_|\_||___| |___/  \_/\_/  |___||___||_|  |___||_|_\
* * * * * * * * * * * * * * * * * * * * * * * by Christopher A Low
```

## Features

* Three difficulty levels: Lieutenant, Major, and Colonel
* Recursive flood fill that opens entire connected empty regions in one move
* Modern Gameplay (first turn is always safe and opens a region)
* Flagging that does not consume a turn, so you can mark as many squares as you like
* Optional Unicode display mode with an ASCII fallback for terminals that need it
* Full input validation handles bad coordinates, wrong lengths, and lowercase entry
* Replay loop (start a new game without restarting the program)

## How to Play

Reveal every square that does not contain a mine. Numbers tell you how many mines touch that square.
Hit a mine and the game ends.

| Input | Action |
|---|---|
| `D5` | Reveal row D, column 5 |
| `^D5` | Place or remove a flag at row D, column 5 |
| `d5` | Same as `D5` — input is case-insensitive |

```
  1 2 3 4 5 6 7 8 9
A # # # 1       1 #
B # # # 1 1 1   1 #
C # # # # F 1   1 #
D # # # # # 1   1 1
E # # # # # 1
F # # # # # 1 1 1
G # # # # # # # #
H # # # # # # # #
I # # # # # # # #
```

This version ships with an improved graphics mode. 
Answering yes to improved graphics at startup swaps the ASCII symbols for Unicode ones.

| ASCII | Unicode | Meaning |
|---|---|---|
| `#` | `■` | Unrevealed |
| `F` | `⚑` | Flagged |
| `*` | `✶` | Mine |

```
  1 2 3 4 5 6 7 8 9
A ■ ■ ■ 1       1 ■
B ■ ■ ■ 1 1 1   1 ■
C ■ ■ ■ ■ ⚑ 1   1 ■
D ■ ■ ■ ■ ■ 1   1 1
E ■ ■ ■ ■ ■ 1
F ■ ■ ■ ■ ■ 1 1 1
G ■ ■ ■ ■ ■ ■ ■ ■
H ■ ■ ■ ■ ■ ■ ■ ■
I ■ ■ ■ ■ ■ ■ ■ ■
```

The mode is optional because Unicode support varies between terminals. Characters that render
cleanly on macOS and Linux can come out as replacement boxes on older Windows consoles. The
ASCII set is the safe default and both modes play identically.

## Difficulty

| Level | Board | Mines | Density |
|---|---|---|---|
| Lieutenant | 9 × 9 | 10 | 12.3% |
| Major | 16 × 16 | 40 | 15.6% |
| Colonel | 22 × 22 | 101 | 20.9% |

Densities follow the classic 1990 Windows curve. Square boards were chosen over the original's
30 × 16 Expert layout so the hardest level still fits in a standard terminal window.

## Build and Run

```
g++ -std=c++17 -o minesweeper minesweeper.cpp
./minesweeper
```

Requires a C++ compiler. No external libraries.

## Troubleshooting: Improved Graphics Mode

Improved graphics mode uses three symbols that need font and encoding support:

If these show up unintended:

* **Windows Command Prompt (`cmd.exe`)** — run `chcp 65001` before launching the game to switch the
  console to UTF-8. If symbols still look wrong, the console font may not support them: right-click
  the title bar → Properties → Font, and switch to a TrueType font such as Consolas or Cascadia Mono.
  The legacy "Raster Fonts" option does not support these characters.
* **Windows Terminal** — generally works with no action needed.
* **macOS (Terminal, iTerm2) and Linux** — generally work out of the box, since these default to
  UTF-8. If something still looks wrong, run `locale` to confirm your encoding is UTF-8; if it is
  not, add `export LANG=en_US.UTF-8` to your shell profile.
* **Still not working?** Answer `N` when asked to enable improved graphics. Classic mode uses only
  plain ASCII (`#`, `F`, `*`) and works identically on every terminal.

## How the Flood Fill Works

Revealing a square with no adjacent mines opens the whole connected empty region. This works because
a zero guarantees no mines in any of its eight neighbors, so all eight can be revealed safely — and
any of those that is also a zero repeats the process.

`RevealCell` handles one square per call and hands the rest to itself, stopping at three base cases:

1. Off the grid — return immediately
2. Already revealed — return immediately, which is what prevents infinite recursion, since every
   square appears in its own neighbors' neighbor lists
3. Non-zero adjacency count — reveal the square but do not spread, forming the border of the opening

The square is marked revealed *before* the eight recursive calls fire. If it were marked afterward,
each neighbor would call back into it and the recursion would never terminate.

## Technical Notes

* Two parallel `char[22][22]` arrays store state — `hiddenBoard` holds mines and adjacency counts,
  `displayBoard` holds what the player sees. Only `displayBoard` is ever printed, so leaking mine
  positions is structurally impossible. Revealing a square is a single copy between them.
* Both grids are declared at the largest board size, with a `dim` field controlling how much is in use
* Mines are placed *after* the first input, excluding the chosen square and its eight neighbors,
  which is what guarantees the opening move reveals a region instead of a lone number
* Adjacency counts come from summing eight boolean checks. An `IsMine()` helper does the bounds
  check internally and returns `false` for coordinates off the grid, which collapses the interior,
  edge, and corner cases into one expression
* Adjacency is computed once after mine placement and never recalculated
* Cells render at a fixed two characters wide so the board stays visually square across all three
  difficulties, which is why column headers past 9 run together rather than being spaced out
* `srand(time(0))` is seeded once in `main()`

## Roadmap v1.1

* Turn counter and timer
* Score based on time, turns, and mines revealed
* Local leaderboard
* General difficulty: 22 × 22 at 25.8% density, above the point where every position can be solved
  by logic. Instead of ending on a forced guess, this mode grants lives and treats a revealed mine
  as a cost affetcing end score rather than a loss.
* Adventure Mode: progress through the ranks, with promotion to General unlocked by clearing all levels
* No-guess board generation (a solver that regenerates any board requiring a coin flip)

## Author

Christopher A Low — built after COMSC-110 at Diablo Valley College, as a self study companion project
to [Connect Four](https://github.com/YOUR-USERNAME/connect4).
