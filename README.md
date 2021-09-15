# Minesweeper
Minesweeper implemented in visual c++

## How to run:
Run the "MineSweeper.sln" file in Microsoft Visual Studio 2019 (or later version). Note that you will need a C++ compiler (it uses some paradigms from C++ 17).

## How to play
Minesweeper is a single-player puzzle video game. The objective of the game is to clear a rectangular board containing hidden bombs without detonating any of them, with help from clues about the number of neighboring mines in each field. 

In the game, the bombs are scattered throughout a board, which is divided into cells. The cells have three states: EXPOSED, NOT EXPOSED and FLAGGED. A cell that is NOT EXPOSED is blank and clickable, while an EXPOSED cell is not clickable anymore. FLAGGED cells are those marked by the player to indicate a potential bomb location.

A player left-clicks a cell to expose it. If a player exposes a bomb cell, the game ends, as there is only 1 life per game! Otherwise, the exposed cell displays either a number, indicating the number of mines diagonally and/or adjacent to it, or a blank tile (or "0"), and all adjacent non-mined cells will automatically be exposed. Right-clicking on a cell will flag it, causing a flag to appear on it. Flagged cells are still considered covered, and a player can click on them to uncover them, although typically they must first be unflagged with an additional right-click.

To win the game, players must uncover all non-bomb cells. Flagging all the bomb-cells is not required.

## Class Structures

This program demonstrates a single responsibility principle by having multiple classes that have their own responsibilities.

* `class Cell` - The class that encapsulates the properties of a cell.  Essentially, it holds the state that the cell is in - is it exposed, not exposed, or flagged? It also holds the count of the neighbourhood bombs.
* `class Grid` - This class consists of a matrix of cells. It has the calculation information about the grid. This class is only responsible to do the calculations and change the state of the cells in the grid respectively.
* `class GridWindow` - This class is a wrapper around `Grid` (implemented as `GridWindow` **has** `Grid` ). While `Grid` is responsible for the 'backend' calculation of the minesweeper, `GridWindow` is the calculation of the 'front-end' properties of the grid. It is responsible to get the click information and pass it down to the 'back-end'. Notice that the click happens in pixel units in front end. It gets passed down to the back-end in the grid units. Also, this class is responsible to initialize the grid according to the window that's open.
* `class Drawer` - This class contains a bunch of static functions used for drawing purposes. The functions take either a `GridWindow` or a `Cell`. This could be further refactored by having a separate drawer for cells. But for simple operations as this, this should suffice.

## Developer

This program is developed by Prabhakar Regmi.
