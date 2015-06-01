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
		board.makeMove(movelist.movesLegal[x]);
	} while ((x < 0) || (x >= movelist.numberOfMovesLegal));
}

//Assumes input is a move command. Returns move integer if valid, else returns -1
int translateMoveCommand(string com){
	//castling
	if (com.length() == 3 && board.sideToMove == WHITE && com.compare("O-O") == 0) return MOVE(25, 27, 0, 0, 1);
	if (com.length() == 5 && board.sideToMove == WHITE && com.compare("O-O-O") == 0) return MOVE(25, 23, 0, 0, 2);
	if (com.length() == 3 && board.sideToMove == BLACK && com.compare("O-O") == 0) return MOVE(95, 97, 0, 0, 3);
	if (com.length() == 5 && board.sideToMove == BLACK && com.compare("O-O-O") == 0) return MOVE(95, 93, 0, 0, 4);
	
	//regular move with possible promotion
	if (com.length() >= 8 && (com[0] - 'a' >= 0 && com[0] - 'a' <= 7) && (com[6] - 'a' >= 0 && com[6] - 'a' <= 7) && (com[1] >= '1' && com[1] <= '8') && 
		(com[7] >= '1' && com[7] <= '8') && (com.substr(2,4).compare(" to ") == 0)){
		int from = (com[0] - 'a' + 1) + (com[1] - '1' + 2) * 10;
		int to = (com[6] - 'a' + 1) + (com[7] - '1' + 2) * 10;
		int prom = (com.length() == 10) ? charToPieceInt(com[9]) : 0;
		return MOVE(from, to, board.pieces[to], prom, 0);
	}
	
	return -1;
}

//Checks if a move integer is contained in the generated movelist
bool isMoveValid(int move){
	for (int i = 0; i < movelist.numberOfMovesLegal; i++){
		if (move == movelist.movesLegal[i].move) return true;
	}
	return false;
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
		int moveNum = getRandomInteger(0, movelist.numberOfMovesLegal - 1);
		while (!board.makeMove(movelist.movesLegal[moveNum])) {
			moveNum = getRandomInteger(0, movelist.numberOfMovesLegal - 1);
		}

		board.displayBoard();
		movelist.printMoveList(board);
		
		cout << "\n\n" << NAME << " has decided to make move " << moveNum << "!";
		board.sideToMove = board.sideToMove ^ 1;
		
		getline(cin, x);
	}
}

//Play against kouri by selecting from movelist
void testFunction4() {
	string x; board.init(true);

	while (42 == 42) {
		movelist.generateMoveList(board);

		board.displayBoard();
		movelist.printMoveList(board);

		playerMove();

		movelist.generateMoveList(board);
		int moveNum = getRandomInteger(0, movelist.numberOfMovesLegal - 1);
		
		while (!board.makeMove(movelist.movesLegal[moveNum])) {
			moveNum = getRandomInteger(0, movelist.numberOfMovesLegal - 1);
			///cout << "testFunction4(): Getting another moveNum: " << moveNum;
		}

		board.displayBoard();
		movelist.printMoveList(board);
		

		cout << "\n\nI, " << NAME << ", have decided to make move " << moveNum << ".";

	}
}

//Play against kouri without a movelist
void testFunction5() {
	string x; Move m;
	board.init(true);

	string help = "To make a move, type a command in the form: a1 to b2 \nPawn promotions are done as such: a1 to b2=Q \nCastling is done using: O-O or O-O-O \nFor a movelist, type: hint \nTo forfeit, type: f ";

	while (true) {
		movelist.generateMoveList(board);

		board.displayBoard();

		cout << "For a list of commands, type: help \nEnter your command: ";
		getline(cin, x);

		//While user command is not valid
		while (!(isMoveValid(translateMoveCommand(x)) || x.compare("f") == 0) || x.compare("hint") == 0 || x.compare("help") == 0) {
			if (x.compare("help") != 0 && x.compare("hint") != 0){
				cout << "\n\nThat is not a valid move or command. For a list of commands, type: help \n";
			}

			board.displayBoard();

			if (x.compare("hint") == 0) movelist.printMoveList(board);
			else if (x.compare("f") == 0) break;
			else if (x.compare("help") == 0) cout << help << "\n";

			cout << "Enter your command: ";
			getline(cin, x);
		}

		if (x.compare("f") == 0) break; //exit game loop

		m.move = translateMoveCommand(x);

		while ( !board.makeMove(m) ) {
			m.move = translateMoveCommand(x);
		}
	

		movelist.generateMoveList(board);
		int moveNum = getRandomInteger(0, movelist.numberOfMovesLegal - 1);

		while (!board.makeMove(movelist.movesLegal[moveNum])) {
			moveNum = getRandomInteger(0, movelist.numberOfMovesLegal - 1);
		}

		board.displayBoard();
		movelist.printMoveList(board);

		cout << "\n\nI, " << NAME << ", have decided to make move " << moveNum << ".";

	}
}

//Does a small promotion test
void testFunction20(){
	board.setUpBoardUsingFEN("rnbqk2r/ppppppPp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	board.displayBoard();
	Move m; m.move = MOVE(87, 98, B_ROOK, W_QUEEN, 0);
	board.makeMove(m);
	board.displayBoard();

	string x; getline(cin, x);
}

//Board evaluation tests
void testFunction21(){
	string x;
	board.setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	board.displayBoard();
	cout << "White: " << evaluate(board) << "\n";
	cout << "Black: " << evaluate(board) << "\n";
	getline(cin, x);

	board.setUpBoardUsingFEN("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	board.displayBoard();
	cout << "White: " << evaluate(board) << "\n";
	cout << "Black: " << evaluate(board) << "\n";
	getline(cin, x);

	board.setUpBoardUsingFEN("rnbqkbnr/8/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	board.displayBoard();
	cout << "White: " << evaluate(board) << "\n";
	cout << "Black: " << evaluate(board) << "\n";
	getline(cin, x);

	board.setUpBoardUsingFEN("8/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	board.displayBoard();
	cout << "White: " << evaluate(board) << "\n";
	cout << "Black: " << evaluate(board) << "\n";
	getline(cin, x);

	board.setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1");
	board.displayBoard();
	cout << "White: " << evaluate(board) << "\n";
	cout << "Black: " << evaluate(board) << "\n";
	getline(cin, x);
}

//Play against AI-enabled kouri
void testFunction22(){
	string x; string choice; Move m;

	string help = "To make a move, type a command in the form: a1 to b2 \nPawn promotions are done as such: a1 to b2=Q \nCastling is done using: O-O or O-O-O \nFor a movelist, type: hint \nTo forfeit, type: f ";

	do {
		cout << "Do you want to go first? (y/n)\n>> ";
		getline(cin, choice);
	} while (choice.compare("y") != 0 && choice.compare("n") != 0);

	if (choice.compare("y") == 0) board.init(true);
	else board.init(false);
	
	board.countPieces();
	
	//While the other side hasn't lost yet
	while ((choice == "y" && board.pieceCount[B_KING] > 0) || (choice == "n" && board.pieceCount[W_KING] > 0)) {
		movelist.generateMoveList(board);

		board.displayBoard();

		cout << "For a list of commands, type: help \nEnter your command: ";
		getline(cin, x);

		//While user command is not valid
		while (!(isMoveValid(translateMoveCommand(x)) || x.compare("f") == 0) || x.compare("hint") == 0 || x.compare("help") == 0) {
			if (x.compare("help") != 0 && x.compare("hint") != 0){
				cout << "\n\nThat is not a valid move or command. For a list of commands, type: help \n";
			}

			board.displayBoard();

			if (x.compare("hint") == 0) movelist.printMoveList(board);
			else if (x.compare("f") == 0) break;
			else if (x.compare("help") == 0) cout << help << "\n";

			cout << "Enter your command: ";
			getline(cin, x);
		}

		if (x.compare("f") == 0) break; //exit game loop

		m.move = translateMoveCommand(x);
		board.makeMove(m);
		//board.sideToMove = board.sideToMove ^ 1;

		movelist.generateMoveList(board);
		int moveNum = getRandomInteger(0, movelist.numberOfMovesLegal - 1);
		board.makeMove(movelist.movesLegal[moveNum]);

		board.displayBoard();

		movelist.printMoveList(board);
		cout << "\n\nI, " << NAME << ", have decided to make move " << moveNum << ".";

		//board.sideToMove = board.sideToMove ^ 1;
		board.countPieces();
	}

	cout << "\n\nGame Over\nPress enter to exit: ";
	getline(cin, x);
}

//Program execution starts here
int main() {
	cout << "Hello. My name is " << NAME << ".\n";
	cout << "\nI have been created by Hunter and Minh for a CS class project";
	cout << "\n\nCurrently, I understand some rules of chess.";
	cout << "\nEnough to generate and make most legal moves.";
	cout << "\nHowever, I do not know at all what makes one move better than another.\n\n";

	string in; cout << "Choose one:\n \"1\" to play against me with a movelist\n \"2\" to have me play against myself\n \"3\" to do castling testing\n "
		<< "\"4\" to parse a FEN string\n \"5\" to play against me without a movelist\n>> ";
	getline(cin, in);

	if (in.compare("1") == 0) {
		testFunction4();
	}
	else if (in.compare("2") == 0) {
		testFunction3();
	}
	else if (in.compare("3") == 0){
		testFunction2();
	}
	else if (in.compare("4") == 0){
		testFunction1();
	}
	else if (in.compare("5") == 0){
		//testFunction5();
		testFunction22();
	}

	return 0;
}
