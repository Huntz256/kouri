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

//Used for pawn promotion user command
int charToPieceInt(char c){
	switch (c){
		case 'p': return B_PAWN; case 'P': return W_PAWN;
		case 'b': return B_BISHOP; case 'B': return W_BISHOP;
		case 'n': return B_KNIGHT; case 'N': return W_KNIGHT;
		case 'r': return B_ROOK; case 'R': return W_ROOK;
		case 'q': return B_QUEEN; case 'Q': return W_QUEEN;
		case 'k': return B_KING; case 'K': return W_KING;
		default: return -1;
	}
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

	//Display the board with white on bottom
	cout << "\n";
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
		setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	}
	else {
		setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
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

	//Reset king's square
	kingSquare[0] = 0; kingSquare[1] = 0;

	//Reset ply and historyPly
	ply = 0; historyPly = 0;
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

	//Update king square
	for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {
		if (pieces[i] == W_KING) {
			kingSquare[WHITE] = i;
		}
		else if (pieces[i] == B_KING) {
			kingSquare[BLACK] = i;
		}
	}

	return 0;
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

const int CASTLE_PERMISSIONS[120] = {
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

//Represents where a knight/rook/bishop/queeen/king can move
const int KNIGHT_MOVEMENTS[8] = { -19, -8, 12, 21, 19, 8, -12, -21 };
const int ROOK_MOVEMENTS[4] = { -10, 1, 10, -1 };
const int BISHOP_MOVEMENTS[4] = { -9, 11, 9, -11 };
const int KING_MOVEMENTS[8] = { -10, -9, 1, 11, 10, 9, -1, -11 };

/*
* Returns true if square square is being attacked by a piece from the side attackingSide for board board.
* Returns false otherwise.
*/
bool BoardStructure::isSquareAttacked(int square, int attackingSide) {

	//cout << "isSquareAttacked(): attackingSide = " << attackingSide << "\n";
	///cout << "isSquareAttacked(): bloop\n";
	//Make sure arguments of this function are valid first
	if (pieces[square] == -1) {
		cout << "EXCEPTION: INVALID SQUARE ARGUMENT";
	}

	//Check if square square is being attacked by attackingSide by any pawns...
	if (attackingSide == WHITE) {

		//If square square is being attacked by a white pawn, return true
		if (pieces[square - 11] == 2 || pieces[square - 9] == 2) {
			///cout << "isSquareAttacked(): yes. square " << square << " is being attacked by a pawn\n"; 
			return true;
		}


	}
	else {
		//If square square is being attacked by a black pawn, return true
		if (pieces[square + 11] == 1 || pieces[square + 9] == 1) {
			///cout << "isSquareAttacked(): yes. square " << square << " is being attacked by a pawn\n"; 
			return true;
		}
	}

	//Check if square square is being attacked by attackingSide by any knights...
	for (int i = 0; i < 8; i++) {

		//If square square is being attacked by a white knight, return true
		if (attackingSide == WHITE && pieces[square + KNIGHT_MOVEMENTS[i]] == 6) {
			///cout << "isSquareAttacked(): yes. square " << square << " is being attacked by a white knight\n"; 
			return true;
		}
		else{
			//If square square is being attacked by a black knight, return true
			if (pieces[square + KNIGHT_MOVEMENTS[i]] == 5) {
				///cout << "isSquareAttacked(): yes. square " << square << " is being attacked by a black knight\n"; 
				return true;
			}
		}
	}

	//Check if square square is being attacked by attackingSide by any rooks or queens...
	int tempSquare = 0;
	for (int i = 0; i < 4; i++) {
		tempSquare = square + ROOK_MOVEMENTS[i];
		while (pieces[tempSquare] != -1) {
			if (pieces[tempSquare] != 0) {
				if (((pieces[tempSquare] == 8) || (pieces[tempSquare] == 10)) && attackingSide == WHITE) {
					///cout << "isSquareAttacked(): yes. square " << square << " is being attacked by a white rook or queen\n"; 
					return true;
				}
				if (((pieces[tempSquare] == 7) || (pieces[tempSquare] == 9)) && attackingSide == BLACK) {
					///cout << "isSquareAttacked(): yes. square " << square << " is being attacked by a black rook or queen\n"; 
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
		while (pieces[tempSquare] != -1) {
			if (pieces[tempSquare] != 0) {
				if (((pieces[tempSquare] == 4) || (pieces[tempSquare] == 10)) && attackingSide == WHITE) {
					///cout << "isSquareAttacked(): yes. square " << square << " is being attacked by a white bishop or queen\n";
					return true;
				}
				if (((pieces[tempSquare] == 3) || (pieces[tempSquare] == 9)) && attackingSide == BLACK) {
					///cout << "isSquareAttacked(): yes. square " << square << " is being attacked by a white bishop or queen\n"; 
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
		if (attackingSide == WHITE && pieces[square + KING_MOVEMENTS[i]] == 12) {
			///cout << "isSquareAttacked(): yes. square " << square << " is being attacked by a white king\n"; 
			return true;
		}
		else{
			//If square square is being attacked by a black knight, return true
			if (pieces[square + KING_MOVEMENTS[i]] == 11 && attackingSide == BLACK) {
				///cout << "isSquareAttacked(): yes. square " << square << " is being attacked by a black king\n"; 
				return true;
			}
		}
	}

	///cout << "isSquareAttacked(): no. square " << square << " is not being attacked by anyone\n";
	return false;
}

//Return true if successful
bool BoardStructure::makeMove(Move m) {

	///cout << "makeMove(): Making move (f:" << m.getFromSquare() << " t:" << m.getToSquare() << " c:" << m.getCapturedPiece() << " p: " << m.getPromoted() << ")\n";

	//Store move in next blank element of history[]
	history[historyPly].move = m.move;
	history[historyPly].castlePerms = castlePerms;
	history[historyPly].enPassSquare = enPassSquare;

	///cout << "makeMove(): history[" << historyPly << "].move is:" << history[historyPly].move << "\n";

	historyPly++; ply++;

	//Update enPassSquare
	enPassSquare = 0;

	//Update en pass square
	if (pieces[m.getFromSquare()] == W_PAWN || (pieces[m.getFromSquare()] == B_PAWN)) {
		if (sideToMove == WHITE) {
			enPassSquare = m.getFromSquare() + 10;
		}
		else {
			enPassSquare = m.getFromSquare() - 10;
		}
	}

	//Update castling permissions
	//If a piece moves from a8, a1, e1, or e8, update castling permissions as needed
	castlePerms &= CASTLE_PERMISSIONS[m.getFromSquare()];

	//If a piece moves to a8, a1, e1, or e8, update castling permissions as needed
	if (m.getCastling() != 0) {
		switch (m.getCastling()) { //Take care of castling
		case 1: //white king side
			pieces[25] = 0; pieces[28] = 0; //Clear king and rook spaces
			pieces[27] = W_KING; pieces[26] = W_ROOK; //Place king and rook
			kingSquare[sideToMove] = 27; //Update kingSquare[]
			break;
		case 2: //white queen side
			pieces[25] = 0; pieces[21] = 0;
			pieces[23] = W_KING; pieces[24] = W_ROOK;
			kingSquare[sideToMove] = 23; //Update kingSquare[]
			break;
		case 3: //black king side
			pieces[95] = 0; pieces[98] = 0;
			pieces[97] = B_KING; pieces[96] = B_ROOK;
			kingSquare[sideToMove] = 97; //Update kingSquare[]
			break;
		case 4: //black queen side
			pieces[95] = 0; pieces[91] = 0;
			pieces[93] = B_KING; pieces[94] = B_ROOK;
			kingSquare[sideToMove] = 93; //Update kingSquare[]
			break;
		default: //nothing
			break;
		}
	}
	else {
		//If there is a promoted piece, set the destination square to that. Else, set it to the piece that's moving
		pieces[m.getToSquare()] = (m.getPromoted() != 0) ? m.getPromoted() : pieces[m.getFromSquare()];
		pieces[m.getFromSquare()] = 0; //Clear the square the piece moved from

		//Update king square if the king moved
		if (m.getFromSquare() == kingSquare[sideToMove]) {
			kingSquare[sideToMove] = m.getToSquare();
		}
	}

	///int otherSideToMove = sideToMove ^ 1;
	int currentSideToMove = sideToMove;

	//Flip side to move
	sideToMove ^= 1;

	//Undo move if needed
	///cout << "otherSideToMove:" << otherSideToMove << "\n";
	///cout << "currentSideToMove:" << currentSideToMove << "\n";
	///cout << "kingSquare[currentSideToMove]:" << kingSquare[currentSideToMove] << "\n";

	if (isSquareAttacked(kingSquare[currentSideToMove], sideToMove) == true) {
		//testIsSquareAttacked(otherSideToMove, *this);
		///cout << "king is under attack. undoing a move.\n";
		undoMove();

		return false;
	}

	

	return true;
	}
void BoardStructure::undoMove() {

	ply--; historyPly--;

	//Get move in last non-blank element of history[] and revert castlePerms and enPassSquare
	int move, from, to, captured, promoted, castling;

	///cout << "undoMove(): history[" << historyPly << "].move is:" << history[historyPly].move << "\n";

	move = history[historyPly].move;

	//Revert castlePerms and enPassSquare
	castlePerms = history[historyPly].castlePerms;
	enPassSquare = history[historyPly].enPassSquare;

	from = move & 0x7F;
	to = (move >> 7) & 0x7F;
	captured = (move >> 14) & 0xF;
	promoted = (move >> 18) & 0x7F;
	castling = (move >> 22) & 0x7;

	//
	///cout << "undoMove(): Undoing f:" << from << " t:" << to << " c:" << captured << " p: " << promoted << ")\n";


	//Flip side
	sideToMove ^= 1;

	//Undo castling, if any
	if (castling != 0) {
		switch (castling) { 
		case 1: //white king side
			pieces[25] = W_KING; pieces[28] = W_ROOK; //Place king and rook
			kingSquare[sideToMove] = 25; //Update kingSquare[]
			break;
		case 2: //white queen side
			pieces[25] = W_KING; pieces[21] = W_ROOK;
			kingSquare[sideToMove] = 25; //Update kingSquare[]
			break;
		case 3: //black king side
			pieces[95] = B_KING; pieces[98] = B_ROOK;
			kingSquare[sideToMove] = 95; //Update kingSquare[]
			break;
		case 4: //black queen side
			pieces[95] = B_KING; pieces[91] = B_ROOK;
			kingSquare[sideToMove] = 95; //Update kingSquare[]
			break;
		default: //nothing
			break;
		}

		///cout << "undoMove(): Undoing castling\n";
	}
	else {

		//Revert promotion, if any
		if (promoted != 0) {
			pieces[to] = 0;

			if (sideToMove == WHITE) {
				pieces[from] = W_PAWN;
			}
			else {
				pieces[from] = B_PAWN;
	}

			///cout << "undoMove(): Undoing promotion\n";
}

		//Revert movement of pieces, if not a promotion or castling
		else {
			///cout << "undoMove(): pieces[from] is:" << pieces[from] << ",pieces[to] is:" << pieces[to] << "\n";
			pieces[from] = pieces[to];

			//If capture, bring captured piece back to life
			if (captured != 0) {
				pieces[to] = captured;
			}
			else {
				pieces[to] = 0;
	}

			///cout << "undoMove(): undid a regular move\n";
}

		//Revert king square if the king moved
		if (to == kingSquare[sideToMove]) {
			kingSquare[sideToMove] = from;
	}
}


}

void BoardStructure::countPieces(){
	//Initialize pieceCount[] array
	for (int i = 0; i < 13; i++){
		pieceCount[i] = 0;
	}

	//Loop through the entire board
	for (int i = 0; i < BOARD_SQUARE_COUNT; i++){
		switch (pieces[i]){
		case EMPTY: pieceCount[EMPTY]++; break;
		case B_PAWN: pieceCount[B_PAWN]++; break;
		case W_PAWN: pieceCount[W_PAWN]++; break;
		case B_BISHOP: pieceCount[B_BISHOP]++; break;
		case W_BISHOP: pieceCount[W_BISHOP]++; break;
		case B_KNIGHT: pieceCount[B_KNIGHT]++; break;
		case W_KNIGHT: pieceCount[W_KNIGHT]++; break;
		case B_ROOK: pieceCount[B_ROOK]++; break;
		case W_ROOK: pieceCount[W_ROOK]++; break;
		case B_QUEEN: pieceCount[B_QUEEN]++; break;
		case W_QUEEN: pieceCount[W_QUEEN]++; break;
		case B_KING: pieceCount[B_KING]++; break;
		case W_KING: pieceCount[W_KING]++; break;
		}
	}
}