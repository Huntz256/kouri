#include "defs.h"
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

//Function name pending
char numToPiece(int num) {
	/* Black pieces are capital letters
	** White pieces are lowercase
	** P/p = pawn
	** B/b = bishop
	** K/k = knight
	** R/r = rook
	** Q/q = queen
	** (smiley) = king
	*/

	unordered_map<int, char> pieceMap;
	pieceMap[0] = ' ';
	pieceMap[1] = 'P';  pieceMap[2] = 'p';
	pieceMap[3] = 'B';  pieceMap[4] = 'b';
	pieceMap[5] = 'K';  pieceMap[6] = 'k';
	pieceMap[7] = 'R';  pieceMap[8] = 'r';
	pieceMap[9] = 'Q';  pieceMap[10] = 'q';
	pieceMap[11] = 002; pieceMap[12] = 001; //Black/white smiley faces

	return pieceMap[num];
}

void BoardStructure::displayFullBoard(bool dispPieces = true){
	for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {
		if (i % 10 == 0) {
			cout << "\n------------------------------\n";
		}
		if (!dispPieces && pieces[i] % 12 < 10) {
			cout << 0;
		}
		if (dispPieces) {
			cout << " " << numToPiece(pieces[i]) << "|";
		}
		else { 
			cout << pieces[i] << "|"; 
		}
	}
	cout << "\n------------------------------\n";
}

void BoardStructure::displayBoard(bool dispPieces = true){
	for (int i = 20; i < BOARD_SQUARE_COUNT-20; i++) {
		if (i % 10 > 0 && i % 10 < 9){
			if (i % 10 == 1) {
				cout << "\n------------------------\n";
			}
			if (!dispPieces && pieces[i] % 12 < 10) {
				cout << 0;
			}
			if (dispPieces) {
				cout << " " << numToPiece(pieces[i]) << "|";
			}
			else {
				cout << pieces[i] << "|";
			}
		}
	}
	cout << "\n------------------------\n";
}

void BoardStructure::init(bool goFirst) {
	if (goFirst){
		int start[120] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 7, 5, 3, 11, 9, 3, 5, 7, 0,
							0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
							0, 8, 6, 4, 12, 10, 4, 6, 8, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {
			pieces[i] = start[i];
		}
	}
	else {
		int start[120] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 8, 6, 4, 12, 10, 4, 6, 8, 0,
							0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
							0, 7, 5, 3, 11, 9, 3, 5, 7, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		for (int i = 0; i < BOARD_SQUARE_COUNT; i++){
			pieces[i] = start[i];
		}
	}
}

//Program execution starts here
int main() {
	cout << "Hello. My name is " << NAME << ".\n";
	
	string choice;
	do {
		cout << "Would you like to go first? (y/n) \n";
		cin >> choice;
	} while (choice.compare("y") != 0 && choice.compare("n") != 0);
	
	if (choice.compare("y") == 0) board.init(true);
	else board.init(false);
		
	board.displayBoard();

	int x; cin >> x;

	return 0;
}


