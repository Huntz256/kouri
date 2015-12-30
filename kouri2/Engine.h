#pragma once

#include "GameState.h"

// REQUIRES: choice be an int within the range of supported game types
// MODIFIES: nothing
// EFFECTS: Returns a Game pointer to the game type object specified
//	by choice. For example, if choice == 0, gameFactory() returns a
//	Game pointer to a new TicTacToe object.
GameState * gameFactory(const int &choice);

// REQUIRES: nothing
// MODIFIES: std::cout
// EFFECTS: Runs loop asking for user input. Processes said input. If user
//	decides to play a game, calls gameFactory() and hands off control
//	to gameLoop()
void runEngine();

// REQUIRES: gs be a valid GameState pointer
// MODIFIES: *gs
// EFFECTS: Runs a loop to carry out the turns of whatever game
//	the user chose earlier
void gameLoop(GameState &gs);

//////// HELPER FUNCTIONS /////////

// Used in runEngine()
// They print out various things, as suggested by the function names
void promptUserForGame();
void printHelp();
void printIntro();