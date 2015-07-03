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


//Used for debugging the function isSquareAttacked()
void testIsSquareAttacked(int side, BoardStructure board) {
	cout << "All squares attacked by side " << side << '\n';

	//Display the board
	cout << "\n";
	for (int rank = RANK_8; rank >= RANK_1; rank--) {
		cout << rank + 1 << " ";
		for (int file = FILE_A; file <= FILE_H; file++) {
			if (board.isSquareAttacked(squareID120[rank * 8 + file], side)) {
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
	return (move >> 18) & 0x7F;
}
int Move::getCastling() {
	return (move >> 22) & 0x7;
}

void MoveListGenerator::addPawnCapturingMove(BoardStructure board, int fromSquare, int toSquare, int capture, int side) {
	if (FILES[fromSquare] == -1) {
		throw "ERROR3";
	}
	if (FILES[toSquare] == -1) {
		throw "ERROR4";
	}
	
	if (side == WHITE) {
		if (RANKS[fromSquare] == RANK_7) {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, W_QUEEN, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, W_ROOK, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, W_KNIGHT, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, W_BISHOP, 0); numberOfMoves++;
		}
		else {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 0, 0); numberOfMoves++;
		}
	}
	else {
		if (RANKS[fromSquare] == RANK_2) {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, B_QUEEN, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, B_ROOK, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, B_KNIGHT, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, B_BISHOP, 0); numberOfMoves++;
		}
		else {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, capture, 0, 0); numberOfMoves++;
		}
	}
}
void MoveListGenerator::addPawnMove(BoardStructure board, int fromSquare, int toSquare, int side) {

	if (FILES[fromSquare] == -1) {
		throw "ERROR1";
	}
	if (FILES[toSquare] == -1) {
		throw "ERROR2";
	}

	//White----
	if (side == WHITE) {
		//Promotions
		if (RANKS[fromSquare] == RANK_7) {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, W_QUEEN, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, W_ROOK, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, W_KNIGHT, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, W_BISHOP, 0); numberOfMoves++;
		}
		else {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 0, 0); numberOfMoves++;
		}

	}

	//Black----
	else {
		//Promotions
		if (RANKS[fromSquare] == RANK_2) {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, B_QUEEN, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, B_ROOK, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, B_KNIGHT, 0); numberOfMoves++;
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, B_BISHOP, 0); numberOfMoves++;
		}
		else {
			moves[numberOfMoves].move = MOVE(fromSquare, toSquare, 0, 0, 0); numberOfMoves++;
		}
	}
}
void MoveListGenerator::generatePawnMoves(BoardStructure board) {
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
				if (board.pieces[i + 9] > 0 && board.getPieceColor(board.pieces[i + 9]) == BLACK) {
					addPawnCapturingMove(board, i, i + 9, board.pieces[i + 9], WHITE);
				}

				if (board.pieces[i + 11] > 0 && board.getPieceColor(board.pieces[i + 11]) == BLACK) {
					addPawnCapturingMove(board, i, i + 11, board.pieces[i + 11], WHITE);
				}

				if (board.enPassSquare != 0) {
					//En passant 
					if (board.pieces[i + 9] > 0 && board.pieces[i + 9] == board.enPassSquare) {
						//addPawnCapturingMove(board, i, i + 9, 0, WHITE);
					}
					if (board.pieces[i + 11] > 0 && board.pieces[i + 11] == board.enPassSquare) {
						//addPawnCapturingMove(board, i, i + 11, 0, WHITE);
					}
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
				if (board.pieces[i - 9] > 0 && board.getPieceColor(board.pieces[i - 9]) == WHITE) {
					addPawnCapturingMove(board, i, i - 9, board.pieces[i - 9], BLACK);
				}

				if (board.pieces[i - 11] > 0 && board.getPieceColor(board.pieces[i - 11]) == WHITE) {
					addPawnCapturingMove(board, i, i - 11, board.pieces[i - 11], BLACK);
				}

				if (board.enPassSquare != 0) {
					//En passant 
					if (board.pieces[i - 9] > 0 && board.pieces[i - 9] == board.enPassSquare) {
						//	addPawnCapturingMove(board, i, i - 9, 0, BLACK);
					}
					if (board.pieces[i - 11] > 0 && board.pieces[i - 11] == board.enPassSquare) {
						//addPawnCapturingMove(board, i, i - 11, 0, BLACK);
					}
				}
			}
			
		}
	}
}
void MoveListGenerator::generateSliderMoves(BoardStructure board) {
	int pieceIndex = SLIDING_PIECES_START_INDEX[board.sideToMove];
	int piece = SLIDING_PIECES[pieceIndex];
	pieceIndex++;

	while (piece != 0) {
		//Go through every square on the board
		for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {

			//If there is a piece on square i that is a slider piece...
			if (board.pieces[i] == piece) {
				
				//For each direction...
				for (int j = 0; j < NUMBER_OF_DIRECTIONS[piece]; j++) {
					int tempSquare = i + PIECE_MOVEMENTS[piece][j];

					//While tempSquare is not off board...
					while (board.pieces[tempSquare] != -1) {

						//If tempSquare is not empty, generate a capturing move
						if (board.pieces[tempSquare] != 0) {
							//If tempSquare has a piece opposite in color to the piece on square i
							//BLACK ^ 1 == WHITE, WHITE ^ 1 == BLACK (^ is the XOR operator)
							if (board.getPieceColor(board.pieces[tempSquare]) == (board.sideToMove ^ 1)) {
								moves[numberOfMoves].move = MOVE(i, tempSquare, board.pieces[tempSquare], 0, 0);
								numberOfMoves++;
							}
							break;
						}

						//If tempSquare is empty, generate a non capture move 
						else if (board.pieces[tempSquare] == 0) {
							moves[numberOfMoves].move = MOVE(i, tempSquare, 0, 0, 0);
							numberOfMoves++;
						}

						tempSquare += PIECE_MOVEMENTS[piece][j];

					}
				}
			}
		}

		piece = SLIDING_PIECES[pieceIndex];
		pieceIndex++;
	}

}
void MoveListGenerator::generateNonSliderMoves(BoardStructure board) {
	int pieceIndex = NON_SLIDING_PIECES_START_INDEX[board.sideToMove];
	int piece = NON_SLIDING_PIECES[pieceIndex];
	pieceIndex++;

	while (piece != 0) {

		//Go through every square on the board
		for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {

			//If there is a piece on square i that is a non slider piece...
			if (board.pieces[i] == piece) {

				if (piece == 0) {
					break;
				}

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
							//cout << "board.getPieceColor(board.pieces[tempSquare])" << board.getPieceColor(board.pieces[tempSquare]) << "\n";
							//cout << "board.sideToMove:" << board.sideToMove << "\n";

							moves[numberOfMoves].move = MOVE(i, tempSquare, board.pieces[tempSquare], 0, 0); 
							numberOfMoves++;

							//If king is attacked by the other side when this move is made
							//if (board.isSquareAttacked(board.kingSquare[board.sideToMove], board.sideToMove ^ 1)) {
								//board.undoMove();
							//}
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
void MoveListGenerator::generateCastlingMoves(BoardStructure board) {
	if (board.sideToMove == WHITE) {
		//White Kingside Castling
		if (board.castlePerms & 1) {
			if (board.pieces[26] == 0 && board.pieces[27] == 0) {
				if (!board.isSquareAttacked(25, BLACK) && !board.isSquareAttacked(26, BLACK) && !board.isSquareAttacked(27, BLACK)) {
					moves[numberOfMoves].move = MOVE(25, 27, 0, 0, 1);
					numberOfMoves++;
				}
			}
		}

		//White Queenside Castling
		if (board.castlePerms & 2) {
			if (board.pieces[22] == 0 && board.pieces[23] == 0 && board.pieces[24] == 0) {
				if (!board.isSquareAttacked(23, BLACK) && !board.isSquareAttacked(24, BLACK) && !board.isSquareAttacked(25, BLACK)) {
					moves[numberOfMoves].move = MOVE(25, 23, 0, 0, 2);
					numberOfMoves++;
				}
			}
		}
	}
	else {
		//Black Kingside Castling
		if (board.castlePerms & 4) {
			if (board.pieces[96] == 0 && board.pieces[97] == 0) {
				if (!board.isSquareAttacked(95, WHITE) && !board.isSquareAttacked(96, WHITE) && !board.isSquareAttacked(97, WHITE)) {
					moves[numberOfMoves].move = MOVE(95, 97, 0, 0, 3);
					numberOfMoves++;
				}
			}
		}


		//Black Queenside Castling
		if (board.castlePerms & 8) {
			if (board.pieces[92] == 0 && board.pieces[93] == 0 && board.pieces[94] == 0) {
				if (!board.isSquareAttacked(93, WHITE) && !board.isSquareAttacked(94, WHITE) && !board.isSquareAttacked(95, WHITE)) {
					moves[numberOfMoves].move = MOVE(95, 93, 0, 0, 4);
					numberOfMoves++;
				}
			}
		}

	}
}
void MoveListGenerator::generateMoveList(BoardStructure board) {
	numberOfMoves = 0;

	//Clear move arrays
	for (int i = 0; i <= numberOfMoves; i++) {
		moves[i].move = 0;
	}

	//Generate psuedo-legal moves
	generatePawnMoves(board);
	generateSliderMoves(board);
	generateNonSliderMoves(board);
	generateCastlingMoves(board);

	//Regenerate position key
	board.positionID = board.generateAndGetPositionID();

}

//Prints all of the moves in our move list in algebraic notation
//MUST be called immediately after generateMoveList() for piece num printing to be accuarate
void MoveListGenerator::printMoveList(BoardStructure board) {

	const char PIECE_NUM_TO_CHAR[13] = { ' ', ' ', ' ', 'B', 'B', 'N', 'N', 'R', 'R', 'Q', 'Q', 'K', 'K' };
	
	for (int i = 0; i < numberOfMoves; i++) {
		int fromSquare = moves[i].getFromSquare();
		int toSquare = moves[i].getToSquare();
		int capPiece = moves[i].getCapturedPiece();
		int promPiece = moves[i].getPromoted();

		cout << "Move " << i << " Found: "; cout << "(piece num: " << board.pieces[fromSquare] << ")";
		//cout << "Move " << i << " Found: "; cout << "(piece num: " << board.pieces[fromSquare] << ")";

		if ((moves[i].getCastling() == 1) || (moves[i].getCastling() == 3)) {
			cout << "O-O\n";
		}
		else if ((moves[i].getCastling() == 2) || (moves[i].getCastling() == 4)) {
			cout << "O-O-O\n";
		}
		else {
			 cout << PIECE_NUM_TO_CHAR[board.pieces[moves[i].getFromSquare()]];
			 //cout << PIECE_NUM_TO_CHAR[board.pieces[fromSquare]];

			 if (moves[i].getCapturedPiece() != 0) {
				 cout << FILES_TO_CHAR[FILES[moves[i].getFromSquare()]] << "x";
				//cout << FILES_TO_CHAR[FILES[fromSquare]] << "x";
			}

			printSquare(toSquare);

			cout << " (f:" << fromSquare << " t:" << toSquare << " c:" << capPiece << " p: " << promPiece << ")\n";
		}
	}

	cout << "# of moves: " << numberOfMoves << "\n\n";
}

//Prints a move in UCI format given a move number on a move list. E.g. uciPrintMove(board, 4)
void MoveListGenerator::uciPrintMoveGivenMoveListNumber(BoardStructure board, int moveNum) {
	const char PIECE_NUM_TO_CHAR[13] = { ' ', ' ', ' ', 'B', 'B', 'N', 'N', 'R', 'R', 'Q', 'Q', 'K', 'K' };

	int fromSquare = moves[moveNum].getFromSquare();
	int toSquare = moves[moveNum].getToSquare();

	printSquare(fromSquare);

	printSquare(toSquare);

	cout << "\n";

}

//Prints a move in UCI format given a move. E.g. uciPrintMove(board, m)
void MoveListGenerator::uciPrintMoveGivenMove(BoardStructure board, Move m) {
	const char PIECE_NUM_TO_CHAR[13] = { ' ', ' ', ' ', 'B', 'B', 'N', 'N', 'R', 'R', 'Q', 'Q', 'K', 'K' };

	int fromSquare = m.getFromSquare();
	int toSquare = m.getToSquare();

	printSquare(fromSquare);
	printSquare(toSquare);
	//cout << "\n";
}

//Prints a move in UCI format given a move integer. E.g. uciPrintMove(board, m.move)
void MoveListGenerator::uciPrintMoveGivenMoveInt(BoardStructure board, int move) {
	const char PIECE_NUM_TO_CHAR[13] = { ' ', ' ', ' ', 'B', 'B', 'N', 'N', 'R', 'R', 'Q', 'Q', 'K', 'K' };

	Move m; m.move = move;
	int fromSquare = m.getFromSquare();
	int toSquare = m.getToSquare();

	printSquare(fromSquare);
	printSquare(toSquare);
	cout << "\n";
}

//Checks if a move integer is contained in the generated movelist and is valid
bool MoveListGenerator::isMoveValid(BoardStructure board, int move) {

	int fromSquare = move & 0x7F;
	int toSquare = (move >> 7) & 0x7F;

	if (fromSquare < 21 || fromSquare > 98) {
		return false;
	}
	if (toSquare < 21 || toSquare > 98) {
		return false;
	}
	if (board.pieces[fromSquare] <= 0) {
		return false;
	}
	
	generateMoveList(board);

	for (int i = 0; i < numberOfMoves; i++) {
		if (!board.makeMove(moves[i])) {
			continue;
		}
		board.undoMove();
		if (moves[i].move == move) {
			return true;
		}
	}
	return false;
}

//Checks if a move integer is contained in the generated movelist and is valid
//Less likely to return a false negative than isMoveValid(), but more likely to return a false positive
bool MoveListGenerator::isMoveValid2(BoardStructure board, int move) {

	int fromSquare = move & 0x7F;
	int toSquare = (move >> 7) & 0x7F;

	if (fromSquare < 21 || fromSquare > 98) {
		return false;
	}
	if (toSquare < 21 || toSquare > 98) {
		return false;
	}
	if (board.pieces[fromSquare] <= 0) {
		return false;
	}

	generateMoveList(board);

	for (int i = 0; i < numberOfMoves; i++) {
		if (moves[i].move == move) {
			return true;
		}
	}
	return false;
}