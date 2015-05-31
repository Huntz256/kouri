#include "defs.h"
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <sstream>
using namespace std;

BoardStructure board; MoveListGenerator movelist;


int getRandomInteger(int min, int max) {
	srand(time(NULL)); 
	return rand() % (max - min + 1) + min;
}

//Make a move from the move list using user input
void playerMove() {
	int x;
	do {
		cout << "\n\nIt's your turn. Choose a move from the list above and enter the move number:";
		cin >> x;
		board.makeMove(movelist.moves[x]);
	} while ((x < 0) || (x >= movelist.numberOfMoves));
}

//Parse an FEN string
int testFunction1(){

	string fen;
	board.displayBoard();
	cout << "\nGive me a valid FEN string:\n";
	getline(cin, fen);

	while (42 == 42) {
		if (board.setUpBoardUsingFEN(&fen[0u]) == -1) {
			return -1;
		}
		board.displayBoard(); testIsSquareAttacked(WHITE, board); testIsSquareAttacked(BLACK, board);
		movelist.generateMoveList(board); movelist.printMoveList(board);
		cout << "\nGive me a valid FEN string:\n";
		getline(cin, fen);
	}
}

//Castling testing
int testFunction2() {

	Move m; string next;

	//Set up board for castling tests
	cout << "\nTesting white castling king side\n";
	if (board.setUpBoardUsingFEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
		return -1;
	}
	board.displayBoard(); cout << "\n";
	cout << "Press enter to continue:";
	getline(cin, next);

	//White castle king side (e1g1)
	m.move = MOVE(25, 27, 0, 0, 1);
	board.makeMove(m);
	//Output fromSquare and toSquare board indices
	cout << "from: " << m.getFromSquare() << " to: " << m.getToSquare() << "\n\n";

	//Display the board
	board.displayBoard(); cout << "\n";

	cout << "Press enter to continue:";
	getline(cin, next);
	
	/******************New test*********************/
	cout << "\nTesting white castling queen side\n";
	if (board.setUpBoardUsingFEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
		return -1;
	}
	board.displayBoard(); cout << "\n";
	cout << "Press enter to continue:";
	getline(cin, next);
	
	//White castle queen side (d7d5)
	m.move = MOVE(25, 23, 0, 0, 2);
	board.makeMove(m);
	//Output move's from square and to square
	cout << "from: " << m.getFromSquare() << " to: " << m.getToSquare() << "\n\n";
	
	//Display the board
	board.displayBoard(); cout << "\n";

	cout << "Press enter to continue:";
	getline(cin, next);

	/******************New test*********************/
	cout << "\nTesting black castling king side\n";
	if (board.setUpBoardUsingFEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
		return -1;
	}
	board.displayBoard(); cout << "\n";
	cout << "Press enter to continue:";
	getline(cin, next);

	//White castle queen side (d7d5)
	m.move = MOVE(95, 97, 0, 0, 3);
	board.makeMove(m);
	//Output move's from square and to square
	cout << "from: " << m.getFromSquare() << " to: " << m.getToSquare() << "\n\n";
	
	//Display the board
	board.displayBoard(); cout << "\n";

	cout << "Press enter to continue:";
	getline(cin, next);

	/******************New test*********************/
	cout << "\nTesting black castling queen side\n";
	if (board.setUpBoardUsingFEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
		return -1;
	}
	board.displayBoard(); cout << "\n";
	cout << "Press enter to continue:";
	getline(cin, next);

	//White castle queen side (d7d5)
	m.move = MOVE(95, 93, 0, 0, 4);
	board.makeMove(m);
	//Output move's from square and to square
	cout << "from: " << m.getFromSquare() << " to: " << m.getToSquare() << "\n\n";

	//Display the board
	board.displayBoard(); cout << "\n";

	cout << "Press enter to continue:";
	getline(cin, next);

	return 0;
}

//Have kouri play against itself
void testFunction3() {
	string x; board.init(true);

	while (42 == 42) {
		movelist.generateMoveList(board);
		int moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);
		board.makeMove(movelist.moves[moveNum]);

		board.displayBoard();
		movelist.printMoveList(board);
		
		cout << "\n\n" << NAME << " has decided to make move " << moveNum << "!";
		board.sideToMove = board.sideToMove ^ 1;
		
		getline(cin, x);
	}
}

//Play against kouri
void testFunction4() {
	string x; board.init(true);

	while (42 == 42) {
		movelist.generateMoveList(board);

		board.displayBoard();
		movelist.printMoveList(board);

		playerMove();
		board.sideToMove = board.sideToMove ^ 1;

		movelist.generateMoveList(board);
		int moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);
		board.makeMove(movelist.moves[moveNum]);

		board.displayBoard();
		movelist.printMoveList(board);

		cout << "\n\nI, " << NAME << ", have decided to make move " << moveNum << ".";
		board.sideToMove = board.sideToMove ^ 1;

	}
}

//Program execution starts here
int main() {
	string t, c;

	do {
		if (!getline(cin, c)) {
			c = "quit";
		}

		istringstream is(c);
		t.clear();
		is >> skipws >> t;

		if (t == "uci") {
			cout << "id name " << NAME << "\n";
			cout << "id author Minter\n";
			cout << "uciok\n";
		}
		else if (t == "isready") {
			cout << "readyok\n";
			board.init(true);
			
		}
		else if (t == "go") {
			movelist.generateMoveList(board);
			int moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);
			board.makeMove(movelist.moves[moveNum]);
			cout << "bestmove ";
			movelist.uciPrintMove(board, moveNum);
			board.sideToMove = board.sideToMove ^ 1;
		}

	} while (t != "quit");

	return 0;
}
