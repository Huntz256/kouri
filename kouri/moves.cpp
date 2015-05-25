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

	//Make sure arguments of this function are valid first
	if (board.pieces[square] == -1) {
		throw "EXCEPTION: INVALID SQUARE ARGUMENT"; 
	}

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
	cout << "All squares attacked by side " << side << '\n';
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

int Move::getFromSquare() {
	return move & 0x7F;
}

int Move::getToSquare() {
	return (move >> 7) & 0x7F;
}

int Move::getCapturedPiece() {
	return (move >> 14) & 0xF;
}

int Move::getPromoted() {
	return (move >> 20) & 0xF;
}

void MoveList::addPawnCapturingMove(BoardStructure board, int fromSquare, int toSquare, int capture, int side) {
	if (side == WHITE) {
		if (ranks[fromSquare] == RANK_7) {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 10, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 8, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 6, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 4, 0); numberOfMoves++;
		}
		else {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 0, 0); numberOfMoves++;
		}
	}
	else {
		if (ranks[fromSquare] == RANK_2) {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 9, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 7, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 5, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 3, 0); numberOfMoves++;
		}
		else {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 0, 0); numberOfMoves++;
		}
	}
}
void MoveList::addPawnMove(BoardStructure board, int fromSquare, int toSquare, int side) {
	if (side == WHITE) {
		if (ranks[fromSquare] == RANK_7) {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 10, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 8, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 6, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 4, 0); numberOfMoves++;
		}
		else {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 0, 0); numberOfMoves++;
		}
	}
	else {
		if (ranks[fromSquare] == RANK_2) {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 9, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 7, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 5, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 3, 0); numberOfMoves++;
		}
		else {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 0, 0); numberOfMoves++;
		}
	}
}
void MoveList::generateMoveList(BoardStructure board) {
	numberOfMoves = 0;

	if (board.sideToMove == WHITE) {

		//Go through every square on the board
		for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {

			//Make sure to only look at squares that are on the board
			if (board.pieces[i] != -1) {

				//If the current square is a white pawn...
				if (board.pieces[i] == 2) {

					//... and the square in front of it is empty, add a move
					if (board.pieces[i + 10] == 0) {
						addPawnMove(board, i, i + 10, WHITE);
					}

					
				}
			}
		}
	}
	else {
		//Go through every square on the board
		for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {

			//Make sure to only look at squares that are on the board
			if (board.pieces[i] != -1) {

				//If the current square is a black pawn...
				if (board.pieces[i] == 1) {

					//... and the square in front of it is empty, add a move
					if (board.pieces[i - 10] == 0) {
						addPawnMove(board, i, i - 10, BLACK);
					}

				}
			}
		}
	}
}

void MoveList::printMoveList() {
	for (int i = 0; i < numberOfMoves; i++) {
		cout << "From:" << moves[i].getFromSquare() << 
			" to:" << moves[i].getToSquare() <<
			" cap:" << moves[i].getCapturedPiece() <<
			" prom:" << moves[i].getPromoted() << '\n';

	}
	cout << "# of moves: " << numberOfMoves;
}
