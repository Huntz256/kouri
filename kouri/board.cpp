#include "defs.h"
#include <iostream>
#include <unordered_map>
#include <fstream>
using namespace std;

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
	15, 7, 15, 15, 15, 3, 15, 15, 11, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

//Represents where a knight/rook/bishop/queeen/king can move
const int KNIGHT_MOVEMENTS[8] = { -19, -8, 12, 21, 19, 8, -12, -21 };
const int ROOK_MOVEMENTS[4] = { -10, 1, 10, -1 };
const int BISHOP_MOVEMENTS[4] = { -9, 11, 9, -11 };
const int KING_MOVEMENTS[8] = { -10, -9, 1, 11, 10, 9, -1, -11 };

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
		pieceMap[B_KING] = 'k'; pieceMap[W_KING] = 'K';

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

//Outputs full 10x12 board to console
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

//Outputs 8x8 board to console
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
	///cout << "Enpass. square: " << enPassSquare << '\n';
	cout << "evaluate(): " << ai.evaluate(*this) << '\n';
}

//Sets up the board for a standard chess match
void BoardStructure::init(bool goFirst) {
	if (goFirst) {
		setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	}
	else {
		setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
	}
}

//Resets the board
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

	//Reset side to move 
	sideToMove = WHITE;

	//Reset position ID by setting all bits to zero
	positionID = 0ULL;

	//Init pv table
	table.initPVTable();
}

//Sets up pieces given a FEN string. Returns 0 if successful.
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

	//Generate position id
	positionID = generateAndGetPositionID();

	//Update material[] and kingSquare[]
	for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {
		if (pieces[i] > 0) {
			int color = getPieceColor(pieces[i]);
			material[color] += PIECE_VALUE[pieces[i]];
			pieceCount[pieces[i]]++;

			if (pieces[i] == W_KING) {
				kingSquare[WHITE] = i;
			}
			else if (pieces[i] == B_KING) {
				kingSquare[BLACK] = i;
			}

		}
	}

	return 0;
}

//Retrieves the color of a piece
int BoardStructure::getPieceColor(int pieceNum) {
	switch (pieceNum) {
	case B_PAWN: case B_BISHOP: case B_KNIGHT: case B_ROOK: case B_QUEEN: case B_KING:
		return BLACK; break;
	case W_PAWN: case W_BISHOP: case W_KNIGHT: case W_ROOK: case W_QUEEN: case W_KING:
		return WHITE; break;
	default:
		///cout << "ERROR: INVALID PIECE COLOR. pieceNum: " << pieceNum << "\n";
		return -42; break;
	}
}

//Displays all the moves so far as move integers
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

//Returns true if square square is being attacked by a piece from the side attackingSide
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

//Remove the piece from square square and update variables as needed
void BoardStructure::removePieceFromSquare(int square) {
	//Update positionID, material, and pieceCount
	positionID ^= pieceSquareKey[pieces[square]][square];
	material[getPieceColor(pieces[square])] -= PIECE_VALUE[pieces[square]];
	pieceCount[pieces[square]]--;

	//Set square to EMPTY
	pieces[square] = EMPTY;
}

//Add piece piece to square square and update variables as needed
void BoardStructure::addPieceToSquare(int square, int piece) {
	//Update positionID, material, and pieceCount
	positionID ^= pieceSquareKey[pieces[square]][square];
	material[getPieceColor(pieces[square])] += PIECE_VALUE[pieces[square]];
	pieceCount[pieces[square]]++;

	//Set square to the piece
	pieces[square] = piece;
}

//Move piece piece from square fromSquare to square toSquare
void BoardStructure::movePieceToSquare(int fromSquare, int toSquare) {
	int piece = pieces[fromSquare];

	//Update positionID and set toSquare to the piece and set fromSquare to EMPTY
	positionID ^= pieceSquareKey[pieces[fromSquare]][fromSquare];
	pieces[fromSquare] = EMPTY;
	positionID ^= pieceSquareKey[pieces[toSquare]][toSquare];
	pieces[toSquare] = piece;
}

//Modifies the board and stores the move in history[]. 
//Returns false if after making the move the side making the move leaves themselves in check,
//or in other cases when the move is invalid
bool BoardStructure::makeMove(Move m) {

	//Inits and validations before we even attempt to make the move--------------------------------------	
	int fromSquare = m.getFromSquare();
	int toSquare = m.getToSquare();
	int capturedPiece = m.getCapturedPiece();
	int castling = m.getCastling();
	int promotedPiece = m.getPromoted();
	int side = sideToMove;
	///cout << "Making move " << fromSquare << "-" << toSquare << "-" << capturedPiece << "-" << castling << "-" << promotedPiece << "\n";

	//If board is not valid, show an error message
	if (!isBoardValid()) {
		cout << "\nERROR: BOARD NOT VALID.\n";
		return false;
	}

	//If piece on fromSquare is not valid, show an error message
	if ((pieces[fromSquare] < B_PAWN) || (pieces[fromSquare] > W_KING)) {
		cout << "\nERROR: PIECE NUMBER " << pieces[fromSquare] << " ON SQUARE " << fromSquare << " IS NOT VALID.\n";
		return false;
	}

	//If square fromSquare or toSquare is not on the board, show an error message
	if (FILES[fromSquare] == -1) {
		cout << "\nERROR: FROMSQUARE " << fromSquare << " IS NOT VALID.\n";
		return false;
	}
	if (FILES[toSquare] == -1) {
		cout << "\nERROR: TOSQUARE " << toSquare << " IS NOT VALID.\n"; 
		return false;
	}

	//End of validations -------------------------------------------------------------------------
	
	//Start of performing the special movement portions of special moves (castling, en passant) -----------------------------

	//Update history[]'s position ID
	history[historyPly].positionID = positionID;

	//If en passant move, make the en passant portion (i.e. clearing a pawn) of the move
	    //to do later

	//If castling move, make the rook movement portion of the castling move
	if (castling != 0) {
		switch (toSquare) {
			//White Kingside Castling
			case 27:
				movePieceToSquare(28, 26); break;
			//White Queenside Castling
			case 23:
				movePieceToSquare(21, 24); break;
			//Black Kingside Castling
			case 97:
				movePieceToSquare(98, 96); break;
			//Black Queenside Castling
			case 93:
				movePieceToSquare(91, 94); break;
			default:
				cout << "\nERROR: INVALID CASTILING TOSQUARE.\n"; break;
		}
	}

	//End of performing the special movement portions of special moves (castling, en passant) -----------------------------

	//Update history[]
	history[historyPly].move = m.move;
	history[historyPly].castlePerms = castlePerms;
	history[historyPly].enPassSquare = enPassSquare;
	
	//Update castling permissions
	//If a piece moves from a8, a1, e1, or e8, update castling permissions as needed
	castlePerms &= CASTLE_PERMISSIONS[fromSquare];
	castlePerms &= CASTLE_PERMISSIONS[toSquare];

	//Update enPassSquare to 0, as the enpassant portion of the move has already been made
	enPassSquare = 0;

	//If a capturing move, remove captured piece
	if (capturedPiece != EMPTY) {
		removePieceFromSquare(toSquare);
	}

	//Update historyPly and ply
	historyPly++; ply++;

	//Update en pass square if needed
	/*if (pieces[m.getFromSquare()] == W_PAWN || (pieces[m.getFromSquare()] == B_PAWN)) {
		if (sideToMove == WHITE) {
			enPassSquare = m.getFromSquare() + 10;
		}
		else {
			enPassSquare = m.getFromSquare() - 10;
		}
	}*/

	//Now that the captured piece is removed, we can now actually move the piece we want to move
	movePieceToSquare(fromSquare, toSquare);

	//If this is a promotion move, then promote the pawn
	if (castling == 0 && promotedPiece != 0) {
		removePieceFromSquare(toSquare);
		addPieceToSquare(toSquare, promotedPiece);
	}

	//Update king square if the king moved
	if ((toSquare == W_KING) || (toSquare == B_KING)) {
		kingSquare[sideToMove] = toSquare;
	}
	
	//Flip side to move and update positionID
	sideToMove ^= 1; positionID = generateAndGetPositionID();

	//Another validation. If board is not valid, show an error message
	if (!isBoardValid()) {
		///cout << "\nERROR: BOARD NOT VALID.\n";
		//return false;
	}

	//If the king is under attack (in check) now, undo the move and return false
	if (isSquareAttacked(kingSquare[side], sideToMove) == true) {
		undoMove();
		return false;
	}
	
	return true;
}

//Undoes the last move
void BoardStructure::undoMove() {

	//If board is not valid, show an error message
	if (!isBoardValid()) {
		///cout << "\nERROR: BOARD NOT VALID.\n";
	}

	//Update ply and historyPly
	ply--; historyPly--;

	//Init variables
	int m = history[historyPly].move;
	int fromSquare = m & 0x7F;
	int toSquare = (m >> 7) & 0x7F;
	int captured = (m >> 14) & 0xF;
	int promotedPiece = (m >> 18) & 0x7F;
	int castling = (m >> 22) & 0x7;

	//If square fromSquare or toSquare is not on the board, output an error message
	if (FILES[fromSquare] == -1) {
		cout << "\nERROR: FROMSQUARE " << fromSquare << " IS NOT VALID.\n"; 
	}
	if (FILES[toSquare] == -1) {
		cout << "\nERROR: TOSQUARE " << toSquare << " IS NOT VALID.\n";
	}

	//Revert castlePerms and enPassSquare
	castlePerms = history[historyPly].castlePerms;
	enPassSquare = history[historyPly].enPassSquare;

	//Flip side to move and update positionID
	sideToMove ^= 1; positionID = generateAndGetPositionID();

	//If en passant move, undo the en passant portion (i.e. clearing a pawn) of the move
	    //to do later

	//If castling move, undo the rook movement portion of the castling move
	//In other words move the rooks back to their home squares if needed
	if (castling != 0) {
		switch (toSquare) {
			//White Kingside Castling
		case 27:
			movePieceToSquare(26, 28); break;
			//White Queenside Castling
		case 23:
			movePieceToSquare(24, 21); break;
			//Black Kingside Castling
		case 97:
			movePieceToSquare(96, 98); break;
			//Black Queenside Castling
		case 93:
			movePieceToSquare(94, 91); break;
		default:
			cout << "\nERROR: INVALID CASTILING TOSQUARE.\n"; break;
		}
	}

	//Move piece back to fromSquare.
	movePieceToSquare(toSquare, fromSquare);

	//Update KingSq if the king moved
	if ((fromSquare == W_KING) || (fromSquare == B_KING)) {
		kingSquare[sideToMove] = fromSquare;
	}

	//Then, undo clearing of piece due to capture if there was a capture
	if (captured != EMPTY) {
		addPieceToSquare(toSquare, captured);
	}

	//Then, undo promotion if there was a promotion
	if (promotedPiece != EMPTY) {
		removePieceFromSquare(fromSquare);
		addPieceToSquare(fromSquare, ((getPieceColor(pieces[fromSquare]) == WHITE) ? W_PAWN : B_PAWN));
	}

	//Update posiition ID
	positionID = generateAndGetPositionID();

	//If board is not valid, show an error message
	if (!isBoardValid()) {
		///cout << "\nERROR: BOARD NOT VALID.\n"; 
	}

}

// Counts all the pieces on the board and records them in the pieceCount[] array
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

//Has this position occured before in the game? If yes, return true. Used for checking threefold repetition.
bool BoardStructure::isRepetition() {
	for (int i = 0; i < historyPly - 1; i++) {
		if (positionID == history[i].positionID) {
			return true;
		}
	}
	return false;
}

//Generate and return a position id representing this board's position
U64 BoardStructure::generateAndGetPositionID() {
	U64 id = 0;

	//Go through each square on the board
	for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {

		//If the square contains a piece, modify id 
		if (pieces[i] > 0) {

			//Set id to id XOR pieceSquareKey
			//cout << "i:" << i << " pieces[i]: " << pieces[i] << "\n";
			id ^= pieceSquareKey[ pieces[i] ][i];
		}
	}
	
	//Modify id if white to move
	if (sideToMove == WHITE) {
		id ^= sideKey;
	}

	//Modify id if en passant square exists
	if (enPassSquare != EMPTY) {
		id ^= pieceSquareKey[EMPTY][enPassSquare];
	}

	//Modify id using castling permissions
	id ^= castlePermKey[castlePerms];

	///cout << "\nPosition ID generated: " << id << "\n";

	return id;
}

//Return false if there is something wrong with the current board
bool BoardStructure::isBoardValid() {

	//Make sure position ID is good
	if (positionID != generateAndGetPositionID()) {
		cout << "ERROR: POSITION ID " << positionID << " INVALID\n";
		return false;
	}

	//Make sure en pass square is valid
	if (enPassSquare != 0 && RANKS[enPassSquare] != RANK_6 && RANKS[enPassSquare] != RANK_3) {
		cout << "ERROR: EN PASS SQUARE," << enPassSquare << " IS INVALID\n";
		return false;
	}

	//Make sure KingSq arrays are valid
	if (pieces[kingSquare[WHITE]] != W_KING) {
		//cout << "ERROR: pieces[kingSquare[WHITE]]  pieces[" << kingSquare[WHITE] << "] = " << pieces[kingSquare[WHITE]] << " WHICH IS NOT A WHITE KING.\n";
		//return false;
	}
	if (pieces[kingSquare[BLACK]] != B_KING) {
		//cout << "ERROR: pieces[kingSquare[BLACK]] = pieces[" << kingSquare[BLACK] << "] = " <<  pieces[kingSquare[BLACK]] << " WHICH IS NOT A BLACK KING.\n";
		//return false;
	}
	

	//Make sure side makes sense
	if ((sideToMove != WHITE) && (sideToMove != BLACK)) {
		cout << "ERROR: SIDE " << sideToMove << " IS INVALID.\n"; 
		return false;
	}

	//Return true otherwise
	return true;
}

