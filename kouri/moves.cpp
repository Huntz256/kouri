#include "defs.h"
#include <iostream>
using namespace std;

//Represents where a knight/rook/bishop/queeen/king can move
const int KNIGHT_MOVEMENTS[8] = { -19, -8, 12, 21, 19, 8, -12, -21 };
const int ROOK_MOVEMENTS[4] = { -10, 1, 10, -1 };
const int BISHOP_MOVEMENTS[4] = { -9, 11, 9, -11 };
const int KING_MOVEMENTS[8] = { -10, -9, 1, 11, 10, 9, -1, -11 };

//Used in generateSliderMoves()
const int SLIDING_PIECES[8] = { W_BISHOP, W_ROOK, W_QUEEN, 0, B_BISHOP, B_ROOK, B_QUEEN };
const int SLIDING_PIECES_START_INDEX[2] = { 0, 4 };

//Used in generateNonSliderMoves()
const int NON_SLIDING_PIECES[6] = { W_KNIGHT, W_KING, 0, B_KNIGHT, B_KING, 0 };
const int NON_SLIDING_PIECES_START_INDEX[2] = { 0, 3 };

//For a piece i, NUMBER_OF_DIRECTIONS[i] is the max number of directions it can move
const int NUMBER_OF_DIRECTIONS[13] = { 0, 0, 0, 4, 4, 8, 8, 4, 4, 8, 8, 8, 8 };

//Used in generateSliderMoves() and generateNonSliderMoves()
const int PIECE_MOVEMENTS[13][8] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -9, 11, 9, -11 },
	{ -9, 11, 9, -11 },
	{ -19, -8, 12, 21, 19, 8, -12, -21 },
	{ -19, -8, 12, 21, 19, 8, -12, -21 },
	{ -10, 1, 10, -1 },
	{ -10, 1, 10, -1 },
	{ -10, -9, 1, 11, 10, 9, -1, -11 },
	{ -10, -9, 1, 11, 10, 9, -1, -11 },
	{ -10, -9, 1, 11, 10, 9, -1, -11 },
	{ -10, -9, 1, 11, 10, 9, -1, -11 }
};

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
		if (board.pieces[square + KNIGHT_MOVEMENTS[i]] == 6 && attackingSide == WHITE) {
			return true;
		}

		//If square square is being attacked by a black knight, return true
		if (board.pieces[square + KNIGHT_MOVEMENTS[i]] == 5 && attackingSide == BLACK) {
			return true;
		}

	}

	//Check if square square is being attacked by attackingSide by any rooks or queens...
	int tempSquare = 0;
	for (int i = 0; i < 4; i++) {
		tempSquare = square + ROOK_MOVEMENTS[i];
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
			tempSquare += ROOK_MOVEMENTS[i];
		}
	}

	//Check if square square is being attacked by attackingSide by any bishops or queens...
	tempSquare = 0;
	for (int i = 0; i < 4; i++) {
		tempSquare = square + BISHOP_MOVEMENTS[i];
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
			tempSquare += BISHOP_MOVEMENTS[i];
		}
	}

	//Check if square square is being attacked by attackingSide by any kings (with the courage to attack)...
	for (int i = 0; i < 8; i++) {

		//If square square is being attacked by a white knight, return true
		if (board.pieces[square + KING_MOVEMENTS[i]] == 12 && attackingSide == WHITE) {
			return true;
		}

		//If square square is being attacked by a black knight, return true
		if (board.pieces[square + KING_MOVEMENTS[i]] == 11 && attackingSide == BLACK) {
			return true;
		}

	}


	return false;
}

//Used for debugging the function isSquareAttacked()
void testIsSquareAttacked(int side, BoardStructure board) {
	cout << "All squares attacked by side " << side << '\n';

	//Display the board
	cout << "\n";
	for (int rank = RANK_8; rank >= RANK_1; rank--) {
		cout << rank + 1 << " ";
		for (int file = FILE_A; file <= FILE_H; file++) {
			if (isSquareAttacked(squareID120[rank * 8 + file], side, board)) {
				cout << " X|";
			}
			else {
				cout << "  |";
			}
		}
		cout << "\n  ------------------------\n";
	}
	cout << "   a  b  c  d  e  f  g  h\n\n";
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
	return (move >> 18) & 0x7;
}
int Move::getCastling() {
	return (move >> 21) & 0x7;
}

void MoveList::addPawnCapturingMove(BoardStructure board, int fromSquare, int toSquare, int capture, int side) {
	if (side == WHITE) {
		if (RANKS[fromSquare] == RANK_7) {
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
		if (RANKS[fromSquare] == RANK_2) {
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
	//White--
	if (side == WHITE) {
		if (RANKS[fromSquare] == RANK_7) {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 10, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 8, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 6, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 4, 0); numberOfMoves++;
		}
		{
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 0, 0); numberOfMoves++;
		}

	}

	//Black----
	else {
		if (RANKS[fromSquare] == RANK_2) {
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
void MoveList::generatePawnMoves(BoardStructure board) {
	if (board.sideToMove == WHITE) {

		//Go through every square on the board
		for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {

			//If the current square is a white pawn...
			if (board.pieces[i] == 2) {

				//... and the square in front of it is empty, add a move
				if (board.pieces[i + 10] == 0) {

					//.. and the pawn is on starting rank
					if (RANKS[i] == RANK_2 && board.pieces[i + 20] == 0) {
						addPawnMove(board, i, i + 20, WHITE);
					}

					//.. regardless ...
					addPawnMove(board, i, i + 10, WHITE);


				}

				//Pawn capturing
				if (board.getPieceColor(board.pieces[i + 9]) == BLACK) {
					addPawnCapturingMove(board, i, i + 9, board.pieces[i + 9], WHITE);
				}

				if (board.getPieceColor(board.pieces[i + 11]) == BLACK) {
					addPawnCapturingMove(board, i, i + 11, board.pieces[i + 11], WHITE);
				}

				//En passant 
				if (board.pieces[i + 9] == board.enPassSquare) {
					//addPawnCapturingMove(board, i, i + 9, 0, WHITE);
				}
				if (board.pieces[i + 11] == board.enPassSquare) {
					//addPawnCapturingMove(board, i, i + 11, 0, WHITE);
				}

			}
			
		}
	}
	else {
		//Go through every square on the board
		for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {

			//If the current square is a black pawn...
			if (board.pieces[i] == 1) {

				//... and the square in front of it is empty, add a move
				if (board.pieces[i - 10] == 0) {

					//.. and the pawn is on starting rank
					if (RANKS[i] == RANK_7 && board.pieces[i - 20] == 0) {
						addPawnMove(board, i, i - 20, BLACK);
					}

					//.. regardless...
					addPawnMove(board, i, i - 10, BLACK);
				}

				//Pawn capturing
				if (board.getPieceColor(board.pieces[i - 9]) == WHITE) {
					addPawnCapturingMove(board, i, i - 9, board.pieces[i - 9], BLACK);
				}

				if (board.getPieceColor(board.pieces[i - 11]) == WHITE) {
					addPawnCapturingMove(board, i, i - 11, board.pieces[i - 11], BLACK);
				}

				//En passant 
				if (board.pieces[i - 9] == board.enPassSquare) {
					//	addPawnCapturingMove(board, i, i - 9, 0, BLACK);
				}
				if (board.pieces[i - 11] == board.enPassSquare) {
					//addPawnCapturingMove(board, i, i - 11, 0, BLACK);
				}
			}
			
		}
	}
}
void MoveList::generateSliderMoves(BoardStructure board) {
	int pieceIndex = SLIDING_PIECES_START_INDEX[board.sideToMove];
	int piece = SLIDING_PIECES[pieceIndex];
	pieceIndex++;

	while (piece != 0) {
		//Go through every square on the board
		for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {

			//If there is a piece on square i that is a slider piece...
			if (board.pieces[i] == piece) {
				//to do
			}
		}
	}

}
void MoveList::generateNonSliderMoves(BoardStructure board) {
	int pieceIndex = NON_SLIDING_PIECES_START_INDEX[board.sideToMove];
	int piece = NON_SLIDING_PIECES[pieceIndex];
	pieceIndex++;

	while (piece != 0) {

		//Go through every square on the board
		for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {

			//If there is a piece on square i that is a non slider piece...
			if (board.pieces[i] == piece) {

				//For each direction...
				for (int j = 0; j < NUMBER_OF_DIRECTIONS[piece]; j++) {
					int tempSquare = i + PIECE_MOVEMENTS[piece][j];

					//If tempSquare is off board, move on to the next direction
					if (board.pieces[tempSquare] == -1) {
						continue;
					}

					//If tempSquare is not empty, generate a capturing move
					if (board.pieces[tempSquare] != 0) {

						//If tempSquare has a piece opposite in color to the piece on square i
						//BLACK ^ 1 == WHITE, WHITE ^ 1 == BLACK (^ is the XOR operator)
						if (board.getPieceColor(board.pieces[tempSquare]) == (board.sideToMove ^ 1)) {
							moves[numberOfMoves].move = MOVE(i, tempSquare, board.pieces[tempSquare], 0, 0); 
							numberOfMoves++;
						}
					}

					//If tempSquare is empty, generate a non capture move 
					else if (board.pieces[tempSquare] == 0) {
						moves[numberOfMoves].move = MOVE(i, tempSquare, 0, 0, 0);
						numberOfMoves++;
					}

				}
			}
		}


		piece = NON_SLIDING_PIECES[pieceIndex];
		pieceIndex++;
	}
}
void MoveList::generateMoveList(BoardStructure board) {
	numberOfMoves = 0;
	generatePawnMoves(board);
	generateSliderMoves(board);
	generateNonSliderMoves(board);
}

//Prints all of the moves in our move list in algebraic notation
void MoveList::printMoveList(BoardStructure board) {

	const char PIECE_NUM_TO_CHAR[13] = { ' ', ' ', ' ', 'B', 'B', 'N', 'N', 'R', 'R', 'Q', 'Q', 'K', 'K' };
	
	for (int i = 0; i < numberOfMoves; i++) {	
		int fromSquare = moves[i].getFromSquare();
		int toSquare = moves[i].getToSquare();
		int capPiece = moves[i].getCapturedPiece();
		int promPiece = moves[i].getPromoted();

		cout << "Move Found: " << PIECE_NUM_TO_CHAR[board.pieces[fromSquare]];

		if (moves[i].getCapturedPiece() != 0) {
			cout << FILES_TO_CHAR[FILES[fromSquare]] << "x";
		}

		printSquare(toSquare);

		cout << " (f:" << fromSquare << " t:" << toSquare << " c:" << capPiece << " p: " << promPiece << ")\n";
	}

	cout << "# of moves: " << numberOfMoves;
}
