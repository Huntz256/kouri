// Implementation of the console
// Runs the loop asking for user input
// Processes input and hands off control to a particular Game
//   if the user decides to play one

#include "Console.h"
#include "TicTacToe.h"

#include <string>
#include <iostream>
#include <cassert>

Game * gameFactory(const int &choice) {
	switch (choice) {
	case 0: return new TicTacToe;
	//case 1: return new Chess;
	//case 2: return new Checkers;
	default: return 0;
	}
}

void startConsole() {
	std::cout << "Starting console..." << std::endl;

	std::string command;
	Game *game = 0;

	displayIntro();
	while (true) {
		// Prompt user for input
		promptUserForGame();
		// Get input
		getline(std::cin, command);
		// Process input
		if (command == "q") break; // Quit
		else if (command == "0" || command == "1" || command == "2") {
			// User has decided to play a game
			game = gameFactory(std::stoi(command));
			//assert(game); // Stops the program if game is NULL for some reason
			//game->play();

			// TEMP CODE
			std::cout << "Recognized command to play game.\n" << 
				"Unfortunately, that functionality is unavailable right now. " <<
				"Please come back later." <<
				std::endl;
			break;
		}
		else if (command == "h") displayHelp();
		else if (command == "i") displayIntro();
		else std::cout << "Unrecognized command. Please try again." << std::endl;
	}

	std::cout << "Thanks for playing. Terminating console..." << std::endl;

	// Delete the game pointer if it's not NULL
	if (game) delete game;
}

void promptUserForGame() {
	std::cout << 
		"Which game would you like to play?\n" <<
		"0: TicTacToe\n" <<
		"1: Chess\n" <<
		"2: Checkers" <<
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
		"The creators of this wonderful program are named Hunter and Minh. " <<
		"They are [REDACTED]\n" <<
		"ERROR: Detected attempt to access classified information. Aborting." <<
		std::endl;
}