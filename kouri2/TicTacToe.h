#pragma once

#include "GameState.h"

class TicTacToe : public GameState {
private:
	static const int TTT_BOARD_SIZE = 9;

public:
	TicTacToe();
	TicTacToe(char board_in[TTT_BOARD_SIZE]);

	// Overridden inherited virtual functions from GameState
	// See GameState interface for RME's
	virtual void printBoard() const;
	virtual std::vector<GameState*> generateMoves() const;
	virtual bool is_gameOver() const;
	virtual void printWinner() const;
	virtual void requestUserInput() const;
	virtual bool is_validMoveCommand(const std::string &com) const;
	virtual void applyMoveCommand(const std::string &com);

private: // VARIABLES

	// Representation invariant: all elements of board must at all times 
	//	be either a ' ', 'X', or 'O'
	char board[TTT_BOARD_SIZE];
	// For reference, board is represented like this:
	//	{ 0, 1, 2,
	//		3, 4, 5,
	//		6, 7, 8 }

	// Is true if it's player 'X's turn, false if it's 'O's turn
	// For now, 'X' will always start first
	bool xMovesNext;

private: // METHODS

	// REQUIRES: That c be one of the valid player chars ('X' or 'O')
	// EFFECTS: Returns true if c is the winner
	bool is_winner(const char &c) const;

	// EFFECTS: Returns true if the board is filled with X's and O's
	//	(ie. no blank spaces left)
	bool is_boardFilled() const;

	// EFFECTS: Returns true if the board is filled and no one won
	bool is_gameTied() const;
};

