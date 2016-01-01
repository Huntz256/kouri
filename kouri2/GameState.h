#pragma once

#include <vector>
#include <iostream>
#include <string>

// Declaration of generic Game interface
// This class and its children must implement the specified virtual
//	member functions
// The GameState object (and its successors) should be kept lightweight
// as vectors of GameStates will be passed to the AI
class GameState {
public:

	// MODIFIES: std::cout
	// EFFECTS: Prints out the current Game board in a consistent, visually
	//	appealing manner
	virtual void printBoard() const = 0;

	// EFFECTS: Returns a vector of strings, each of which represents 
	//	a valid Move command that can be applied to the GameState
	//	using applyMoveCommand()
	virtual std::vector<std::string> generateMoves() const = 0;

	// EFFECTS: Returns true if *this (Game) has ended (ie. one side has won
	//	or no moves can be made according to the rules of this game)
	virtual bool is_gameOver() const = 0;

	// REQUIRES: That the game is over
	// MODIFIES: std::cout
	// EFFECTS: Prints out the winner
	virtual void printWinner() const = 0;

	// MODIFIES: std::cout
	// EFFECTS: Prints out a message telling to user to input a command
	//	that will be translated into a Move using processMoveCommand(),
	//	preferrably with friendly instructions
	//	
	//	Something along the lines of: "Your turn. Enter a number from 0
	//	through 8:"
	virtual void requestUserInput() const = 0;

	// EFFECTS: Returns true if com is a valid Move command (ie. if com 
	//	can be succesfully parsed into a valid Move by processMoveCommand()
	virtual bool is_validMoveCommand(const std::string &com) const = 0;

	// REQUIRES: com be a valid Move command
	// MODIFIES: *this (GameState)
	// EFFECTS: Processed com, applies the corresponding Move to the
	//	current game, and updates GameState accordingly
	virtual void applyMoveCommand(const std::string &com) = 0;

	// EFFECTS: Returns an int representing a numerical estimate
	//	of the value of the board from the POV of the computer
	//
	//	ie. Returns a higher value if the board is advantageous for
	//	the computer (consult for internet for sample evaluation
	//	functions for various strategy games such as chess)
	virtual int evaluate() const = 0;
};
