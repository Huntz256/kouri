#pragma once

#include "GameState.h"

class TicTacToe : public GameState {
public:
	TicTacToe();

	virtual void printBoard();

	virtual std::vector<GameState*> generateMoves();

	virtual bool is_gameOver();

	virtual void displayWinner();

private:
	static const int TTT_BOARD_SIZE = 9;
	// Representation invariant: all elements of board must at all times 
	//	be either a ' ', 'X', or 'O'
	char board[TTT_BOARD_SIZE];
};