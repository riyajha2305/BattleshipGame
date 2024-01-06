#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
using namespace std;

class BattleshipPlayer {
private:
    vector<vector<char> > grid;
    map<char, int> shipSquares;

public:
    BattleshipPlayer(){
        grid = vector<vector<char> > (10, vector<char>(10, '.'));
        shipSquares = {{'A',5}, {'B',4}, {'D',3}, {'C',2}, {'F',1}};
    }

    void printGrid() {
        cout << "  A B C D E F G H I J" << "\n";
        for (int i = 0; i < 10; i++) {
            cout << setw(2) << i + 1;
            for (int j = 0; j < 10; j++) {
                cout << grid[i][j] << ' ';
            }
            cout << "\n";
        }
    }

    void placeShip(int row, int col, char dir, int n) {
        map<int,char> ship = {{5,'A'}, {4,'B'}, {3,'D'}, {2,'C'}, {1,'F'}};
        if(dir == 'H'){
            for(int j = col; j <= (col+n-1); j++){
                grid[row][j] = ship[n];
            }
        }
        else if(dir == 'V'){
            for(int j = row; j <= (row+n-1); j++){
                grid[j][col] = ship[n];
            }
        }
    }

    bool isCellValid(int row, int col, char dir, int n){
        if(row < 0 | row > 9 | col < 0 | col > 9) return false;
        if(dir == 'H'){
            for(int j = col; j <= (col+n-1); j++){
                if(j < 0 | j > 9) return false;
                if(grid[row][j] != '.') return false;
            }
        }
        else if(dir == 'V'){
            for(int j = row; j <= (row+n-1); j++){
                if(j < 0 | j > 9) return false;
                if(grid[j][col] != '.') return false;
            }
        }
        return true;
    }

    char getCell(int row, int col) const {
        return grid[row][col];
    }

    void setCell(int row, int col, char value) {
        grid[row][col] = value;
    }

    bool find(char cell){
        if(shipSquares.find(cell) != shipSquares.end()){
            return true;
        }
        return false;
    }

    int shipsLeft(char cell){
        shipSquares[cell]--;
        if(shipSquares[cell] == 0) {
            shipSquares.erase(cell);
            return 0;
        }
        return shipSquares[cell];
    }

    bool allShipsSunk(){
        return (shipSquares.empty());
    }
};

class PlanningPhase {
private:
    BattleshipPlayer &battleshipPlayer;

public:

    PlanningPhase(BattleshipPlayer &battleshipplayer) : battleshipPlayer(battleshipplayer) {}

    void placeShip(int n){

        if(n == 0) return;

        vector<string> ship = {"Aircraft carrier", "Battleship", "Destroyer", "Corvette", "Frigate"};
        int startRow, startCol;
        cout << "Enter starting row for ship " << ship[5-n] << " of length " << n << ": ";
        cin >> startRow;
        cout << "Enter starting column for ship " << ship[5-n] << " (A-J) " << "of length " << n << ": ";
        char colChar;
        cin >> colChar;
        startCol = colChar - 'A';
        char direction;
        cout << "Place ship Horizontally (H) or Vertically (V) ? (Press H/V) : ";
        cin >> direction;

        if (battleshipPlayer.isCellValid(startRow - 1, startCol, direction, n)) {
            battleshipPlayer.placeShip(startRow - 1, startCol, direction, n);
            cout << "\n";
            battleshipPlayer.printGrid();
            cout << "\n";
            placeShip(n-1);
        } else {
            cout << "Invalid co-ordinate. Try Again !" << "\n";
            placeShip(n);
        }
    }
};

class GuessingPhase {
private:
    const BattleshipPlayer &targetPlayer;
    BattleshipPlayer &opponentPlayer;

public:
    GuessingPhase(const BattleshipPlayer &target, BattleshipPlayer &opponent) : targetPlayer(target), opponentPlayer(opponent) {}

    pair<int, int> askInput(){
        int row, col;
        cout << "Enter a row (1-10): ";
        cin >> row;
        cout << "Enter a column (A-J): ";
        char colChar;
        cin >> colChar;
        col = colChar - 'A';

        return {row - 1, col};
    }

    void guess(int row, int col) {
        char opponentGridCell = opponentPlayer.getCell(row, col);
        if (opponentGridCell == 'H' || opponentGridCell == '*') {
            cout << "----------------------------------------------" << "\n";
            cout << "| Already guessed this position. Try again ! |" << "\n";
            cout << "----------------------------------------------" << "\n";
            cout << "\n";
            pair<int, int> coord = askInput();
            guess(coord.first, coord.second);
        } else if (opponentPlayer.find(opponentGridCell)) {
            if(opponentPlayer.shipsLeft(opponentGridCell) > 0) {
                cout << "\n";
                cout << "---------" << "\n";
                cout << "| HIT ! |" << "\n";
                cout << "---------" << "\n";
                cout << "\n";
            }
            else {
                cout << "\n";
                cout << "----------------" << "\n";
                cout << "| HIT & SUNK ! |" << "\n";
                cout << "----------------" << "\n";
                cout << "\n";
            }
            opponentPlayer.setCell(row, col, 'H');
        } else {
                cout << "\n";
                cout << "----------" << "\n";
                cout << "| MISS ! |" << "\n";
                cout << "----------" << "\n";
                cout << "\n";
            opponentPlayer.setCell(row, col, '*');
        }
    }

    void printOpponentGrid(){
        cout << "  A B C D E F G H I J" << "\n";
        for (int i = 0; i < 10; i++) {
            cout << setw(2) << i + 1;
            for (int j = 0; j < 10; j++) {
                char opponentGridCell = opponentPlayer.getCell(i, j);
                if(opponentPlayer.find(opponentGridCell)) {
                    cout << '.' << ' ';
                } else {
                    cout << opponentGridCell << ' ';
                }
            }
            cout << "\n";
        }
    }
};

class BattleshipGame {
private:
    BattleshipPlayer player1;
    BattleshipPlayer player2;

public:

    void play() {

        cout << "---------------------------" << "\n";
        cout << "| Welcome to Battleship ! |" << "\n";
        cout << "---------------------------" << "\n";
        cout << "\n";

        cout << "-----------------------------------" << "\n";
        cout << "| Initial status of Player 1 grid |" << "\n";
        cout << "-----------------------------------" << "\n";
        cout << "\n";
        player1.printGrid();
        cout << "\n";
        cout << "-----------------------------------" << "\n";
        cout << "| Initial status of Player 2 grid |" << "\n";
        cout << "-----------------------------------" << "\n";
        cout << "\n";
        player2.printGrid();
        cout << "\n";

        PlanningPhase planning1(player1);
        PlanningPhase planning2(player2);

        cout << "-----------------------------" << "\n";
        cout << "| Player 1 place your ships |" << "\n";
        cout << "-----------------------------" << "\n";
        cout << "\n";
        planning1.placeShip(5);
        cout << "\n";
        cout << "-----------------------------" << "\n";
        cout << "| Player 2 place your ships |" << "\n";
        cout << "-----------------------------" << "\n";
        cout << "\n";
        planning2.placeShip(5);
        cout << "\n";

        GuessingPhase guessing1(player1, player2);
        GuessingPhase guessing2(player2, player1);

        cout << "--------------" << "\n";
        cout << "| GAME START |" << "\n";
        cout << "--------------" << "\n";
        cout << "\n";

        bool gameOver = false;
        int currentPlayer = 1;

        while (!gameOver) {
            cout << "\n";
            cout << "--------------------------------------" << "\n";
            cout << "| Player " << currentPlayer << " guess the opponent's ship |" << "\n";
            cout << "--------------------------------------" << "\n";
            cout << "\n";
            cout << "---------------------------" << "\n";
            cout << "| Current status of Grids |" << "\n";
            cout << "---------------------------" << "\n";
            cout << "\n";
            if(currentPlayer == 1){
                cout << "-------------------" << "\n";
                cout << "| Player " << currentPlayer << "'s Grid |" << "\n";
                cout << "-------------------" << "\n";
                cout << "\n";
                player1.printGrid();
                cout << "\n";
                cout << "----------------------" << "\n";
                cout << "| Player " << currentPlayer << "'s Guesses |" << "\n";
                cout << "----------------------" << "\n";
                cout << "\n";
                guessing2.printOpponentGrid();
                cout << "\n";
            } else {
                cout << "-------------------" << "\n";
                cout << "| Player " << currentPlayer << "'s Grid |" << "\n";
                cout << "-------------------" << "\n";
                cout << "\n";
                player2.printGrid();
                cout << "\n";
                cout << "----------------------" << "\n";
                cout << "| Player " << currentPlayer << "'s Guesses |" << "\n";
                cout << "----------------------" << "\n";
                cout << "\n";
                cout << "Opponent's Grid:" << "\n";
                guessing2.printOpponentGrid();
                cout << "\n";
            }

            pair<int, int> coord;

            if(currentPlayer == 1) coord = guessing1.askInput();
            else coord = guessing2.askInput();

            if (currentPlayer == 1) guessing1.guess(coord.first, coord.second);
            else guessing2.guess(coord.first, coord.second);

            cout << "------------------------------------------" << "\n";
            cout << "| Status of grids after Player " << currentPlayer << "'s moves |" << "\n";
            cout << "------------------------------------------" << "\n";
            cout << "\n";
            
            if(currentPlayer == 1){
                cout << "-------------------" << "\n";
                cout << "| Player " << currentPlayer << "'s Grid |" << "\n";
                cout << "-------------------" << "\n";
                cout << "\n";
                player1.printGrid();
                cout << "\n";
                cout << "----------------------" << "\n";
                cout << "| Player " << currentPlayer << "'s Guesses |" << "\n";
                cout << "----------------------" << "\n";
                cout << "\n";
                guessing1.printOpponentGrid();
                cout << "\n";
            } else {
                cout << "-------------------" << "\n";
                cout << "| Player " << currentPlayer << "'s Grid |" << "\n";
                cout << "-------------------" << "\n";
                cout << "\n";
                player2.printGrid();
                cout << "\n";
                cout << "----------------------" << "\n";
                cout << "| Player " << currentPlayer << "'s Guesses |" << "\n";
                cout << "----------------------" << "\n";
                cout << "\n";
                guessing2.printOpponentGrid();
                cout << "\n";
            }

            gameOver = player1.allShipsSunk() | player2.allShipsSunk();

            currentPlayer = (currentPlayer == 1) ? 2 : 1;
        }

        cout << "---------------" << "\n";
        cout << "| GAME OVER ! |" << "\n";
        cout << "---------------" << "\n";
        cout << "\n";

        cout << "------------------" << "\n";
        cout << "| PLAYER " << currentPlayer << " WON ! |" << "\n";
        cout << "------------------" << "\n";
        cout << "\n";

        
    }
};

int main() {
    BattleshipGame game;
    game.play();

    return 0;
}
