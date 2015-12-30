// Implementation of the console
// Runs the loop asking for user input
// Processes input and hands off control to a particular Game
//   if the user decides to play one

#include "Engine.h"
#include "TicTacToe.h"

#include <string>
#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdlib>

GameState * gameFactory(const int &choice) {
	switch (choice) {
	case 0: return new TicTacToe;
	//case 1: return new Chess;
	//case 2: return new Checkers;
	default: return 0;
	}
}

void runEngine() {
	using namespace std;

	cout << "Starting engine... (vroom vroom)\n" << endl;

	string command;
	GameState *gs = 0;

	printIntro();

	while (true) {
		// Prompt user for input
		promptUserForGame();
		
		// Get input
		getline(cin, command);
		system("cls"); // Clears console. Windows-only.

		// Process input
		if (command == "q") break; // Quit
		//else if (command == "0" || command == "1" || command == "2") {
		else if (command == "0") {
			// User has decided to play a game
			if (command == "0") {
				cout << "You have decided to play Tic-Tac-Toe!\n" << endl;
			}
			gs = gameFactory(std::stoi(command));
			assert(gs); // Stops the program if game is NULL for some reason
			gameLoop(*gs);
		}
		else if (command == "h") {
			printHelp();
			cout << endl;
		}
		else if (command == "i") {
			printIntro();
			cout << endl;
		}
		else cout << "Unrecognized command. Please try again.\n" << std::endl;
	}

	cout << "Thanks for playing. Terminating engine... (waah waah)" << endl;

	// Delete the game pointer if it's not NULL
	if (gs) delete gs;
}

void gameLoop(GameState &gs) {
	using namespace std;

	string command;

	while (true) {

		do {
			gs.printBoard();
			cout << endl;

			gs.requestUserInput();
			getline(cin, command);
			system("cls");
		} while (!gs.is_validMoveCommand(command));
		
		gs.applyMoveCommand(command);
		gs.printBoard();
		cout << endl;

		// Check if game is over after player made move
		if (gs.is_gameOver()) {
			gs.printWinner();
			cout << endl;
			break;
		}
		else {
			// minimax will be called here somewhere (once implemented)

			srand((int)time(NULL));
			int randMove;
			do {
				// Generate rand int from 0 through 8
				randMove = rand() % 9;
			} while (!gs.is_validMoveCommand(to_string(randMove)));

			gs.applyMoveCommand(to_string(randMove));
			system("cls");
			cout << "Computer chose to make the move: " << randMove << 
				"\n" << endl;

			// Check to see if game is over after computer made move
			if (gs.is_gameOver()) {
				gs.printBoard();
				gs.printWinner();
				cout << endl;
				break;
			}
		}
	}
}

/////////// HELPER FUNCTIONS ////////////

void promptUserForGame() {
	std::cout << 
		"Which game would you like to play?\n" <<
		"0: TicTacToe\n" <<
		"1: Chess (Unavailable)\n" <<
		"2: Checkers (Unavailable)\n" <<
		"\n" <<
		"Type \"h\" for help, \"q\" to quit." << 
		std::endl;
}

void printHelp() {
	std::cout << 
		"Here is a list of commands you can type:\n" <<
		"h\tBrings up this help menu.\n" <<
		"q\tQuits the game.\n" <<
		"i\tDisplays information about this program and its authors\n" <<
		"\n" <<
		"To use a command, simply type it and press Enter." <<
		std::endl;
}

void printIntro() {
	std::cout <<
		"Hi there. This is kouri v2.0.\n" <<
		"The creators of this program are named Hunter and Minh.\n" <<
		"The first is a computer engineering student and the \n" <<
		"second is a computer science student." <<
		std::endl;
}