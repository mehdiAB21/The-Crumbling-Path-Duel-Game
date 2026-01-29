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


struct Player {
    int x = 0, y = 0;   // location of player
    long long score = 0;
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
            if(c == 'X' && board[i][j+1] == 'X'){
                cout << GRAY << "XX" << RESET;
                j++;
                continue;
            }
            else if(c == '+' || c == '-' || c == '|'){
                cout << GRAY << c << RESET;
                continue;
            }
            else if(c == 'A'){
                cout << RED << c << board[i][j+1] << RESET;
                j++;
                continue;
            }
            else if(c == 'B'){
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



//!! TEST:

void PlaceTestPlayers(vector<vector<Cell>>& grid){
    // بازیکن ۱ (A)
    grid[0][0].occupant = 1;   // A1
    grid[1][2].occupant = 2;   // A2
    grid[3][5].occupant = 3;   // A3

    // بازیکن ۲ (B)
    grid[0][5].occupant = -1;  // B1
    grid[2][1].occupant = -2;  // B2
    grid[3][0].occupant = -3;  // B3

    // تست خانه تخریب‌شده
    grid[1][4].destroyed = true;
}




int main(){
    srand(time(0));
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int rows = 6, cols = 10;

    vector<vector<Cell>> grid(rows, vector<Cell>(cols));

    vector<vector<char>> board(4*rows + 1, vector<char>(5*cols + 1, ' '));
    vector<vector<string>> Points(4, vector<string>(6));


    CreateBoard(board, 6, 10);
    RandomPoint(grid, 6, 10);
    PlaceTestPlayers(grid); 
    UpdateBoard(board, grid, rows, cols);
    PrintBoard(board);

    
    return 0;
}