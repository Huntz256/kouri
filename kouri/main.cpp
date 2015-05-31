#include "defs.h"
#include <iostream>
#include <string>
#include <random>
#include <ctime>
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

//Look at various positions
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
	cout << "Hello. My name is " << NAME << ".\n";

	string in; cout << "Type \"1\" to run function1, \"2\" to run function2, \"3\" to run function3, or \"4\" to run function4.\n";
	getline(cin, in);

	if (in.compare("1") == 0) { 
		return testFunction1(); 
	}
	else if (in.compare("2") == 0) { 
		testFunction2(); 
	}
	else if (in.compare("3") == 0){
		testFunction3();
	}
	else {
		testFunction4();
	}

	return 0;
}
