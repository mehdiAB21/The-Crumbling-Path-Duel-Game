#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <cctype>


using namespace std;

// ---------- Colors ----------
const string RESET = "\x1b[0m";
const string RED   = "\x1b[31m";
const string GREEN = "\x1b[32m";
const string BLUE  = "\x1b[34m";
const string GRAY  = "\x1b[90m";

// ---------- Data Structures ----------
struct Cell {
    int score = 0;
    bool destroyed = false;
    int occupant = 0;   // 0=empty | >0 = A pieces | <0 = B pieces   | ex: 1 = A piece | -1 = B piece
};

struct Piece {
    int r, c;  
};

struct Player {
    long long score = 0;
    vector<Piece> pieces; // اندیس 1..x
};

// ---------- Helpers ----------
void clearScreen() {
    // ANSI clear + cursor home
    cout << "\x1b[2J\x1b[H";
}

//-----------Random Number--------

int random(int size)
{
    int R = rand() % size;
    return R;
}


//----------Random Point---------
//* first random point generator
void RandomPoint(vector<vector<Cell>> &grid, int row, int column)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            int point = random(99) + 1;
            grid[i][j].score = point;
            grid[i][j].destroyed = false;
            grid[i][j].occupant = 0;
        }
    }
}
//----------Create Board-------
void CreateBoard(vector<vector<char>> &board, int row, int column)
{
    for (int i = 0; i < 4 * row + 1; i++)
    {
        int a = i / 4 + 2;
        for (int j = 0; j < 5 * column + 1; j++)
        {
            if (i % 4 == 0 && j % 5 == 0)
                board[i][j] = '+';
            else if (i % 4 == 0)
                board[i][j] = '-';
            else if (j % 5 == 0)
                board[i][j] = '|';
            else
                board[i][j] = ' ';
        }
    }
}


//----------Put on Cell-----------
void PutOnCell(vector<vector<char>>& board, int row, int column, const vector<vector<Cell>>& grid)
{
    int i = row * 4 + 2;
    int j1 = column * 5 + 2;
    int j2 = j1 + 1;
    
    // when destroyed
    if(grid[row][column].destroyed){
        board[i][j1] = 'X';
        board[i][j2] = 'X';
        return;
    }
    
    // when player is on this cell
    if(grid[row][column].occupant != 0){
        int id = abs(grid[row][column].occupant);

        if(id > 9) id = 9; // یا مدیریت خطا/محدودیت

        if(grid[row][column].occupant > 0){
            board[i][j1] = 'A';
            board[i][j2] = char('0' + id);
        }else{
            board[i][j1] = 'B';
            board[i][j2] = char('0' + id);
        }
        return;
    }
    

    int score = grid[row][column].score;
    board[i][j1] = char('0' + (score/10));   // دهگان
    board[i][j2] = char('0' + (score%10));   // یکان
}

//----------Update Board-----------
void UpdateBoard(vector<vector<char>> &board, const vector<vector<Cell>> &grid, int rows, int cols)
{
    CreateBoard(board, rows, cols);

    // put every cell
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            PutOnCell(board, i, j, grid);
        }
    }
}

//----------Print Board-----------
void PrintBoard(const vector<vector<char>>& board){
    for(int i = 0; i < board.size(); i++){
        cout << "\t\t";
        for(int j = 0; j < board[i].size(); j++){
            char c = board[i][j];
            if(c == 'X' && j+1 < (int)board[i].size() && board[i][j+1] == 'X'){
                cout << GRAY << "XX" << RESET;
                j++;
                continue;
            }
            else if(c == '+' || c == '-' || c == '|'){
                cout << GRAY << c << RESET;
                continue;
            }
            else if(c == 'A' && j+1 < (int)board[i].size()){
                cout << RED << c << board[i][j+1] << RESET;
                j++;
                continue;
            }
            else if(c == 'B' && j+1 < (int)board[i].size()){
                cout << BLUE << 'B' << board[i][j+1] << RESET;
                j++;
                continue;
            }
            else if(isdigit(static_cast<unsigned char>(c))){
                cout << c << RESET;
                continue;
            }
            else{
                cout << c;
            }
        }
        cout << '\n';
    }
}

bool IsInside(int r, int c, int rows, int cols){
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

bool CanPlaceHere(const vector<vector<Cell>>& grid, int r, int c){
    return !grid[r][c].destroyed && grid[r][c].occupant == 0;
}

//----------Place initial A and B-----------
void PlaceInitialPieces_A_then_B(vector<vector<Cell>>& grid, vector<Piece>& piecesA, vector<Piece>& piecesB, int x){
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    piecesA.resize(x);
    piecesB.resize(x);

    // ---- Player A ----
    cout << "\n--- Place Player A pieces ---\n";
    for(int i = 0; i < x; ++i){
        while(true){
            int r, c;
            cout << "Place A" << (i + 1) << " (row col): ";
            cin >> r >> c;
            r--; c--;

            if(!IsInside(r,c,rows,cols) || grid[r][c].occupant != 0){
                cout << "Invalid cell. Try again.\n";
                continue;
            }

            grid[r][c].occupant = +(i + 1);
            piecesA[i] = {r, c};
            break;
        }
    }
    // ---- Player B ----
        cout << "\n--- Place Player B pieces ---\n";
        for (int i = 0; i < x; i++)
        {
            while (true)
            {
                int r, c;
                cout << "Place B" << (i + 1) << " (row col): ";
                cin >> r >> c;
                r--;
                c--;

                if (!IsInside(r, c, rows, cols) ||
                    grid[r][c].destroyed ||
                    grid[r][c].occupant != 0)
                {
                    cout << "Invalid cell. Try again.\n";
                    continue;
                }

                grid[r][c].occupant = -(i + 1);
                piecesB[i] = {r, c};
                break;
            }
        }
}

int main(){
    srand(time(0));
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int rows = 6, cols = 10;  // default size
    vector<Piece> piecesA, piecesB;
    
    cout << "Enter number of rows: " << endl;
    cin >> rows;
    cout << "Enter number of columns: " << endl;
    cin >> cols;
    vector<vector<Cell>> grid(rows, vector<Cell>(cols));
    vector<vector<char>> board(4*rows + 1, vector<char>(5*cols + 1, ' '));


    RandomPoint(grid, rows, cols);
    CreateBoard(board, rows, cols);
    UpdateBoard(board, grid, rows, cols);
    PrintBoard(board);

    int x = 0;
    cout << "Enter x (number of pieces per player, <= 9 recommended): " << endl;
    cin >> x;
    cout << "Enter x..." << x << endl;

    


    
    PlaceInitialPieces_A_then_B(grid, piecesA, piecesB, x);
    UpdateBoard(board, grid, rows, cols);
    clearScreen();
    PrintBoard(board);

    
    return 0;
}