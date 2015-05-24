#include "defs.h"
#include <iostream>
using namespace std;

//Represents where a knight/rook/bishop/king can move
const int KnightMovements[8] = { -19, -8, 12, 21, 19, 8, -12, -21 };
const int RookMovements[4] = { -10, 1, 10, -1 };
const int BishopMovements[4] = { -9, 11, 9, -11 };
const int KingMovements[8] = { -10, -9, 1, 11, 10, 9, -1, -11 };

/*
 * Returns true if square square is being attacked by a piece from the side attackingSide for board board.
 * Returns false otherwise.
 */
bool isSquareAttacked(int square, int attackingSide, BoardStructure board) {

	//Check if square square is being attacked by attackingSide by any pawns...
	if (attackingSide == WHITE) {

		//If square square is being attacked by a white pawn, return true
		if (board.pieces[square - 11] == 2 || board.pieces[square - 9] == 2) {
			return true;
		}

	}
	else {
		//If square square is being attacked by a black pawn, return true
		if (board.pieces[square + 11] == 1 || board.pieces[square + 9] == 1) {
			return true;
		}
	}

	//Check if square square is being attacked by attackingSide by any knights...
	for (int i = 0; i < 8; i++) {

		//If square square is being attacked by a white knight, return true
		if (board.pieces[square + KnightMovements[i]] == 6 && attackingSide == WHITE) {
			return true;
		}

		//If square square is being attacked by a black knight, return true
		if (board.pieces[square + KnightMovements[i]] == 5 && attackingSide == BLACK) {
			return true;
		}

	}

	//Check if square square is being attacked by attackingSide by any rooks or queens...
	int tempSquare = 0;
	for (int i = 0; i < 4; i++) {
		tempSquare = square + RookMovements[i];
		while (board.pieces[tempSquare] != -1) {
			if (board.pieces[tempSquare] != 0) {
				if ( ((board.pieces[tempSquare] == 8) || (board.pieces[tempSquare] == 10)) && attackingSide == WHITE ) {
					return true;
				}
				if ( ((board.pieces[tempSquare] == 7) || (board.pieces[tempSquare] == 9)) && attackingSide == BLACK ) {
					return true;
				}
				break;
			 }
			tempSquare += RookMovements[i];
		}
	}

	//Check if square square is being attacked by attackingSide by any bishops or queens...
	tempSquare = 0;
	for (int i = 0; i < 4; i++) {
		tempSquare = square + BishopMovements[i];
		while (board.pieces[tempSquare] != -1) {
			if (board.pieces[tempSquare] != 0) {
				if (((board.pieces[tempSquare] == 4) || (board.pieces[tempSquare] == 10)) && attackingSide == WHITE) {
					return true;
				}
				if (((board.pieces[tempSquare] == 3) || (board.pieces[tempSquare] == 9)) && attackingSide == BLACK) {
					return true;
				}
				break;
			}
			tempSquare += BishopMovements[i];
		}
	}

	//Check if square square is being attacked by attackingSide by any kings (with the courage to attack)...
	for (int i = 0; i < 8; i++) {

		//If square square is being attacked by a white knight, return true
		if (board.pieces[square + KingMovements[i]] == 12 && attackingSide == WHITE) {
			return true;
		}

		//If square square is being attacked by a black knight, return true
		if (board.pieces[square + KingMovements[i]] == 11 && attackingSide == BLACK) {
			return true;
		}

	}


	return false;
}

//Used for debugging the function isSquareAttacked()
void testIsSquareAttacked(int side, BoardStructure board) {
	cout << "All sqaures attacked by side " << side << '\n';
	for (int rank = RANK_8; rank >= RANK_1; rank--) {
		for (int file = FILE_A; file <= FILE_H; file++) {
			if (isSquareAttacked(squareID120[rank * 8 + file], side, board)) {
				cout << "X ";
			}
			else {
				cout << "- ";
			}
		}
		cout << '\n';
	}
	cout << '\n';
}