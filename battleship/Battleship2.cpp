#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <string>

using namespace std;

#define GRID_SIZE 10

unordered_map<string,int> shipLengths = {
    {"Aircraft Carrier", 5},
    {"Battleship", 4},
    {"Destroyer", 3},
    {"Corvette", 2},
    {"Frigate", 1}
};

void printGrid(vector<vector<char>> &grid){
    cout << "   A B C D E F G H I J" << "\n";
    for (int i = 0; i < GRID_SIZE; i++) {
        cout << setw(2) << i + 1 << ' ';
        for (int j = 0; j < GRID_SIZE; j++) {
            cout << grid[i][j] << ' ';
        }
        cout << "\n";
    }
    cout<<"\n";
}

class BattleshipPlayer {
    vector<vector<char>> board, guess;
    unordered_map<string,int> shipHits;

    bool placeShip(int x, int y, int dx, int dy, string ship){
        int i = x, j = y;
        while((i-x) + (j-y) < shipLengths[ship]){
            if(i >= GRID_SIZE || j >= GRID_SIZE || board[i][j] != '.')return false;
            i += dx;
            j += dy;
        }
        i = x, j = y;
        while((i-x) + (j-y) < shipLengths[ship]){
            board[i][j] = ship[0];
            i += dx;
            j += dy;
        }
        return true;
    }

    public:
    string name;

    BattleshipPlayer(string name){
        this->name = name;
        board = vector<vector<char>>(GRID_SIZE, vector<char>(GRID_SIZE, '.'));
        guess = vector<vector<char>>(GRID_SIZE, vector<char>(GRID_SIZE, '.'));
        for(auto [ship, length]: shipLengths){
            shipHits[ship] = 0;
        }
        printGrid(board);
    }

    void plan(){
        char x, y, dir;
        for(auto [ship, length]: shipLengths){
            bool placed = false;
            while(!placed){
                cout<<"Place "<<ship<<"\n";
                cout<<"\n";
                cout<<"Enter Initial Coordinate Row (1-10) Column (A-J):\n";
                cin>>x>>y;
                cout<<"Direction Horizontal (H) or Vertical (V):\n";
                cin>>dir;
                int dx = dir == 'V' ? 1 : 0, dy = dir == 'H' ? 1 : 0;
                placed = placeShip(x-'1', y-'A', dx, dy, ship);
                if(placed){
                    cout<<"\n"<<ship<<" placed successfully.\n";
                } else {
                    cout<<"\nFailed ! Place again.\n";
                }
            }
            cout << "\n";
            printGrid(board);
        }
    }

    bool validGuess(int x, int y){
        if(max(x, y) < GRID_SIZE && min(x, y) >= 0 && guess[x][y] == '.')return true;
        return false;
    }

    char opponentGuess(int x, int y){
        unordered_map<char, string> shipNames = {
            {'A',"Aircraft Carrier"},
            {'B',"Battleship"},
            {'D',"Destroyer"},
            {'C',"Corvette"},
            {'F',"Frigate"}
        };

        if(board[x][y] == '.')return '*';
        char ship = board[x][y];
        string shipName = shipNames[ship];
        shipHits[shipName]++;
        if(shipHits[shipName] == shipLengths[shipName])return ship;
        return 'H';
    }

    void updateGuess(int x, int y, char c){
        guess[x][y] = c;

        cout << "-------------" << "\n";
        cout << "| Your Grid |" << "\n";
        cout << "-------------" << "\n";
        cout << "\n";
        
        printGrid(board);

        cout << "\n";
        cout << "-------------------" << "\n";
        cout << "| Opponent's Grid |" << "\n";
        cout << "-------------------" << "\n";
        cout << "\n";

        printGrid(guess);

        cout << "\n";
    }
};

void playPlayer(BattleshipPlayer *player, BattleshipPlayer *opponent, int hitPlayer, int hitOpponent){
    cout<<player->name<<" guess a coordinate:\n";
    char a, b;
    cin>>a>>b;
    int x = a-'1', y = b-'A';
    if(!player->validGuess(x, y)){
        cout<<"\n";
        cout<<"-----------------------------"<<"\n";
        cout<<"| Invalid guess. Try again !|"<<"\n";
        cout<<"-----------------------------"<<"\n";
        cout<<"\n";
        playPlayer(player, opponent, hitPlayer, hitOpponent);
        return;
    }
    char output = opponent->opponentGuess(x, y);
    
    if(output != 'H' && output != '*'){
        cout << "\n";
        cout << "----------------" << "\n";
        cout << "| HIT & SUNK ! |" << "\n";
        cout << "----------------" << "\n";
        cout << "\n";
        cout<<"Ship "<<output<<" of "<<opponent->name<<" drowned.\n";
        cout<<"\n";
        player->updateGuess(x, y, 'H');
        hitPlayer++;
    } else if(output == '*'){
        cout << "\n";
        cout << "----------" << "\n";
        cout << "| MISS ! |" << "\n";
        cout << "----------" << "\n";
        cout << "\n";
        player->updateGuess(x, y, '*');
    } else {
        cout << "\n";
        cout << "---------" << "\n";
        cout << "| HIT ! |" << "\n";
        cout << "---------" << "\n";
        cout << "\n";
        player->updateGuess(x, y, 'H');
    }
    if(hitPlayer == shipLengths.size()){

        cout << "---------------" << "\n";
        cout << "| GAME OVER ! |" << "\n";
        cout << "---------------" << "\n";
        cout << "\n";

        cout<<"--------------------" << "\n";
        cout<<"| "<<player->name<<" WON !!! |\n";
        cout<<"--------------------" << "\n";

        return;
    }
    playPlayer(opponent, player, hitOpponent, hitPlayer);
}

class BattleshipGame {
    BattleshipPlayer *player1, *player2;

    public:

    BattleshipGame(){
        player1 = new BattleshipPlayer("Player 1");
        player2 = new BattleshipPlayer("Player 2");
    }

    void play(){

        cout << "---------------------------" << "\n";
        cout << "| Welcome to Battleship ! |" << "\n";
        cout << "---------------------------" << "\n";
        cout << "\n";

        cout << "-----------------------------" << "\n";
        cout << "| Player 1 place your ships |" << "\n";
        cout << "-----------------------------" << "\n";
        cout << "\n";

        player1->plan();

        cout << "\n";
        cout << "-----------------------------" << "\n";
        cout << "| Player 2 place your ships |" << "\n";
        cout << "-----------------------------" << "\n";
        cout << "\n";

        player2->plan();

        cout << "\n";
        
        cout << "--------------" << "\n";
        cout << "| GAME START |" << "\n";
        cout << "--------------" << "\n";
        cout << "\n";

        playPlayer(player1, player2, 0, 0);
    }
};

int main(){
    BattleshipGame game;
    game.play();
    return 0;
}