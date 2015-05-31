#include "defs.h"
#include <iostream>
#include <string>
#include <random>
#include <ctime>
using namespace std;

BoardStructure board; MoveList movelist;


int getRandomInteger(int min, int max) {
	srand(time(NULL)); 
	return rand() % (max - min + 1) + min;
}
int testFunction1(){
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

	board.displayBoard(); 
	movelist.generateMoveList(board); movelist.printMoveList(board);
	getline(cin, choice);

	//Output the first parts of a game from http://en.lichess.org/NgHuzc5J ...
	if (board.setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == -1) {
		return -1;
	}

	board.displayBoard(); 
	movelist.generateMoveList(board); movelist.printMoveList(board);
	getline(cin, choice);

	//e4
	if (board.setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1") == -1) {
		return -1;
	}

	board.displayBoard(); 
	movelist.generateMoveList(board); movelist.printMoveList(board);
	getline(cin, choice);

	//d5
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2") == -1) {
		return -1;
	}

	board.displayBoard();
	movelist.generateMoveList(board); movelist.printMoveList(board);
	getline(cin, choice);

	//d4?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp1pppp/8/3p4/3PP3/8/PPP2PPP/RNBQKBNR b KQkq d3 0 2") == -1) {
		return -1;
	}

	board.displayBoard(); 
	movelist.generateMoveList(board); movelist.printMoveList(board);
	getline(cin, choice);

	//e6?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp2ppp/4p3/3p4/3PP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 3") == -1) {
		return -1;
	}

	board.displayBoard(); 
	movelist.generateMoveList(board); movelist.printMoveList(board);
	getline(cin, choice);

	//c4?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/ppp2ppp/4p3/3p4/2PPP3/8/PP3PPP/RNBQKBNR b KQkq c3 0 3") == -1) {
		return -1;
	}

	board.displayBoard(); 
	movelist.generateMoveList(board); movelist.printMoveList(board);
	getline(cin, choice);

	//c6?!
	if (board.setUpBoardUsingFEN("rnbqkbnr/pp3ppp/2p1p3/3p4/2PPP3/8/PP3PPP/RNBQKBNR w KQkq - 0 4") == -1) {
		return -1;
	}

	Move move; move.move = 0; int fromSquare = 6, toSquare = 12, capturedPiece = 8, promote = 7;
	move.move = MOVE(fromSquare, toSquare, capturedPiece, promote, 0);
	cout << "from:" << move.getFromSquare() << " to:" << move.getToSquare() << " cap:" << move.getCapturedPiece() << " prom:" << move.getPromoted();


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

void testFunction2() {

	//Initatize board to starting position
	board.init(true); Move m;

	//Display the board and first 5 elements of history
	board.displayBoard(); cout << "\n";
	for (int i = 0; i < 5; i++) {
		cout << board.history[i].move << " ";
	}
	cout << "\n";

	//Make move 35 to 55 (e2e4)
	m.move = MOVE(35, 55, 0, 0, 0);
	board.makeMove(m);

	//Output fromSquare and toSquare board indices
	cout << "from: " << m.getFromSquare() << " to: " << m.getToSquare() << "\n";

	//Display the board and first 5 elements of history
	board.displayBoard(); cout << "\n";
	for (int i = 0; i < 5; i++) {
		cout << board.history[i].move << " ";
	}
	
	//Make move 84 to 64 (d7d5)
	m.move = MOVE(84, 64, 0, 0, 0);
	board.makeMove(m);

	//Output move's from square and to square
	cout << "from: " << m.getFromSquare() << " to: " << m.getToSquare() << "\n";
	
	//Display the board and first 5 elements of history
	board.displayBoard(); cout << "\n";
	for (int i = 0; i < 5; i++){
		cout << board.history[i].move << " ";
	}

	cout << "\n";
}

//Has kouri play against itself
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

//Program execution starts here
int main() {
	cout << "Hello. My name is " << NAME << ".\n";

	//return testFunction1();
	//testFunction2();
	testFunction3();

	int x; cin >> x;

	return 0;
}
