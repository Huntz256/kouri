#pragma once

#include <vector>
#include <iostream>

// Declaration of generic Game interface
// This class and its children must implement the specified virtual
//	member functions
// The GameState object (and its successors) should be kept lightweight
// as vectors of GameStates will be passed to the AI
class GameState {
public:
	// MODIFIES: std::cout
	// EFFECTS: prints out the current Game board in a consistent, visually
	//	appealing manner
	virtual void printBoard() = 0;

	// EFFECTS: Returns a homogenous vector of GameState pointers, each of 
	//	which can be dereferenced to obtain a valid GameState after a single
	//	move has been made
	// The responsibility for deleting said pointers lies outside of GameState
	virtual std::vector<GameState*> generateMoves() = 0;

	// EFFECTS: Returns true if *this (Game) has ended (ie. one side has won
	//	or no moves can be made according to the rules of this game)
	virtual bool is_gameOver() = 0;

	// EFFECTS: 
	virtual void displayWinner() = 0;
};