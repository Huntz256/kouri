#include "defs.h"
#include <iostream>
#include <unordered_map>
using namespace std;

//Returns a character representing a piece, given a piece number.
char numToPieceChar(int num) {
	unordered_map<int, char> pieceMap;

	if (num >= 0) {
		pieceMap[0] = ' ';
		pieceMap[1] = 'p';  pieceMap[2] = 'P'; // black pawn / white pawn
		pieceMap[3] = 'b';  pieceMap[4] = 'B'; // black bishop / white bishop
		pieceMap[5] = 'n';  pieceMap[6] = 'N'; // black knight / white knight
		pieceMap[7] = 'r';  pieceMap[8] = 'R'; // black rook / white rook
		pieceMap[9] = 'q';  pieceMap[10] = 'Q'; // black queen / white queen
		pieceMap[11] = 002; pieceMap[12] = 001; // smiley faces = black king / white king

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
	for (int rank = RANK_8; rank >= RANK_1; rank--) {
		for (int file = FILE_A; file <= FILE_H; file++) {
			cout << " " << numToPieceChar(pieces[squareID120[rank * 8 + file]]) << "|";
		}
		cout << "\n------------------------\n";
	}

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

	//Clear the pawns bitboard 
	for (int i = 0; i < 2; i++) {
		pawns[i] = 0ULL;
	}

	//No side can castle on an empty board
	castlePerms = 0;
}
int BoardStructure::setUpBoardUsingFEN(char* fen) {

	resetBoardToEmpty();

	int currentRank = RANK_8, currentFile = FILE_A, emptyNum = 0, piece = 0, currentSquare = 0;

	//Set up board pieces using the infomation in fen
	while ((currentRank >= RANK_1) && *fen) {
		emptyNum = 1;

		switch (*fen) {
			case 'p': piece = 1; break; case 'P': piece = 2; break;
			case 'b': piece = 3; break; case 'B': piece = 4; break;
			case 'n': piece = 5; break; case 'N': piece = 6; break;
			case 'r': piece = 7; break; case 'R': piece = 8; break;
			case 'q': piece = 9; break; case 'Q': piece = 10; break;
			case 'k': piece = 11; break; case 'K': piece = 12; break;

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

void BoardStructure::makeMove(Move m){
	//If there is a promoted piece, set the destination square to that. Else, set it to the piece that's moving
	pieces[m.getToSquare()] = (m.getPromoted() != 0) ? m.getPromoted() : pieces[m.getFromSquare()];
	pieces[m.getFromSquare()] = 0; //Clear the square the piece moved from

	//Change pawns array as well?

	//Store move in next blank element of history[]
	for (int i = 0; i < 1028; i++){
		if (history[i].move == 0){
			history[i].move = m.move;
			break;
		}
	}
}