#include "defs.h"
#include <iostream>
#include <string>
using namespace std;

BoardStructure board;

//Program execution starts here
int main() {
	cout << "Hello. My name is " << NAME << ".\n";

	string choice;
	do {
		cout << "Would you like to go first? (y/n) \n";
		getline(cin, choice);
	} while (choice.compare("y") != 0 && choice.compare("n") != 0);

	//Output the starting position
	board.init(choice.compare("y") == 0); board.displayBoard(); 
	cout << "\nI will now output some random board positions... (enter anything to continue)\n";  getline(cin, choice);

	//Output an empty board
	board.resetBoardToEmpty(); board.displayBoard(); getline(cin, choice);

	//Output a position with two queens and two pawns
	if (board.setUpBoardUsingFEN("8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 1 ") == -1) {
		return -1;
	}

	board.displayBoard(); testIsSquareAttacked(WHITE, board); testIsSquareAttacked(BLACK, board); getline(cin, choice);

	//Output the first parts of a game from http://en.lichess.org/NgHuzc5J ...
	if (board.setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == -1) {
		return -1;
	}

	board.displayBoard(); testIsSquareAttacked(WHITE, board); testIsSquareAttacked(BLACK, board); getline(cin, choice);

	//e4
	if (board.setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1") == -1) {
		return -1;
	}

	board.displayBoard(); testIsSquareAttacked(WHITE, board); testIsSquareAttacked(BLACK, board); getline(cin, choice);

	//d5
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2") == -1) {
		return -1;
	}

	board.displayBoard(); testIsSquareAttacked(WHITE, board); testIsSquareAttacked(BLACK, board); getline(cin, choice);

	//d4?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp1pppp/8/3p4/3PP3/8/PPP2PPP/RNBQKBNR b KQkq d3 0 2") == -1) {
		return -1;
	}

	board.displayBoard(); testIsSquareAttacked(WHITE, board); testIsSquareAttacked(BLACK, board);  getline(cin, choice);

	//e6?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp2ppp/4p3/3p4/3PP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 3") == -1) {
		return -1;
	}

	board.displayBoard(); testIsSquareAttacked(WHITE, board); testIsSquareAttacked(BLACK, board); getline(cin, choice);

	//c4?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp2ppp/4p3/3p4/2PPP3/8/PP3PPP/RNBQKBNR b KQkq c3 0 3") == -1) {
		return -1;
	}

	board.displayBoard(); testIsSquareAttacked(WHITE, board); testIsSquareAttacked(BLACK, board); getline(cin, choice);

	//c6?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/pp3ppp/2p1p3/3p4/2PPP3/8/PP3PPP/RNBQKBNR w KQkq - 0 4") == -1) {
		return -1;
	}

	string fen; 
	board.displayBoard(); 
	cout << "\nGive me a valid FEN string:\n";
	getline(cin, fen);

	while (42 == 42) {
		if (board.setUpBoardUsingFEN(&fen[0u]) == -1) {
			return -1;
		}
		board.displayBoard(); testIsSquareAttacked(WHITE, board); testIsSquareAttacked(BLACK, board);
		cout << "\nGive me a valid FEN string:\n";  
		getline(cin, fen);
	}

	return 0;
}
