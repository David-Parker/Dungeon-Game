#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>

using namespace std;

#define UP 119 /* W */
#define RIGHT 100 /* D */
#define DOWN 115 /* S */
#define LEFT 97 /* A */
#define STOP 113 /* Q */
#define NUM_ROWS 50
#define NUM_COLS 75

#define RANDOM false

void clrscr();

/* Game board */
class Board {
private:
	static const int rows = NUM_ROWS;
	static const int cols = NUM_COLS;
	static const int numObstacles = 2100;
	char board[rows][cols];

	bool fillBoard() {
	/* Set everything to ground pieces */
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			setSlot(ground,i,j);
		}
	}

	/* Create some obstacles */
	for(int i = 0; i < numObstacles; i++) {
		setSlot(wall, rand() % rows, rand() % cols);
	}

	/* Create the portal */
	if(RANDOM) setSlot(portal, rand() % rows, rand() % cols);
	else setSlot(portal, rows - 2, cols - 2);

	return true;
}

public:
	/* Board Pieces */
	static const char ground = '_';
	static const char wall 	 = '|';
	static const char portal = 'O';
	static const char player = '0';

	Board() : board() {
		fillBoard();
	}
	~Board() {}

	void printBoard() {
		string wholeBoard = "";
		for(int i = 0; i < rows; i++) {
			for(int j = 0; j < cols; j++) {
				wholeBoard += board[i][j];
			}
			wholeBoard += "\n";
		}
		cout << wholeBoard;
	}

	void setSlot(const char c, const int x, const int y) {
		board[x][y] = c;
	}

	void setPlayerPosition(const int x, const int y) {
		setSlot(player, x, y);
	}

	char getObject(const int x, const int y) {
		return board[x][y];
	}

	const int getXSize() { 
		const int retval = rows - 1;
		return retval; 
	}
	const int getYSize() { 
		const int retval = cols - 1;
		return retval; 
	}

};

/* Creates a new player with a starting position */
class Player {
public:
	Player(const int startX, const int startY) : posX(startX), posY(startY) {}

	~Player() {}

	void move(Board& b, const int x, const int y) {
		b.setSlot(Board::ground, posX, posY);
		posX += x;
		posY += y;
		b.setSlot(Board::player, posX, posY);
	}

	int getX() { return posX; }
	int getY() { return posY; }

private:
	int posX;
	int posY;
};

class Game {
public:
	Game() : play(true), board() {}
	~Game() {}

	void attemptMove(const int x, const int y) {
		int newX = player->getX() + x;
		int newY = player->getY() + y;
		/* Boundary check */
		if(	newX > board.getXSize() || 
			newY > board.getYSize() ||
			newX < 0 ||
			newY < 0) {
				/* Do not move */
				return;
		}

		if(board.getObject(newX,newY) == Board::wall)
			return;

		else if (board.getObject(newX,newY) == Board::portal)
			play = false;

		else { player->move(board,x,y); }
	}

	void checkCommand(int input) {
		if(input == UP) {
			attemptMove(-1,0);
		}
		else if(input == DOWN) {
			attemptMove(1,0);
		}
		else if(input == RIGHT) {
			attemptMove(0,1);
		}
		else if(input == LEFT) {
			attemptMove(0,-1);
		}
		else if(input == STOP) {
			exit(0);
		}
	}

	bool validMove(int x, int y) {
		if(	x > board.getXSize() || 
			y > board.getYSize() ||
			x < 0 ||
			y < 0) {
				/* Do not move */
				return false;
		}

		if(board.getObject(x,y) == Board::wall) return false;

		return true;
	}

	void findSolution(bool& found, int x, int y) {
		if(board.getObject(x,y) == Board::portal) {
			found = true;
			return;
		}

		if(hasVisited[x][y] == true) return;
		hasVisited[x][y] = true;

		if(!found) {
			if(validMove(x + 1, y)) findSolution(found, x + 1, y);
			if(validMove(x, y + 1)) findSolution(found, x, y + 1);
			if(validMove(x - 1, y)) findSolution(found, x - 1, y);
			if(validMove(x, y - 1)) findSolution(found, x, y - 1);
		}

	}

	bool hasASolution() {
		bool found = false;
		/* Set has Visited array to false */
		for(int i = 0; i < board.getXSize(); i++) {
			for(int j = 0; j < board.getYSize(); j++)
				hasVisited[i][j] = false;
		}

		findSolution(found, player->getX(), player->getY());

		return found;
	}

	void gameLoop() {

		/* Find a valid starting point */
		while(1) {
			int xPlayerStart = 0;
			int yPlayerStart = 0;

			if(RANDOM) {
				xPlayerStart = rand() % board.getXSize();
				yPlayerStart = rand() & board.getYSize();
			}

			if(board.getObject(xPlayerStart, yPlayerStart) != Board::portal ) {
				player = new Player(xPlayerStart, yPlayerStart);

				/* Set the player onto the board */
				player->move(board, 0, 0);

				/* Check if this game has a solution */
				if(!hasASolution()) {
					play = false;
					break;
				}
				clrscr();
				board.printBoard();
				break;
			}
		}

		while(play) {
			input = getch();
			clrscr();
			checkCommand(input);
			board.printBoard();
		}
	}

	void start() {
		play = true;
		gameLoop();
	}

private:
	bool play;
	int input;
	Board board;
	Player * player;
	bool hasVisited[NUM_ROWS][NUM_COLS];
};

void clrscr(){
   system("cls");
}

int main() {
    srand(time(0));

    while(1) {
   		Game g;
		g.start();
    }

	return 1;
}