#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
using namespace std;

//-----------Random Number--------

int random(int size)
{
    int R = rand() % size;
    return R;
}

//----------Random Point---------
void RandomPoint(vector<vector<string>>& Points, int row, int column){
 for(int i=0; i<row; i++){
  for(int j=0; j<column; j++){
   int point=random(100) + 1;
   string s;
   if(point/10==0) s="0" + to_string(point);
   else s=to_string(point);
   Points[i][j]=s;
  }
 }
}
//----------Create Board-------
void CreateBoard(vector<vector<char>>& board , int row, int column){
 for(int i=0; i<4*row+1; i++){
  int a= i/4 + 2;
  for(int j=0; j<5*column+1; j++){
   if(i%4==0 && j%5==0) board[i][j]='+';
   else if(i%4==0) board[i][j]='-';
   else if(j%5==0) board[i][j]='|';
   else board[i][j]=' ';
  }
 }
}

//----------Put on Cell-----------
void PutOnCell(vector<vector<char>>& board, int row, int column , string s){
 int i=row*4 + 2;
 int j1=column*5 + 2;
 int j2= j1 + 1;
 char Left=s[0];
 char Right=s[1];
 board[i][j1]=Left;
 board[i][j2]=Right;
}

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
    int occupant = 0;       // empty = 0 | player = 1 or 2 |
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




int main() {
 srand(time(0));
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<vector<char>> board(17, vector<char>(31, ' '));
    vector<vector<string>> Points(4, vector<string>(6));
    CreateBoard(board, 4, 6);
    RandomPoint(Points, 4, 6);
    for(int i=0; i<4; i++){
     for(int j=0; j<6; j++){
      PutOnCell(board, i, j, Points[i][j]);
  }
 }
    for(int i=0; i<17;i++){
     cout<<"\t\t";
     for(int j=0; j<31; j++){
      cout<< board[i][j];
  }
  cout<<endl;
 }
    return 0;
}