#pragma once

#include "GameState.h"

class TicTacToe : public GameState {
private:
	static const int TTT_BOARD_SIZE = 9;

public:
	TicTacToe();
	TicTacToe(char board_in[TTT_BOARD_SIZE]);

	virtual void printBoard() const;
	virtual std::vector<GameState*> generateMoves() const;

	// EFFECTS: Returns true if the game
	virtual bool is_gameOver() const;

	// REQUIRES: The game must be over
	// MODIFIES: std::cout
	// EFFECTS: Prints out who won (or if it's a tie)
	virtual void displayWinner() const;

private: // Private variables
	// Representation invariant: all elements of board must at all times 
	//	be either a ' ', 'X', or 'O'
	char board[TTT_BOARD_SIZE];

private: // Private functions
	// REQUIRES: That c be one of the valid player chars ('X' or 'O')
	// EFFECTS: Returns true if c is the winner
	bool is_winner(const char &c) const;

	// EFFECTS: Returns true if the board is filled with X's and O's
	//	(ie. no blank spaces left)
	bool is_boardFilled() const;

	// EFFECTS: Returns true if the board is filled and no one won
	bool is_gameTied() const;
};

/*
For reference:
{ 0, 1, 2,
  3, 4, 5,
	6, 7, 8 }
*/