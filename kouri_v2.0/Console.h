#pragma once

#include "Game.h"

// REQUIRES: choice be an int within the range of supported game types
// MODIFIES: nothing
// EFFECTS: Returns a Game pointer to the game type object specified
//	by choice. For example, if choice == 0, gameFactory() returns a
//	Game pointer to a new TicTacToe object.
Game * gameFactory(const int &choice);

// REQUIRES: nothing
// MODIFIES: std::cout
// EFFECTS: Runs loop asking for user input. Processes said input. If user
//	decides to play a game, calls gameFactory() and plays the game.
void startConsole();

// Helper functions used by startConsole()
// They print out various things, as suggested by the function names
void promptUserForGame();
void displayHelp();
void displayIntro();