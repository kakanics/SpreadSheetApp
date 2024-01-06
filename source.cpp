#include <iostream>
#include <conio.h>
#include <cmath>
#include <fstream>
#include <Windows.h>
#include <limits>
#include <regex>
#include <vector>
#include <sstream>
#include <stack>

#define NULLSTR                        \
    string s = "";                     \
    for (int i = 0; i < cellSize; i++) \
    {                                  \
        s += " ";                      \
    }
#define RED 4
#define GREEN 2
#define WHITE 15
#define MINIMUM -2147482648
#define MAXIMUM 2147482647
#define SHEETFACTOR 5
#define PRINTLN                            \
    for (int i = 0; i < cellSize + 3; i++) \
        cout << "_";
#define PRINTWL                            \
    cout << "|";                           \
    for (int i = 0; i < cellSize + 2; i++) \
        cout << " ";                       \
    cout << "|";
#define SAVE                         \
    Sheet.save("UndoPowerFile.IDS"); \
    undoButton u;                    \
    u.saveToStream();                \
    undoQ.push(u);

using namespace std;
int cellSize = 4;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // used to color some text
class undoButton
{
public:
    stringstream datastream;
    undoButton() {}
    undoButton(const undoButton &other) : datastream(other.datastream.str()) {}
    undoButton &operator=(const undoButton &other)
    {
        if (this != &other)
        {
            this->datastream.str(other.datastream.str());
        }
        return *this;
    }
    void saveToStream()
    {
        try
        {
            fstream file("UndoPowerFile.IDS", ios::in);
            string s = "";
            while (getline(file, s))
            {
                datastream << s << '\n';
            }
        }
        catch (exception e)
        {
            cout << "Error reading the file";
            getch();
        }
    }
    void saveToFile()
    {
        fstream file("UndoPowerFile.IDS", ios::out);
        file << datastream.str();
    }
};
void gotoXY(int x, int y) // move cursor to required location
{
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}
enum datatypes
{
    integer,
    character,
    floating
};
enum colors
{
    red,
    green,
    white
};
struct coords
{
    int x, y;
};

class Cell
{
private:
    colors color;
    datatypes dataType;
    string data;
    coords *allignment;

public:
    friend class node;
    friend class sheet;

    Cell(int x, int y, datatypes type, string _data)
    {
        allignment = new coords();
        allignment->x = x;
        allignment->y = y;
        dataType = type;
        data = _data;
    }
    Cell()
    {
        allignment = new coords();
        color = white;
        dataType = character;
        NULLSTR
        data = s;
        allignment->x = 0;
        allignment->y = 0;
    }
    int getX()
    {
        return allignment->x;
    }
    int getY()
    {
        return allignment->y;
    }
    string getData()
    {
        return data;
    }
    void setData(string _data)
    {
        string s = "";
        cellSize = cellSize < _data.length() ? _data.length() : cellSize;
        cellSize = cellSize > 10 ? 10 : cellSize;
        for (int i = 0; i < cellSize && i < _data.length(); i++)
        {
            s += _data[i];
        }
        s = regex_replace(s, regex("\\s+$"), "");
        if (regex_match(s, regex("^-?\\d+$")))
        {
            dataType = integer;
            if (s.length() >= 10)
                dataType = character;
        }
        else if (regex_match(s, regex("^-?\\d*\\.\\d+$")))
        {
            dataType = floating;
        }
        else
        {
            dataType = character;
        }
        _data = s;
        if (_data.length() < cellSize)
            for (int i = 0; i < cellSize - _data.length(); i++)
            {
                s += " ";
            }

        data = s;
    }
    void deselect()
    {
        color = white;
    }
    void select()
    {
        color = red;
    }
    int getColorCode()
    {
        return (color == red) ? RED : (color == green) ? GREEN
                                                       : WHITE;
    }
    void clear()
    {
        dataType = character;
        NULLSTR
        data = s;
    }
    bool checkNull()
    {
        for (int i = 0; i < data.length(); i++)
        {
            if (data[i] != ' ')
                return false;
        }
        return true;
    }
};
class node
{
private:
    Cell *cell;
    node *left;
    node *up;
    node *right;
    node *down;

public:
    friend class sheet;
    friend ostream &operator<<(ostream &out, const node &n);
    node()
    {
        left = nullptr;
        up = nullptr;
        right = nullptr;
        down = nullptr;
        cell = new Cell();
    }
    node(Cell *_cell)
    {
        cell = _cell;
        left = nullptr;
        up = nullptr;
        right = nullptr;
        down = nullptr;
    }
    Cell *getCell() { return cell; }
    void locator()
    {
        int t = 0;
        node *temp = this;
        while (temp->up)
        {
            t++;
            temp = temp->up;
        }
        temp = this;
        cell->allignment->y = t;
        t = 0;
        while (temp->left)
        {
            t++;
            temp = temp->left;
        }
        cell->allignment->x = t;
    }
};
ostream &operator<<(ostream &out, const node &n)
{
    out << "0";
    return out;
}
class sheet
{
private:
    node *selectedNode;
    vector<vector<string>> copiedData;

public:
    sheet()
    {
        selectedNode = new node();
        selectedNode->getCell()->select();
        for (int i = 0; i < 4; i++)
        {
            extendRightEnd();
            extendDownEnd();
        }
    }
    int getSheetX()
    {
        node *temp = getTopLeft();
        int i = 0;
        while (temp->right)
        {
            temp = temp->right;
            i++;
        }
        return i;
    }
    int getSheetY()
    {
        node *temp = getTopLeft();
        int i = 0;
        while (temp->down)
        {
            temp = temp->down;
            i++;
        }
        return i;
    }
    void extendRightEnd()
    {
        node *temp = getTopRight();
        while (temp)
        {
            node *newNode = new node();
            temp->right = newNode;
            temp->right->left = temp;
            temp = temp->down;
        }
        temp = getTopRight();
        while (temp->left->down)
        {
            temp->down = temp->left->down->right;
            temp->down->up = temp;
            temp = temp->down;
        }
    }
    void extendDownEnd()
    {
        node *temp = getBottomLeft();
        while (temp)
        {
            node *newNode = new node();
            temp->down = newNode;
            temp->down->up = temp;
            temp = temp->right;
        }
        temp = getBottomLeft();
        while (temp->up->right)
        {
            temp->right = temp->up->right->down;
            temp->right->left = temp;
            temp = temp->right;
        }
    }
    void insertRight()
    {
        node *temp = getTopCurrentRow();
        node *right = temp->right;
        if (!right)
            extendRightEnd();
        else
        {
            while (temp)
            {
                node *newNode = new node();
                temp->right = newNode;
                right->left = newNode;
                newNode->right = right;
                newNode->left = temp;
                temp = temp->down;
                right = right->down;
            }
            temp = getTopCurrentRow();
            while (temp->down)
            {
                temp->right->down = temp->down->right;
                temp->down->right->up = temp->right;
                temp = temp->down;
            }
        }
    }
    void insertColumnLeft()
    {
        node *temp = getTopLeft();
        while (temp)
        {
            node *newnode = new node();
            newnode->right = temp;
            temp->left = newnode;
            temp = temp->down;
        }
        temp = getTopLeft();
        while (temp->right->down)
        {
            temp->down = temp->right->down->left;
            temp->down->up = temp;
            temp = temp->down;
        }
    }
    void insertLeft()
    {
        node *temp = selectedNode;
        if (!selectedNode->left)
            insertColumnLeft();
        else
        {
            selectedNode = selectedNode->left;
            insertRight();
            selectedNode = temp;
        }
    }
    void insertRightShiftFill()
    {
        node *temp = selectedNode->right ? selectedNode->right : selectedNode;
        if (temp == selectedNode)
        {
            extendRightEnd();
            temp = temp->right;
        }
        while (!temp->getCell()->checkNull())
        {
            if (!temp->right)
                extendRightEnd();
            temp = temp->right;
        }
        while (temp != selectedNode)
        {
            temp->getCell()->setData(temp->left->getCell()->getData());
            temp = temp->left;
        }
        selectedNode->cell->clear();
        selectedNode->cell->deselect();
        selectedNode = selectedNode->right;
        selectedNode->cell->select();
    }
    void insertRightShift()
    {
        extendRightEnd();
        node *temp = selectedNode->right;
        while (temp->right)
        {
            temp = temp->right;
        }
        while (temp != selectedNode)
        {
            temp->getCell()->setData(temp->left->getCell()->getData());
            temp = temp->left;
        }
        temp->getCell()->clear();
    }
    void deleteByLeftShift()
    {
        node *temp = selectedNode;
        while (temp && !temp->getCell()->checkNull())
        {
            if (!temp->right)
            {
                temp->getCell()->clear();
                return;
            }
            temp->getCell()->setData(temp->right->getCell()->getData());
            temp = temp->right;
        }
    }
    void deleteByUpShift()
    {
        node *temp = selectedNode;
        while (temp && !temp->getCell()->checkNull())
        {
            if (!temp->down)
            {
                temp->getCell()->clear();
                return;
            }
            temp->getCell()->setData(temp->down->getCell()->getData());
            temp = temp->down;
        }
    }
    void insertDownShiftFill()
    {
        node *temp = selectedNode->down ? selectedNode->down : selectedNode;
        if (temp == selectedNode)
        {
            extendDownEnd();
            temp = temp->down;
        }
        while (!temp->getCell()->checkNull())
        {
            if (!temp->down)
                extendDownEnd();
            temp = temp->down;
        }
        while (temp != selectedNode)
        {
            temp->getCell()->setData(temp->up->getCell()->getData());
            temp = temp->up;
        }
        selectedNode->cell->clear();
        selectedNode->cell->deselect();
        selectedNode = selectedNode->down;
        selectedNode->cell->select();
    }
    void insertDownShift()
    {
        extendDownEnd();
        node *temp = selectedNode->down;
        while (temp->down)
        {
            temp = temp->down;
        }
        while (temp != selectedNode)
        {
            temp->getCell()->setData(temp->up->getCell()->getData());
            temp = temp->up;
        }
        temp->getCell()->clear();
    }

    void insertTopRow()
    {
        node *temp = getBottomLeft();
        while (temp->up)
            temp = temp->up;
        while (temp)
        {
            node *newNode = new node();
            temp->up = newNode;
            temp->up->down = temp;
            temp = temp->right;
        }
        temp = getBottomLeft();
        while (temp->up)
            temp = temp->up;
        while (temp->down->right)
        {
            temp->right = temp->down->right->up;
            temp->right->left = temp;
            temp = temp->right;
        }
    }
    void insertDown()
    {
        node *temp = getLeftCurrentRow();
        node *down = temp->down;
        if (!down)
            extendDownEnd();
        else
        {
            while (temp)
            {
                node *newNode = new node();
                temp->down = newNode;
                down->up = newNode;
                newNode->up = temp;
                newNode->down = down;
                temp = temp->right;
                down = down->right;
            }
            temp = getLeftCurrentRow();
            while (temp->right)
            {
                temp->down->right = temp->right->down;
                temp->down->right->left = temp->down;
                temp = temp->right;
            }
        }
    }
    void deleteColumn()
    {
        if (!selectedNode->left && !selectedNode->right)
            return;
        if (!selectedNode->left)
        {
            selectedNode = selectedNode->right;
            selectedNode->getCell()->select();
            node *temp = selectedNode;
            while (temp->up)
            {
                temp = temp->up;
            }
            while (temp)
            {
                node *nodeToDel = temp->left;
                temp->left = nullptr;
                temp = temp->down;
                delete nodeToDel;
            }
            return;
        }
        selectedNode = selectedNode->left;
        selectedNode->getCell()->select();
        node *temp = selectedNode;
        while (temp->up)
        {
            temp = temp->up;
        }
        while (temp)
        {
            node *nodeToDel = temp->right;
            temp->right = temp->right->right;
            if (temp->right)
                temp->right->left = temp;
            delete nodeToDel;
            temp = temp->down;
        }
    }
    void deleteRow()
    {
        if (!selectedNode->up && !selectedNode->down)
            return;
        if (!selectedNode->up)
        {
            selectedNode = selectedNode->down;
            selectedNode->getCell()->select();
            node *temp = selectedNode;
            while (temp->left)
            {
                temp = temp->left;
            }
            while (temp)
            {
                node *nodeToDel = temp->up;
                temp->up = nullptr;
                temp = temp->right;
                delete nodeToDel;
            }
            return;
        }
        selectedNode = selectedNode->up;
        selectedNode->getCell()->select();
        node *temp = selectedNode;
        while (temp->left)
        {
            temp = temp->left;
        }
        while (temp)
        {
            node *nodeToDel = temp->down;
            temp->down = temp->down->down;
            if (temp->down)
                temp->down->up = temp;
            delete nodeToDel;
            temp = temp->right;
        }
    }
    void insertTop()
    {
        node *temp = selectedNode;
        if (!selectedNode->up)
            insertTopRow();
        else
        {
            selectedNode = selectedNode->up;
            insertDown();
            selectedNode = temp;
        }
    }
    void clearRow()
    {
        node *temp = selectedNode;
        while (temp->left)
            temp = temp->left;
        while (temp)
        {
            temp->cell->clear();
            temp = temp->right;
        }
    }
    void clearColumn()
    {
        node *temp = selectedNode;
        while (temp->up)
            temp = temp->up;
        while (temp)
        {
            temp->cell->clear();
            temp = temp->down;
        }
    }
    node *getTopCurrentRow()
    {
        node *temp = selectedNode;
        while (temp->up)
            temp = temp->up;
        return temp;
    }
    node *getLeftCurrentRow()
    {
        node *temp = selectedNode;
        while (temp->left)
            temp = temp->left;
        return temp;
    }
    node *getTopLeft()
    {
        node *temp = selectedNode;
        while (temp->up)
            temp = temp->up;
        while (temp->left)
            temp = temp->left;
        return temp;
    }
    node *getBottomLeft()
    {
        node *temp = selectedNode;
        while (temp->left)
            temp = temp->left;
        while (temp->down)
            temp = temp->down;
        return temp;
    }
    node *getTopRight()
    {
        node *temp = selectedNode;
        while (temp->up)
            temp = temp->up;
        while (temp->right)
            temp = temp->right;
        return temp;
    }
    void sheetRight()
    {
        selectedNode->cell->deselect();
        printSelectedCell();
        if (!selectedNode->right)
        {
            extendRightEnd();
            selectedNode = selectedNode->right;
            selectedNode->cell->select();
            printer();
        }
        else
        {
            selectedNode = selectedNode->right;
            selectedNode->cell->select();
            printSelectedCell();
        }
    }
    void sheetLeft()
    {
        if (selectedNode->left)
        {
            selectedNode->cell->deselect();
            printSelectedCell();
            selectedNode = selectedNode->left;
            selectedNode->cell->select();
            printSelectedCell();
        }
    }
    void sheetUp()
    {
        if (selectedNode->up)
        {
            selectedNode->cell->deselect();
            printSelectedCell();
            selectedNode = selectedNode->up;
            selectedNode->cell->select();
            printSelectedCell();
        }
    }
    void sheetDown()
    {
        selectedNode->cell->deselect();
        printSelectedCell();
        if (!selectedNode->down)
        {
            extendDownEnd();
            selectedNode = selectedNode->down;
            selectedNode->cell->select();
            printer();
        }
        else
        {
            selectedNode = selectedNode->down;
            selectedNode->cell->select();
            printSelectedCell();
        }
    }
    void printSelectedCellOld()
    {
        node *print = selectedNode;
        print->locator();
        SetConsoleTextAttribute(hConsole, print->cell->getColorCode());
        gotoXY(print->cell->getX() * (cellSize + 3), (print->cell->getY() * 4));
        PRINTLN
        gotoXY(print->cell->getX() * (cellSize + 3), (print->cell->getY() * 4) + 1);
        PRINTWL
        gotoXY(print->cell->getX() * (cellSize + 3), (print->cell->getY() * 4) + 2);
        cout << "| " << print->cell->getData();
        gotoXY(print->cell->getX() * (cellSize + 3), (print->cell->getY() * 4) + 3);
        PRINTWL
        if (!print->down)
        {
            gotoXY(print->cell->getX() * (cellSize + 3), (print->cell->getY() * 4) + 4);
            PRINTLN
        }
    }
    void printSelectedCell()
    {
        node *print = selectedNode;
        int counterX, counterY;
        counterX = counterY = 0;
        node *temp = print;
        while (temp->left && counterX < SHEETFACTOR)
        {
            temp = temp->left;
            counterX++;
        }
        while (temp->up && counterY < SHEETFACTOR)
        {
            temp = temp->up;
            counterY++;
        }
        if (counterX == SHEETFACTOR || counterY == SHEETFACTOR)
        {
            printer();
            return;
        }
        SetConsoleTextAttribute(hConsole, print->cell->getColorCode());
        gotoXY(counterX * (cellSize + 3), (counterY * 4));
        PRINTLN
        gotoXY(counterX * (cellSize + 3), (counterY * 4) + 1);
        PRINTWL
        gotoXY(counterX * (cellSize + 3), (counterY * 4) + 2);
        cout << "| " << print->cell->getData();
        gotoXY(counterX * (cellSize + 3), (counterY * 4) + 3);
        PRINTWL
        if (!print->down)
        {
            gotoXY(counterX * (cellSize + 3), (counterY * 4) + 4);
            PRINTLN
        }
    }
    void printGridOld()
    {
        system("cls");
        node *head = getTopLeft();
        while (head)
        {
            node *temp = head;
            while (temp)
            {
                temp->locator();
                SetConsoleTextAttribute(hConsole, temp->cell->getColorCode());
                gotoXY(temp->cell->getX() * (cellSize + 3), (temp->cell->getY() * 4));
                PRINTLN
                gotoXY(temp->cell->getX() * (cellSize + 3), (temp->cell->getY() * 4) + 1);
                PRINTWL
                gotoXY(temp->cell->getX() * (cellSize + 3), (temp->cell->getY() * 4) + 2);
                PRINTWL
                gotoXY(temp->cell->getX() * (cellSize + 3), (temp->cell->getY() * 4) + 3);
                PRINTWL
                if (!temp->down)
                {
                    gotoXY(temp->cell->getX() * (cellSize + 3), (temp->cell->getY() * 4) + 4);
                    PRINTLN
                }
                temp = temp->right;
            }
            head = head->down;
        }
    }
    void printGrid()
    {
        system("cls");
        node *tempH = selectedNode;
        int countX, countY;
        int countPrintedX, countPrintedY;
        countX = countY = 0;
        while (countX < SHEETFACTOR - 1 && tempH->left)
        {
            tempH = tempH->left;
            countX++;
        }
        while (countY < SHEETFACTOR - 1 && tempH->up)
        {
            tempH = tempH->up;
            countY++;
        }
        countPrintedX = countPrintedY = 0;
        while (tempH && countPrintedY < SHEETFACTOR)
        {
            node *temp = tempH;
            while (temp && countPrintedX < SHEETFACTOR)
            {
                SetConsoleTextAttribute(hConsole, temp->cell->getColorCode());
                gotoXY(countPrintedX * (cellSize + 3), (countPrintedY * 4));
                PRINTLN
                gotoXY(countPrintedX * (cellSize + 3), (countPrintedY * 4) + 1);
                PRINTWL
                gotoXY(countPrintedX * (cellSize + 3), (countPrintedY * 4) + 2);
                PRINTWL
                gotoXY(countPrintedX * (cellSize + 3), (countPrintedY * 4) + 3);
                PRINTWL
                if (!temp->down)
                {
                    gotoXY(countPrintedX * (cellSize + 3), (countPrintedY * 4) + 4);
                    PRINTLN
                }
                temp = temp->right;
                countPrintedX++;
            }
            tempH = tempH->down;
            countPrintedY++;
            countPrintedX = 0;
        }
    }

    void printerOld()
    {
        printGrid();
        node *head = getTopLeft();
        while (head)
        {
            node *temp = head;
            while (temp)
            {
                temp->locator();
                SetConsoleTextAttribute(hConsole, temp->cell->getColorCode());
                gotoXY(temp->cell->getX() * (cellSize + 3), (temp->cell->getY() * 4) + 2);
                cout << "| " << temp->cell->getData();
                temp = temp->right;
            }
            head = head->down;
        }
    }
    void printer()
    {
        printGrid();
        node *tempH = selectedNode;
        int countX, countY;
        int countPrintedX, countPrintedY;
        countX = countY = 0;
        while (countX < SHEETFACTOR && tempH->left)
        {
            tempH = tempH->left;
            countX++;
        }
        while (countY < SHEETFACTOR && tempH->up)
        {
            tempH = tempH->up;
            countY++;
        }
        countPrintedX = countPrintedY = 0;
        while (tempH && countPrintedY < SHEETFACTOR + 1)
        {
            node *temp = tempH;
            while (temp && countPrintedX < SHEETFACTOR + 1)
            {
                SetConsoleTextAttribute(hConsole, temp->cell->getColorCode());
                gotoXY(countPrintedX * (cellSize + 3), (countPrintedY * 4) + 2);
                cout << "| " << temp->cell->getData();
                temp = temp->right;
                countPrintedX++;
            }
            tempH = tempH->down;
            countPrintedY++;
            countPrintedX = 0;
        }
    }
    node *getCell(int x, int y)
    {
        if (x > getSheetX() || y > getSheetY() || x < 0 || y < 0)
            return nullptr;
        node *temp = getTopLeft();
        while (x != 0)
        {
            temp = temp->right;
            x--;
        }
        while (y != 0)
        {
            temp = temp->down;
            y--;
        }
        return temp;
    }
    void enterData()
    {
        string s = "";
        cout << "ENTER THE DATA: ";
        getline(cin, s);
        selectedNode->getCell()->setData(s);
        printer();
    }
    void CLI()
    {
        string s = "";
        cout << "ENTER COMMAND: ";
        cin.clear();
        getline(cin, s);
        vector<string> ops;
        string y = "";
        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] != ' ')
                y += s[i];
            else
            {
                ops.push_back(y);
                y = "";
            }
        }
        ops.push_back(y);
        if (ops[0] == "PASTE")
        {
            PasteData();
        }
        if (ops[0] == "SAVE")
        {
            save(ops[1]);
        }
        if (ops[0] == "LOAD")
        {
            load(ops[1]);
        }
        if (ops.size() != 3)
        {
            return;
        }
        if (ops[0] == "SUM")
        {
            sumFunction(ops[1], ops[2]);
        }
        if (ops[0] == "AVG")
        {
            AverageFunction(ops[1], ops[2]);
        }
        if (ops[0] == "COPY")
        {
            CopyFunction(ops[1], ops[2], false);
        }
        if (ops[0] == "CUT")
        {
            CopyFunction(ops[1], ops[2], true);
        }
        if (ops[0] == "COUNT")
        {
            CountFunction(ops[1], ops[2]);
        }
        if (ops[0] == "MIN")
        {
            MinMax(ops[1], ops[2], true);
        }
        if (ops[0] == "MAX")
        {
            MinMax(ops[1], ops[2], false);
        }
    }
    void sumFunction(string s1, string s2)
    {
        stringstream ss1(s1);
        stringstream ss2(s2);
        string item;
        int x1, y1;
        int x2, y2;

        if (getline(ss1, item, ','))
        {
            x1 = stoi(item);
        }

        if (getline(ss1, item, ','))
        {
            y1 = stoi(item);
        }
        if (getline(ss2, item, ','))
        {
            x2 = stoi(item);
        }

        if (getline(ss2, item, ','))
        {
            y2 = stoi(item);
        }
        if (x1 > x2)
        {
            swap(x1, x2);
        }
        if (y1 > y2)
        {
            swap(y1, y2);
        }

        if ((x1 != x2 && y1 != y2) || (x2 > getSheetX() || y2 > getSheetY()))
        {
            cout << "Cannot Process";
            getch();
            return;
        }
        string direction;
        direction = (x1 == x2) ? "d" : "r";
        iterator iter = getCell(x1, y1);
        float sum = 0;
        if (direction == "d")
        {
            for (int i = 0; i < y2 - y1 + 1; i++)
            {
                if (iter == selectedNode)
                {
                    cout << "cannot process because selected cell in range";
                    getch();
                    return;
                }
                try
                {
                    sum += stof(*iter);
                }
                catch (exception e)
                {
                }
                iter++;
            }
        }
        if (direction == "r")
        {
            for (int i = 0; i < x2 - x1 + 1; i++)
            {
                if (iter == selectedNode)
                {
                    cout << "cannot process because selected cell in range";
                    getch();
                    return;
                }
                try
                {
                    sum += stof(*iter);
                }
                catch (exception e)
                {
                }
                ++iter;
            }
        }
        selectedNode->getCell()->setData(to_string(sum));
    }
    void CountFunction(string s1, string s2)
    {
        stringstream ss1(s1);
        stringstream ss2(s2);
        string item;
        int x1, y1;
        int x2, y2;

        if (getline(ss1, item, ','))
        {
            x1 = stoi(item);
        }

        if (getline(ss1, item, ','))
        {
            y1 = stoi(item);
        }
        if (getline(ss2, item, ','))
        {
            x2 = stoi(item);
        }

        if (getline(ss2, item, ','))
        {
            y2 = stoi(item);
        }
        if (x1 > x2)
        {
            swap(x1, x2);
        }
        if (y1 > y2)
        {
            swap(y1, y2);
        }

        if ((x1 != x2 && y1 != y2) || (x2 > getSheetX() || y2 > getSheetY()))
        {
            cout << "Cannot Process";
            getch();
            return;
        }
        string direction;
        direction = (x1 == x2) ? "d" : "r";
        iterator iter = getCell(x1, y1);
        int count = 0;
        if (direction == "d")
        {
            for (int i = 0; i < y2 - y1 + 1; i++)
            {
                if (iter == selectedNode)
                {
                    cout << "cannot process because selected cell in range";
                    getch();
                    return;
                }
                try
                {
                    stof(*iter);
                    count++;
                }
                catch (exception e)
                {
                }
                iter++;
            }
        }
        if (direction == "r")
        {
            for (int i = 0; i < x2 - x1 + 1; i++)
            {
                if (iter == selectedNode)
                {
                    cout << "cannot process because selected cell in range";
                    getch();
                    return;
                }
                try
                {
                    stof(*iter);
                    count++;
                }
                catch (exception e)
                {
                }
                ++iter;
            }
        }
        getch();
        selectedNode->getCell()->setData(to_string(count));
    }
    void MinMax(string s1, string s2, bool minFlag)
    {
        int min = MAXIMUM;
        int max = MINIMUM;
        stringstream ss1(s1);
        stringstream ss2(s2);
        string item;
        int x1, y1;
        int x2, y2;

        if (getline(ss1, item, ','))
        {
            x1 = stoi(item);
        }

        if (getline(ss1, item, ','))
        {
            y1 = stoi(item);
        }
        if (getline(ss2, item, ','))
        {
            x2 = stoi(item);
        }

        if (getline(ss2, item, ','))
        {
            y2 = stoi(item);
        }
        if (x1 > x2)
        {
            swap(x1, x2);
        }
        if (y1 > y2)
        {
            swap(y1, y2);
        }

        if ((x1 != x2 && y1 != y2) || (x2 > getSheetX() || y2 > getSheetY()))
        {
            cout << "Cannot Process";
            getch();
            return;
        }
        string direction;
        direction = (x1 == x2) ? "d" : "r";
        iterator iter = getCell(x1, y1);
        int count = 0;
        if (direction == "d")
        {
            for (int i = 0; i < y2 - y1 + 1; i++)
            {
                if (iter == selectedNode)
                {
                    cout << "cannot process because selected cell in range";
                    getch();
                    return;
                }
                try
                {
                    min = min < stof(*iter) ? min : stof(*iter);
                    max = max > stof(*iter) ? max : stof(*iter);
                }
                catch (exception e)
                {
                }
                iter++;
            }
        }
        if (direction == "r")
        {
            for (int i = 0; i < x2 - x1 + 1; i++)
            {
                if (iter == selectedNode)
                {
                    cout << "cannot process because selected cell in range";
                    getch();
                    return;
                }
                try
                {
                    min = min < stof(*iter) ? min : stof(*iter);
                    max = max > stof(*iter) ? max : stof(*iter);
                }
                catch (exception e)
                {
                }
                ++iter;
            }
        }
        if (minFlag)
            selectedNode->getCell()->setData(to_string(min));
        else
            selectedNode->getCell()->setData(to_string(max));
    }
    void AverageFunction(string s1, string s2)
    {
        stringstream ss1(s1);
        stringstream ss2(s2);
        string item;
        int x1, y1;
        int x2, y2;

        if (getline(ss1, item, ','))
        {
            x1 = stoi(item);
        }

        if (getline(ss1, item, ','))
        {
            y1 = stoi(item);
        }
        if (getline(ss2, item, ','))
        {
            x2 = stoi(item);
        }

        if (getline(ss2, item, ','))
        {
            y2 = stoi(item);
        }
        if (x1 > x2)
        {
            swap(x1, x2);
        }
        if (y1 > y2)
        {
            swap(y1, y2);
        }

        if ((x1 != x2 && y1 != y2) || (x2 > getSheetX() || y2 > getSheetY()))
        {
            cout << "Cannot Process";
            getch();
            return;
        }
        string direction;
        direction = (x1 == x2) ? "d" : "r";
        iterator iter = getCell(x1, y1);
        float sum = 0;
        if (direction == "d")
        {
            for (int i = 0; i < y2 - y1 + 1; i++)
            {
                if (iter == selectedNode)
                {
                    cout << "cannot process because selected cell in range";
                    getch();
                    return;
                }
                try
                {
                    sum += stof(*iter);
                }
                catch (exception e)
                {
                }
                iter++;
            }
        }
        if (direction == "r")
        {
            for (int i = 0; i < x2 - x1 + 1; i++)
            {
                if (iter == selectedNode)
                {
                    cout << "cannot process because selected cell in range";
                    getch();
                    return;
                }
                try
                {
                    sum += stof(*iter);
                }
                catch (exception e)
                {
                }
                ++iter;
            }
        }
        getch();
        sum /= (x1 == x2) ? (y2 - y1 + 1) : (x2 - x1 + 1);
        selectedNode->getCell()->setData(to_string(sum));
    }
    void CopyFunction(string s1, string s2, bool flag)
    {
        stringstream ss1(s1);
        stringstream ss2(s2);
        string item;
        int x1, y1;
        int x2, y2;

        if (getline(ss1, item, ','))
        {
            x1 = stoi(item);
        }

        if (getline(ss1, item, ','))
        {
            y1 = stoi(item);
        }
        if (getline(ss2, item, ','))
        {
            x2 = stoi(item);
        }

        if (getline(ss2, item, ','))
        {
            y2 = stoi(item);
        }
        if (x2 > getSheetX() || y2 > getSheetY() || x1 > x2 || y1 > y2)
        {
            cout << "Cannot Process";
            getch();
            return;
        }
        copiedData.clear();
        for (int i = y1; i < y2 + 1; i++)
        {
            iterator iter = getCell(x1, i);
            vector<string> data;
            for (int x = x1; x < x2 + 1; x++)
            {
                data.push_back(*iter);
                if (flag)
                    iter.clear();
                ++iter;
            }
            copiedData.push_back(data);
        }
    }

    void PasteData()
    {
        if (copiedData.empty())
            return;
        node *temp = selectedNode;
        for (int i = 0; i < copiedData.size(); i++)
        {
            vector<string> data = copiedData[i];
            node *temp2 = temp;
            for (int j = 0; j < data.size(); j++)
            {
                temp2->getCell()->setData(data[j]);
                if (!temp2->right && j + 1 < data.size())
                    extendRightEnd();
                temp2 = temp2->right;
            }
            if (!temp->down && i + 1 < copiedData.size())
                extendDownEnd();
            temp = temp->down;
        }
    }
    void save(string filename)
    {
        fstream file(filename, ios ::out);
        node *temp = getTopLeft();
        while (temp)
        {
            node *rowTemp = temp;
            while (rowTemp)
            {
                file << rowTemp->cell->data;
                file << (char)200;
                rowTemp = rowTemp->right;
            }
            file << "\n";
            temp = temp->down;
        }
    }
    void load(string filename)
    {
        try
        {
            fstream file(filename, ios::in);

            selectedNode = new node();
            selectedNode->cell->select();
            node *temp = selectedNode;
            string s;
            while (getline(file, s))
            {
                node *tempRow = temp;
                string cellData = "";
                for (int i = 0; i < s.length(); i++)
                {
                    if (s[i] == (char)200)
                    {
                        tempRow->cell->setData(cellData);
                        cellData = "";
                        if (!tempRow->right)
                            extendRightEnd();
                        tempRow = tempRow->right;
                    }
                    else
                    {
                        cellData += s[i];
                    }
                }
                extendDownEnd();
                temp = temp->down;
            }
            selectedNode = getTopRight();
            deleteColumn();
            selectedNode = getBottomLeft();
            deleteRow();
            selectedNode = getTopLeft();
            getTopRight()->cell->deselect();
            getBottomLeft()->cell->deselect();
            printer();
        }
        catch (exception e)
        {
            cout << "Error reading the file";
            getch();
        }
    }

    class iterator
    {
    private:
        node *curr;

    public:
        iterator(node *i)
        {
            curr = i;
        }
        void clear()
        {
            curr->getCell()->clear();
        }
        string operator*()
        {
            return curr->getCell()->getData();
        }
        void operator++()
        {
            if (!curr)
                return;
            curr = curr->right;
        }
        void operator--()
        {
            if (!curr)
                return;
            curr = curr->left;
        }
        void operator--(int)
        {
            if (!curr)
                return;
            curr = curr->up;
        }
        void operator++(int)
        {
            if (!curr)
                return;
            curr = curr->down;
        }
        bool operator!=(node *n)
        {
            return curr != n;
        }
        bool operator==(node *n)
        {
            return curr == n;
        }
    };
};

main()
{
    sheet Sheet;
    stack<undoButton> undoQ;
    Sheet.printer();
    bool movementFlag = true;
    SAVE while (true)
    {
        int key = _getch();
        if (key == 'I' || key == 'i')
        {
            Sheet.enterData();
            SAVE
                Sheet.printer();
        }
        if (key == '(' || key == '9')
        {
            if (!undoQ.empty())
            {
                undoQ.pop();
            }
            if (!undoQ.empty())
            {
                undoButton u;
                u = undoQ.top();
                u.saveToFile();
                Sheet.load("UndoPowerFile.IDS");
            }
            else
            {
                cout << "can't undo";
                Sleep(1000);
            }
        }
        if (key == 'O' || key == 'o')
        {
            Sheet.CLI();
            SAVE
                Sheet.printer();
        }
        if (key == 'D' || key == 'd')
        {
            Sheet.insertRight();
            SAVE
                Sheet.printer();
        }
        if (key == 'H' || key == 'h')
        {
            Sheet.insertRightShift();
            SAVE
                Sheet.printer();
        }
        if (key == 'G' || key == 'g')
        {
            Sheet.insertDownShift();
            SAVE
                Sheet.printer();
        }
        if (key == 'F' || key == 'f')
        {
            Sheet.deleteByLeftShift();
            SAVE
                Sheet.printer();
        }
        if (key == 'T' || key == 't')
        {
            Sheet.deleteByUpShift();
            SAVE
                Sheet.printer();
        }
        if (key == 'A' || key == 'a')
        {
            Sheet.insertLeft();
            SAVE
                Sheet.printer();
        }
        if (key == 'S' || key == 's')
        {
            Sheet.insertDown();
            SAVE
                Sheet.printer();
        }
        if (key == 'W' || key == 'w')
        {
            Sheet.insertTop();
            SAVE
                Sheet.printer();
        }
        if (key == 'x' || key == 'X')
        {
            Sheet.deleteColumn();
            SAVE
                Sheet.printer();
        }
        if (key == 'z' || key == 'Z')
        {
            Sheet.deleteRow();
            SAVE
                Sheet.printer();
        }
        if (key == 'v' || key == 'V')
        {
            Sheet.clearColumn();
            SAVE
                Sheet.printer();
        }
        if (key == 'c' || key == 'C')
        {
            Sheet.clearRow();
            SAVE
                Sheet.printer();
        }
        else if (key == 224)
        {
            int arrowKey = _getch();
            if (arrowKey == 75)
            {
                // Left arrow key
                Sheet.sheetLeft();
            }
            else if (arrowKey == 77)
            {
                // Right arrow key
                Sheet.sheetRight();
            }
            else if (arrowKey == 72)
            {
                // Up arrow key
                Sheet.sheetUp();
            }
            else if (arrowKey == 80)
            {
                // Down arrow key
                Sheet.sheetDown();
            }

            // Sleep(20);
        }
    }
}
