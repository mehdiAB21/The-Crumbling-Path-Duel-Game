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
    int id;
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

//----------Calculate Time--------
void ShowTime(time_t Begin){
	int Elapsed=time(nullptr)-Begin;
	int Min=Elapsed/60;
	int Sec=Elapsed%60;
	if(Min<10) {
		if(Sec<10) cout<<"Time: 0"<<Min<<":0"<<Sec;
		else cout<<"Time: 0"<<Min<<":"<<Sec;
	}
	else cout<<"Time: "<<Min<<":"<<Sec;
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
	cout<<endl <<"\t\t  ";
	for(int j=0; j<board[0].size()/5; j++) {
		if(j<10) cout<< j << "    ";
		else cout<< j << "   ";
	}
	cout<< endl << endl ;
    for(int i = 0; i < board.size(); i++){
        if(i%4==2){
    		if((i/4)<10) cout<<"\t     "<< i/4 << "  ";
    		else cout<<"\t     "<< i/4 << " " ;
		}
		else cout<< "\t\t";
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

//-----------Print Header---------
void PrintHeader(time_t Begin, int Round_time, int Score_P1, int Score_P2){
	ShowTime(Begin);
	cout <<" | Turn Player ";
	if(Round_time%2==1)	cout << RED << "1 (P1) | " << RESET;
	else cout << BLUE <<"2 (P2) | " << RESET;
	cout <<"Score P1: "<<Score_P1<<" | Score P2: "<<Score_P2<<endl;
	cout<<"****************************************************************\n";
	cout<<"****************************************************************\n";
}

//----------Place initial A and B-----------
void PlaceInitialPieces_A_then_B(vector<vector<Cell>>& grid, vector<Piece>& piecesA, vector<Piece>& piecesB, int x){
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    piecesA.resize(x);
    piecesB.resize(x);

    // ---- Player A ----
    cout << RED << "\n--- Place Player A pieces ---\n" << RESET;
    for(int i = 0; i < x; ++i){
        while(true){
            int r, c;
            cout << "Place A" << (i + 1) << " (row col): ";
            cin >> r >> c;
            

            if(!IsInside(r,c,rows,cols) || grid[r][c].occupant != 0){
                cout << "Invalid cell. Try again.\n";
                continue;
            }

            grid[r][c].occupant = +(i + 1);
            piecesA[i] = {i + 1 ,r, c};
            break;
        }
    }
    // ---- Player B ----
        cout << BLUE << "\n--- Place Player B pieces ---\n" << RESET;
        for (int i = 0; i < x; i++){
        while (true){
            int r, c;
            cout << "Place B" << (i + 1) << " (row col): ";    
            cin >> r >> c;   
                
            if (!IsInside(r, c, rows, cols) || grid[r][c].occupant != 0) {
                cout << "Invalid cell. Try again.\n";
                continue;
            }
            grid[r][c].occupant = -(i + 1);
            piecesB[i] = {-(i + 1) ,r, c};
            break;    
        }
    }
}

///---------- Check is clear way -----------
bool IsPathClearDir(const vector<vector<Cell>>& grid, int startR, int startC, char dir, int steps)
{
    int dr = 0, dc = 0;

    switch (tolower(dir)) {
        case 'u': dr = -1; break;
        case 'd': dr =  1; break;
        case 'l': dc = -1; break;
        case 'r': dc =  1; break;
        default: return false;
    }

    int r = startR;
    int c = startC;

    for (int i = 1; i <= steps; i++) {
        r += dr;
        c += dc;

        if (!IsInside(r, c, grid.size(), grid[0].size()))
            return false;

        if (grid[r][c].destroyed || grid[r][c].occupant != 0)
            return false;
    }

    return true;
}

//--------- Move Function ----------------
bool MovePieceByDir(vector<vector<Cell>> &grid, Piece &piece, int &score, char dir, int steps){

    if (!IsPathClearDir(grid, piece.r, piece.c, dir, steps)){
        return false;
    }

    int dr = 0, dc = 0;
    switch (tolower(dir)){
        case 'u':
            dr = -1;
            break;
        case 'd':
            dr = 1;
            break;
        case 'l':
            dc = -1;
            break;
        case 'r':
            dc = 1;
            break;
    }

    int destR = piece.r + dr * steps;
    int destC = piece.c + dc * steps;

    // score of goal cell
    score += grid[destR][destC].score;

    // destroy origin cell
    grid[piece.r][piece.c].destroyed = true;
    grid[piece.r][piece.c].occupant = 0;

    // transport player piece
    piece.r = destR;
    piece.c = destC;
    
    grid[destR][destC].occupant = piece.id;

    return true;
}
//--------- Can move? -----------
bool HasAnyMove(const vector<vector<Cell>>& grid, const Piece& p)
{
    const char dirs[4] = {'u', 'd', 'l', 'r'};

    for (char d : dirs) {
        int steps = 1;

        while (true) {
            if (!IsPathClearDir(grid, p.r, p.c, d, steps))
                break;  // Can't move

            return true; // Can move at least one step
        }
    }
    return false;
}

bool IsPlayerTrapped(const vector<vector<Cell>>& grid, const vector<Piece>& pieces)
{
    for (const Piece& p : pieces) {
        if (HasAnyMove(grid, p))
            return false; // At least one piece can move
    }
    return true; // No pieces can move
}



int main()
{
    srand(time(0));
    ios::sync_with_stdio(false);
    
    int round_Num = 1;

    int rows = 6, cols = 10;  // default size😐
    vector<Piece> piecesA, piecesB;
    int scoreA = 0, scoreB = 0; 

    cout << "Enter number of rows: " << endl;
    cin >> rows;
    cout << "Enter number of columns: " << endl;
    cin >> cols;
    clearScreen();
    
    vector<vector<Cell>> grid(rows, vector<Cell>(cols));
    vector<vector<char>> board(4*rows + 1, vector<char>(5*cols + 1, ' '));
    
    int x = 0;
    cout << "Enter x (number of pieces per player, <= 9 recommended): " << endl;
    cin >> x;

    CreateBoard(board, rows, cols);
    PrintBoard(board);
    RandomPoint(grid, rows, cols);
    UpdateBoard(board, grid, rows, cols);
    
    PlaceInitialPieces_A_then_B(grid, piecesA, piecesB, x);
    UpdateBoard(board, grid, rows, cols);
    clearScreen();
    time_t Begin=time(nullptr);
    PrintHeader(Begin, round_Num, scoreA, scoreB);
    PrintBoard(board);
    
    while (true){
        int id, steps;
        char dir;

        if (round_Num % 2 != 0){
            if (IsPlayerTrapped(grid, piecesA)){
                cout << "\nGame Over! Player A has no moves.\n";
                break;
            }
            cout << RED << "Player A: \n" << RESET;
            cout << "Select one of your piece:... ";
            cin >> id;
            id--;

            cout << "Direction (U/D/L/R): ";
            cin >> dir;

            cout << "Steps: ";
            cin >> steps;

            if (!MovePieceByDir(grid, piecesA[id], scoreA, dir, steps)){
                clearScreen();
                PrintHeader(Begin, round_Num, scoreA, scoreB);
                PrintBoard(board);
                cout << "Invalid move!\n";
                continue;
            }
            round_Num++;
            UpdateBoard(board, grid, rows, cols);
            clearScreen();
            PrintHeader(Begin, round_Num, scoreA, scoreB);
            PrintBoard(board);
            cout << endl;
        }
        else if (round_Num % 2 == 0){
            if (IsPlayerTrapped(grid, piecesB)) {
                cout << "\nGame Over! Player B has no moves.\n";
                break;
            }
            cout << BLUE << "Player B: \n" << RESET;
            cout << "Select one of your piece:... ";
            cin >> id;
            id--;

            cout << "Direction (U/D/L/R): ";
            cin >> dir;

            cout << "Steps: ";
            cin >> steps;

            if (!MovePieceByDir(grid, piecesB[id], scoreB, dir, steps)){
                clearScreen();
                PrintHeader(Begin, round_Num, scoreA, scoreB);
                PrintBoard(board);
                cout << "Invalid move!\n";
                continue;
            }
            round_Num++;
            UpdateBoard(board, grid, rows, cols);
            clearScreen();
            PrintHeader(Begin, round_Num, scoreA, scoreB);
            PrintBoard(board);
            cout << endl;
        }
    }

    cout << "\nFinal Scores:\n";
    cout << RED << "Player A: " << scoreA << RESET << endl;
    cout << BLUE << "Player B: " << scoreB << RESET << endl;

    if (scoreA > scoreB)
        cout << RED << "Player A Wins!\n"
             << RESET;
    else if (scoreB > scoreA)
        cout << BLUE << "Player B Wins!\n"
             << RESET;
    else{
        if (round_Num % 2 == 0){
            cout << " Draw! Last player wins, A.\n";
        }else{
            cout << " Draw! Last player wins, B.\n";
        }

    }

    return 0;
}