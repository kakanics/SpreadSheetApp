# Spreadsheet Application

## Classes

1. **sheet**
   - Main class representing the spreadsheet.

2. **iterator**
   - Iterator class for navigating through the spreadsheet.

3. **node**
   - Node class representing a cell in the spreadsheet.

## Functions

### sheet Class Functions

1. **MinMax**
   - Finds the minimum or maximum value in a specified range.

2. **AverageFunction**
   - Calculates the average value in a specified range.

3. **CopyFunction**
   - Copies data from one range to another.

4. **PasteData**
   - Pastes copied data into the spreadsheet.

5. **save**
   - Saves the spreadsheet to a file.

6. **load**
   - Loads a previously saved spreadsheet from a file.

### iterator Class Functions

- **clear**
  - Clears the content of the current cell.

- **operator***
  - Retrieves the data from the current cell.

- **operator++**
  - Moves the iterator to the right.

- **operator--**
  - Moves the iterator to the left.

- **operator--(int)**
  - Moves the iterator up.

- **operator++(int)**
  - Moves the iterator down.

- **operator!=**
  - Checks if the iterator is not equal to a given node.

- **operator==**
  - Checks if the iterator is equal to a given node.

## Data Structure

- **Node Class:**
  Represents a cell in the spreadsheet.

- **Quad Linked List:**
  A linked list where each node has links to the next, previous, above, and below nodes, forming a 2D grid.

## Usage

The application uses keyboard inputs to perform various actions. Here's a list of key commands and their corresponding functionalities:

- **'I' or 'i':**
  - Inserts data into the current cell.
  - After inserting data, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'(' or '9':**
  - Undoes the last action.
  - If there are multiple undo steps, it performs a full undo.
  - Displays an error message if there are no actions to undo.

- **'O' or 'o':**
  - Opens the command-line interface (CLI) for additional operations.
  - After using CLI, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'D' or 'd':**
  - Inserts a new column to the right of the selected column.
  - After insertion, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'H' or 'h':**
  - Inserts a new column to the right and shifts existing columns to the right.
  - After insertion, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'G' or 'g':**
  - Inserts a new row below the selected row.
  - After insertion, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'F' or 'f':**
  - Deletes the selected column and shifts columns to the left.
  - After deletion, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'T' or 't':**
  - Deletes the selected row and shifts rows up.
  - After deletion, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'A' or 'a':**
  - Inserts a new column to the left of the selected column.
  - After insertion, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'S' or 's':**
  - Inserts a new row above the selected row.
  - After insertion, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'W' or 'w':**
  - Inserts a new row at the top of the sheet.
  - After insertion, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'x' or 'X':**
  - Deletes the selected column.
  - After deletion, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'z' or 'Z':**
  - Deletes the selected row.
  - After deletion, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'v' or 'V':**
  - Clears the selected column (sets all cell data to default).
  - After clearing, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **'c' or 'C':**
  - Clears the selected row (sets all cell data to default).
  - After clearing, use 'SAVE' to save changes and 'Sheet.printer()' to display the updated spreadsheet.

- **Arrow Keys (Left, Right, Up, Down):**
  - Moves the selection in the corresponding direction.
  - Use 'Sheet.sheetLeft()', 'Sheet.sheetRight()', 'Sheet.sheetUp()', 'Sheet.sheetDown()' to navigate.

## Installation

1. **Clone Repository:**
    - `git clone <repository_url> <folder_name>`
2. **Compile Code:**
    - `g++ filename.cpp -o output.exe`
3. **Run Executable:**
    - `./output.exe`
