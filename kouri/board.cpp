#include "defs.h"
#include <iostream>
#include <unordered_map>
using namespace std;

//Returns a character representing a piece, given a piece number.
char numToPieceChar(int num) {
	unordered_map<int, char> pieceMap;

	if (num >= 0) {
		pieceMap[0] = ' ';
		pieceMap[B_PAWN] = 'p';  pieceMap[W_PAWN] = 'P'; 
		pieceMap[B_BISHOP] = 'b';  pieceMap[W_BISHOP] = 'B';
		pieceMap[B_KNIGHT] = 'n';  pieceMap[W_KNIGHT] = 'N';
		pieceMap[B_ROOK] = 'r';  pieceMap[W_ROOK] = 'R'; 
		pieceMap[B_QUEEN] = 'q';  pieceMap[W_QUEEN] = 'Q';
		pieceMap[B_KING] = 002; pieceMap[W_KING] = 001; // smiley faces

		return pieceMap[num];
	}

	return '~';
}

void BoardStructure::displayFullBoard(bool dispPieces){
	for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {
		if (i % 10 == 0) {
			cout << "\n------------------------------\n";
		}
		if (!dispPieces && pieces[i] < 10 && pieces[i] >= 0) {
			cout << 0;
		}
		if (dispPieces) {
			cout << " " << numToPieceChar(pieces[i]) << "|";
		}
		else {
			cout << pieces[i] << "|";
		}
	}
	cout << "\n------------------------------\n";

	//Also display some more infomation
	cout << "Side to move: " << (sideToMove == 0 ? "White" : "Black");
	cout << "\nCastling permissions: " << castlePerms << '\n';
}
void BoardStructure::displayBoard() {

	//Display the board
	cout << "\n\n";
	for (int rank = RANK_8; rank >= RANK_1; rank--) {
		cout << rank + 1 << " ";
		for (int file = FILE_A; file <= FILE_H; file++) {
			cout << " " << numToPieceChar(pieces[squareID120[rank * 8 + file]]) << "|";
		}
		cout << "\n  ------------------------\n";
	}
	cout << "   a  b  c  d  e  f  g  h\n\n";

	//Also display some more infomation
	cout << "Side to move: " << (sideToMove == 0 ? "White" : "Black");
	cout << "\nCastling permissions: " << castlePerms << '\n';
}
void BoardStructure::init(bool goFirst) {
	if (goFirst) {
		setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w  KQkq - 0 1");
	}
	else {
		setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b  KQkq - 0 1");
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

	//Clear the pawns bitboard and reset material value to zero
	for (int i = 0; i < 2; i++) {
		pawns[i] = 0ULL;
		material[i] = 0;
	}

	//No side can castle on an empty board
	castlePerms = 0;

	//Reset en pass square
	enPassSquare = 0;
}

int BoardStructure::setUpBoardUsingFEN(char* fen) {

	resetBoardToEmpty();

	int currentRank = RANK_8, currentFile = FILE_A, emptyNum = 0, piece = 0, currentSquare = 0;

	//Set up board pieces using the infomation in fen
	while ((currentRank >= RANK_1) && *fen) {
		emptyNum = 1;

		switch (*fen) {
			case 'p': piece = B_PAWN; break; case 'P': piece = W_PAWN; break;
			case 'b': piece = B_BISHOP; break; case 'B': piece = W_BISHOP; break;
			case 'n': piece = B_KNIGHT; break; case 'N': piece = W_KNIGHT; break;
			case 'r': piece = B_ROOK; break; case 'R': piece = W_ROOK; break;
			case 'q': piece = B_QUEEN; break; case 'Q': piece = W_QUEEN; break;
			case 'k': piece = B_KING; break; case 'K': piece = W_KING; break;

			case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
				piece = 0; emptyNum = *fen - '0'; break;

			case '/': case ' ':
				currentRank--; currentFile = FILE_A; fen++; continue;
			default:
				return -1;
		}

		for (int i = 0; i < emptyNum; i++) {
			if (piece != 0) {
				pieces[squareID120[currentRank * 8 + currentFile]] = piece;
			}
			currentFile++;
		}

		fen++;
	}

	//Set the side to move using infomation in FEN
	sideToMove = (*fen == 'w') ? 0 : 1;

	//Set castling permissions using infomation in FEN
	fen += 2;
	for (int i = 0; i < 4; i++) {
		if (*fen == ' ') {
			break;
		}

		//for a FEN such as ...blah blah w KQkq blah blah...
		switch (*fen) {
			case 'K': castlePerms |= 1;  break; //... if there is a K, do castlePerms OR 0001; e.g. 0000 | 0001 = 0001
			case 'Q': castlePerms |= 2;  break; //... if there is a Q, do castlePerms OR 0010; e.g. 0001 | 0010 = 0011
			case 'k': castlePerms |= 4;  break; //... if there is a k, do castlePerms OR 0100; e.g. 0011 | 0100 = 0111
			case 'q': castlePerms |= 8; break; //... if there is a q, do castlePerms OR 1000; e.g. 0111 | 1000 = 1111
			default: break;
		}
		fen++;
	}

	return 0;
}

void BoardStructure::makeMove(Move m) {
	if (m.getCastling() != 0){
		switch (m.getCastling()){ //Take care of castling
		case 1: //white king side
			pieces[25] = 0; pieces[28] = 0; //Clear king and rook spaces
			pieces[27] = W_KING; pieces[26] = W_ROOK; //Place king and rook
			break;
		case 2: //white queen side
			pieces[25] = 0; pieces[21] = 0;
			pieces[23] = W_KING; pieces[24] = W_ROOK;
			break;
		case 3: //black king side
			pieces[95] = 0; pieces[98] = 0;
			pieces[97] = B_KING; pieces[96] = B_ROOK;
			break;
		case 4: //black queen side
			pieces[95] = 0; pieces[91] = 0;
			pieces[93] = B_KING; pieces[94] = B_ROOK;
			break;
		default: //nothing
			break;
		}
	}
	else{
		//If there is a promoted piece, set the destination square to that. Else, set it to the piece that's moving
		pieces[m.getToSquare()] = (m.getPromoted() != 0) ? m.getPromoted() : pieces[m.getFromSquare()];
		pieces[m.getFromSquare()] = 0; //Clear the square the piece moved from
	}

	//Store move in next blank element of history[]
	for (int i = 0; i < 1028; i++) {
		if (history[i].move == 0) {
			history[i].move = m.move;
			break;
		}
	}
}

int BoardStructure::getPieceColor(int pieceNum) {
	switch (pieceNum) {
	case B_PAWN: case B_BISHOP: case B_KNIGHT: case B_ROOK: case B_QUEEN: case B_KING:
		return BLACK; break;
	case W_PAWN: case W_BISHOP: case W_KNIGHT: case W_ROOK: case W_QUEEN: case W_KING:
		return WHITE; break;
	default:
		return -42; break;
	}
}

void BoardStructure::displayHistory(){
	for (int i = 0; i < 1028; i++) {
		if (history[i].move == 0) break;
		cout << history[i].move << " ";
	}
	cout << "\n";
}

//Prints a square. For example, printSquare(55) prints "e4"
void printSquare(int square) {
	cout << FILES_TO_CHAR[FILES[square]] << (1 + RANKS[square]);
}