#include<iostream>
#include<string>
#include<iomanip>
#include<cstdlib>

using namespace std;

struct connect4{

    connect4(string Names[]){
        re_init(Names);
    }

    connect4(){
        string N[2] = {"P1", "P2"};
        re_init(N);
    }

    void re_init(string Names[]){
        for(int i = 0; i< 7; i++){
            ColumnMoves[i] = 6;
            for(int j = 0; j< 6; j++)
                Data[j][i] = '.'; // 32 is code of space character
        }
        Result = 0;
        GameOver = false;
        PlayerNames[0] = Names[0];
        PlayerNames[1] = Names[1];
        PlayerChar[0] = 'o';
        PlayerChar[1] = 'x';
        PlayerWinningChar[0] = 'O';
        PlayerWinningChar[1] = 'X';
        TurningPlayer = 0;
    }

    int MakeMove(int Column){
    /*
        This Function must make a move in the Column given as a parameter
        The function will use the function GameOver() to mark is the game
        is completed and set the value of variables GameOver and Result.
        The function must also update the array ColumnMoves.

        The function must return 0 if the move has been made, 1 if the move
        is invalid (can not be done as the column is already filled) and 2
        if the game is already over.

        This function must also change the value of TurningPlayer if the move
        was successful;
    */
        Column--;
        if(Column < 0 || Column > 6 || ColumnMoves[Column] < 1)
            return 1;

        if(GameOver)
            return 2;

        ColumnMoves[Column]--;
        Data[ ColumnMoves[Column]][Column] = PlayerChar[TurningPlayer];
        bool Res = CheckIfGameOver(ColumnMoves[Column], Column);

        if(!Res){
            TurningPlayer++;
            TurningPlayer%=2;
        }
    }

    bool isGameOver(){
        return GameOver;
    }

    int WhosTurn(){
        return TurningPlayer;
    }

    string getPlayerName(int id){
        if(id < 0 || id > 1 )
            return "";
        return PlayerNames[id];
    }

    int getResult(){
        return Result;
    }

    char getData(int r, int c){
        if(0 <= r && r < 6 && 0 <= c && c < 8 )
            return Data[r][c];
        return 0;
    }

private:

    bool CheckAndMark(int r, int c, int dr, int dc){
        bool FourPlus = false;
        int Count = 0;
        int i, j;
        for(i = r, j = c ; 0<=i && i < 6 && 0<=j && j < 7 && Data[r][c] == Data[i][j] ; i+=dr, j+=dc)
                Count++;

        for(i = r, j = c ; 0<= i && i < 6 && 0<= j && j< 7 && Data[r][c] == Data[i][j] ; i-=dr, j-=dc)
                Count++;

        if(Count > 4)
           FourPlus = true;

        if(FourPlus){
            for(i = r + dr, j = c + dc ; 0<=i && i < 6 && 0<= j && j < 7 && Data[r][c] == Data[i][j] ; i+=dr, j+=dc)
                    Data[i][j] = PlayerWinningChar[TurningPlayer];

            for(i = r - dr, j = c - dc ; 0 <= i && i < 6 && 0 <= j && j < 7 && Data[r][c] == Data[i][j] ; i-=dr, j-=dc)
                    Data[i][j] = PlayerWinningChar[TurningPlayer];
        }

        return FourPlus;
    }

    bool CheckIfGameOver(int r, int c){
        if(GameOver)
            return GameOver;
        bool Vertical, Horizontal, MainDiagonal, CrossDiagonal;
        Horizontal = Vertical = MainDiagonal = CrossDiagonal = false;

//      Check Vertically downwards
        Vertical = CheckAndMark(r, c, 1, 0);

//        Checking Horizontally
        Horizontal = CheckAndMark(r, c, 0, 1);

//      Check along main diagonal
        MainDiagonal = CheckAndMark(r, c, 1, 1);

//      Check along cross diagonal
        CrossDiagonal = CheckAndMark(r, c, 1, -1);

        if(Horizontal || Vertical || MainDiagonal || CrossDiagonal){
            GameOver = true;
            Result = 2 * TurningPlayer - 1;
            Data[r][c] = PlayerWinningChar[TurningPlayer];
            return GameOver;
        }

        int RemainingMoveCount = 0;
        for(int i = 0; i < 7; i++)
            RemainingMoveCount+= ColumnMoves[i];
        if(RemainingMoveCount == 0)
            GameOver = true;

        return GameOver;
    }

    bool CheckIfGameOverV1(int r, int c){
        /*
        Checking Vertically downwards
        */
        if(GameOver)
            return GameOver;

        bool Vertical, Horizontal, Diagonal;
        Horizontal = Vertical = Diagonal = false;

        if( r <= 2 && (Data[r][c] == Data[r+1][c]) && (Data[r][c] == Data[r+2][c]) && (Data[r][c] == Data[r+3][c]) )
            Vertical = true;

        if(Vertical)
            for(int i = 1; i  < 4 ; i++)
                Data[r+i][c] = PlayerWinningChar[ TurningPlayer];

        /*
        Checking Horizontally
        */

        int H_Count = 0;
        for(int i = c ; i < 7 && Data[r][c] == Data[r][i] ; i++)
                H_Count++;

        for(int i = c; i >= 0 && Data[r][c] == Data[r][i] ; i--)
                H_Count++;

        H_Count--;

        if(H_Count > 3)
                Horizontal = true;

        if(Horizontal){
            for(int i = c+1; i  < 7  && Data[r][i] == Data[r][c]  ; i++)
                Data[r][i] = PlayerWinningChar[ TurningPlayer];

            for(int i = c-1; i  >= 0 && Data[r][i] == Data[r][c]  ; i--)
                Data[r][i] = PlayerWinningChar[ TurningPlayer];
        }
        /*
        Checking along main diagonal \
        */

        int D_Count = 0;
        int i, j;
        for(i = r, j = c ; i < 6 && j < 7 && Data[r][c] == Data[i][j] ; i++, j++)
                D_Count++;

        for(i = r, j = c ; i >= 0 && j >= 0 && Data[r][c] == Data[i][j] ; i--, j--)
                D_Count++;

        D_Count--;
        if(D_Count > 3)
                Diagonal = true;

        if(Diagonal){
            for(i = r+1, j = c+1 ; i < 6 && j < 7 && Data[r][c] == Data[i][j] ; i++, j++)
                    Data[i][j] = PlayerWinningChar[TurningPlayer];

            for(i = r-1, j = c-1 ; i >= 0 && j >= 0 && Data[r][c] == Data[i][j] ; i--, j--)
                    Data[i][j] = PlayerWinningChar[TurningPlayer];
        }

        /*
        Checking along cross diagonal /
        */

        D_Count = 0;
        for(i = r, j = c ; i >= 0 && j < 7 && Data[r][c] == Data[i][j] ; i--, j++)
                D_Count++;

        for(i = r, j = c ; i < 6 && j >= 0 && Data[r][c] == Data[i][j] ; i++, j--)
                D_Count++;

        D_Count--;


        if(D_Count > 3){
            Diagonal = true;
            for(i = r-1, j = c+1 ; i >= 0 && j < 7 && Data[r][c] == Data[i][j] ; i--, j++)
                    Data[i][j] = PlayerWinningChar[TurningPlayer];

            for(i = r+1, j = c-1 ; i < 6 && j >= 0 && Data[r][c] == Data[i][j] ; i++, j--)
                    Data[i][j] = PlayerWinningChar[TurningPlayer];

        }

        if(Horizontal || Vertical || Diagonal){
            GameOver = true;
            Result = 2 * TurningPlayer - 1;
            Data[r][c] = PlayerWinningChar[TurningPlayer];
            return GameOver;
        }

        int RemainingMoveCount = 0;
        for(int i = 0; i < 7; i++)
            RemainingMoveCount+= ColumnMoves[i];
        if(RemainingMoveCount == 0)
            GameOver = true;

        return GameOver;
    }
    string PlayerNames[2]; // To Hold Name of Players Who are playing
    int TurningPlayer; // To save the players ID who is making a move; 0: First Player, 1: Second Player
    char PlayerChar[2]; // Characters to be shown for move
    char PlayerWinningChar[2]; // Character to be shown if player wins.
    char Data[6][7]; // Array to hold game data.
    bool GameOver; // If true it means the game is over.
    int Result;// Variable to hold result of game {0: Drawn, -1: First Player Lost, 1: First Player Won}
    int  ColumnMoves[7]; // An array to keep record of first empty cell in a column
};

struct GameViewer{

    void ShowGame(connect4& GameRef){

        system("cls");
//        BlankLines(50);
        cout<<"\t\t"<< GameRef.getPlayerName(0)<<"\t\tvs\t\t"<<GameRef.getPlayerName(1);
        BlankLines(3);
        for(int  i = 1; i < 8; i++){
            if(i == 1)
                cout<<setw(8)<<i;
            else
                cout<<setw(11)<<i;
        }
        BlankLines(3);

        for(int r = 0; r < 6; r++){
            for(int c = 0 ; c < 7; c++){
                if(c == 0)
                    cout<<setw(2)<<"|";
                cout<<setw(6)<< GameRef.getData(r, c)<<setw(5)<<"|";
            }
            cout<<endl;

            for(int b = 0; r < 5 && b < 2; b++){
                for(int c = 0 ; r<5 && c < 7; c++){
                    if(c == 0)
                        cout<<setw(2)<<"|";
                    cout<<setw(6)<<' '<<setw(5)<<"|";
                }
                cout<<endl;
            }
        }
        for(int i = 0; i< 80; i++)
            cout<<char(220);
        int PID = GameRef.WhosTurn();
        cout<<endl<<"Turn of "<< GameRef.getPlayerName(PID);
        cout<<endl<<endl;
  //      BlankLines(20);
    }

private:
    void BlankLines(int n){
        for(int i = 0;  i < n; i++)
           cout<<"\n";
    }
};

bool isNumber(string S){
    int i, N;
    N = S.length();
    cout<<"HERE "<< N<<endl;
    if(N == 0)
        return false;
    for(i = 0; i < N &&  S[i] == ' '; i++);
    for( ; i < N &&  S[i] >= '0' && S[i]<= '9'; i++);
    for( ; i < N &&  S[i] == ' '; i++);
    if( i < N)
        return false;
    return true;
}


int main(){

    string N[2] = {"Player 1", "Player 2"};
    string Move;
    int Column;
    connect4 GameBord(N);
    GameViewer GameView;

    while(!GameBord.isGameOver()){
        GameView.ShowGame(GameBord);
        cout<<"Enter column number to make a move ";
        getline(cin, Move);
        if( isNumber(Move))
		{
            //Column = stoi(Move);
            cin>>Column;
            GameBord.MakeMove( Column );
        }
    }
    cout<<"Game Over"<<endl;
    GameView.ShowGame(GameBord);
    if(GameBord.getResult())
        cout<<GameBord.getPlayerName(GameBord.WhosTurn())<<" WON THE GAME"<<endl;
    else
        cout<<"Game Dawn";

//    system("pause");
    return 0;
}
