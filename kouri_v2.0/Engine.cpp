// Implementation of the console
// Runs the loop asking for user input
// Processes input and hands off control to a particular Game
//   if the user decides to play one

#include "Engine.h"
#include "TicTacToe.h"

#include <string>
#include <iostream>
#include <cassert>

GameState * gameFactory(const int &choice) {
	switch (choice) {
	case 0: return new TicTacToe;
	//case 1: return new Chess;
	//case 2: return new Checkers;
	default: return 0;
	}
}

void startEngine() {
	using namespace std;

	cout << "Starting engine... (vroom vroom)\n" << endl;

	string command;
	GameState *gs = 0;

	displayIntro();
	while (true) {
		// Prompt user for input
		promptUserForGame();
		
		// Get input
		getline(cin, command);
		cout << "--------------------------------\n" << endl;
		system("cls"); // Clears console. Windows-only.

		// Process input
		if (command == "q") break; // Quit
		//else if (command == "0" || command == "1" || command == "2") {
		else if (command == "0") {
			// User has decided to play a game
			gs = gameFactory(std::stoi(command));
			//assert(game); // Stops the program if game is NULL for some reason
			//game->play();

			// TEMP CODE
			if (gs) gs->printBoard();
			cout << 
				"Recognized command to play game.\n" << 
				"Unfortunately, that functionality is unavailable right now. " <<
				"Please come back later.\n" <<
				endl;
			// END TEMP CODE
		}
		else if (command == "h") {
			displayHelp();
			cout << endl;
		}
		else if (command == "i") {
			displayIntro();
			cout << endl;
		}
		else cout << "Unrecognized command. Please try again.\n" << std::endl;
	}

	cout << "Thanks for playing. Terminating engine... (waah waah)" << endl;

	// Delete the game pointer if it's not NULL
	if (gs) delete gs;
}

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

void displayHelp() {
	std::cout << 
		"Here is a list of commands you can type:\n" <<
		"h\tBrings up this help menu.\n" <<
		"q\tQuits the game.\n" <<
		"i\tDisplays information about this program and its authors\n" <<
		"\n" <<
		"To use a command, simply type it and press Enter." <<
		std::endl;
}

void displayIntro() {
	std::cout <<
		"Hi there. This is kouri v2.0.\n" <<
		"The creators of this program are named Hunter and Minh.\n" <<
		"The first is a computer engineering student and the \n" <<
		"second is a computer science student." <<
		std::endl;
}