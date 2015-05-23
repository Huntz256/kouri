#include "defs.h"
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

/*
 * Returns a character representing a piece, given a piece number.
*/
char numToPiece(int num) {
	unordered_map<int, char> pieceMap;
	
	if (num >= 0) {
		pieceMap[0] = ' ';
		pieceMap[1] = 'P';  pieceMap[2] = 'p'; // P/p = black pawn / white pawn
		pieceMap[3] = 'B';  pieceMap[4] = 'b'; // B/b = black bishop / white bishop
		pieceMap[5] = 'N';  pieceMap[6] = 'n'; // N/n = black knight / white knight
		pieceMap[7] = 'R';  pieceMap[8] = 'r'; // R/r = black rook / white rook
		pieceMap[9] = 'Q';  pieceMap[10] = 'q'; // Q/q = black queen / white queen
		pieceMap[11] = 002; pieceMap[12] = 001; // smiley faces = black king / white king

		return pieceMap[num];
	}

	return '~';
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
		if (i % 10 > 0 && i % 10 < 9) {
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
	if (goFirst) {
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

void BoardStructure::resetBoardToEmpty() {
	//Set all squares on the board to the value -1 representing off board squares
	for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {
		pieces[i] = -1;
	}

	//Set only the middle 8 x 8 squares to the value 0 representing empty squares
	for (int i = 0; i < 64; i++) {
		pieces[squareID120[i]] = 0;
	}

	//Clear the pawns bitboard 
	for (int i = 0; i < 2; i++) {
		pawns[i] = 0ULL;
	}

	//No side can castle on an empty board
	castlePerms = 0;
}

//Program execution starts here
int main() {
	cout << "Hello. My name is " << NAME << ".\n";
	
	string choice;
	do {
		cout << "Would you like to go first? (y/n) \n";
		getline(cin, choice);
	} while (choice.compare("y") != 0 && choice.compare("n") != 0);
	
	board.init(choice.compare("y") == 0);

	board.displayBoard();

	getline(cin, choice);

	board.resetBoardToEmpty();

	board.displayBoard();

	getline(cin, choice);

	return 0;
}


