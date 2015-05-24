#include "defs.h"
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;


//Returns a character representing a piece, given a piece number.
char numToPiece(int num) {
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
		setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w");
	}
	else {
		setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b");
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

	//Go through each character in the fen string
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

	sideToMove = (*fen == 'w') ? 0 : 1;

	return 0;
}


//Program execution starts here
int main() {
	cout << "Hello. My name is " << NAME << ".\n";
	
	string choice;
	do {
		cout << "Would you like to go first? (y/n) \n";
		getline(cin, choice);
	} while (choice.compare("y") != 0 && choice.compare("n") != 0);
	
	//Output the starting position
	board.init(choice.compare("y") == 0); board.displayBoard(); getline(cin, choice);

	//Output an empty board
	board.resetBoardToEmpty(); board.displayBoard(); getline(cin, choice);

	//Output the first parts of a game from http://en.lichess.org/NgHuzc5J ...
	if (board.setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == -1) {
		return -1;
	}

	board.displayBoard(); getline(cin, choice);

	//e4
	if (board.setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1") == -1) {
		return -1;
	}

	board.displayBoard(); getline(cin, choice);

	//d5
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2") == -1) {
		return -1;
	}

	board.displayBoard(); getline(cin, choice);

	//d4?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp1pppp/8/3p4/3PP3/8/PPP2PPP/RNBQKBNR b KQkq d3 0 2") == -1) {
		return -1;
	}

	board.displayBoard(); getline(cin, choice);

	//e6?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp2ppp/4p3/3p4/3PP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 3") == -1) {
		return -1;
	}

	board.displayBoard(); getline(cin, choice);

	//c4?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp2ppp/4p3/3p4/2PPP3/8/PP3PPP/RNBQKBNR b KQkq c3 0 3") == -1) {
		return -1;
	}

	board.displayBoard(); getline(cin, choice);

	//c6?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/pp3ppp/2p1p3/3p4/2PPP3/8/PP3PPP/RNBQKBNR w KQkq - 0 4") == -1) {
		return -1;
	}

	board.displayBoard(); getline(cin, choice);

	return 0;
}


