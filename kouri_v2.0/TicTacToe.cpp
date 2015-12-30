#include "GameState.h"
#include "TicTacToe.h"
#include <string>
#include <vector>
#include <cassert>

TicTacToe::TicTacToe()
	: board{' ',' ',' ',
					' ',' ',' ',
					' ',' ',' '} {}

TicTacToe::TicTacToe(char board_in[TTT_BOARD_SIZE]) {
	for (int i = 0; i < TTT_BOARD_SIZE; ++i) {
		board[i] = board_in[i];
	}
}

void TicTacToe::printBoard() const {
	using namespace std;
	string hrz_line = "-----";
	cout << hrz_line << "\n";
	for (int i = 0; i < TTT_BOARD_SIZE; ++i) {
		cout << board[i];
		if ((i + 1) % 3 == 0) {
			cout << "\n" << hrz_line << endl;
		} else cout << "|";
	}
}

std::vector<GameState*> TicTacToe::generateMoves() const {
	// TO DO
	std::vector<GameState*> test;
	return test;
}

bool TicTacToe::is_gameOver() const {
	return is_winner('X') || is_winner('O') || is_gameTied();
}

void TicTacToe::displayWinner() const {
	assert(is_gameOver()); // Makes sure that the game is over

	using namespace std;
	if (is_winner('X')) cout << "Game over. 'X' wins!" << endl;
	else if (is_winner('O')) cout << "Game over. 'O' wins!" << endl;
	else cout << "Game over. It's a tie!" << endl;
}

bool TicTacToe::is_winner(const char &c) const {
	assert(c == 'X' || c == 'O');

	return
		// Check rows
		(board[0] == board[1] && board[1] == board[2] && board[2] == c) ||
		(board[3] == board[4] && board[4] == board[5] && board[5] == c) ||
		(board[6] == board[7] && board[7] == board[8] && board[8] == c) ||
		// Check columns
		(board[0] == board[3] && board[3] == board[6] && board[6] == c) ||
		(board[1] == board[4] && board[4] == board[7] && board[7] == c) ||
		(board[2] == board[5] && board[5] == board[8] && board[8] == c) ||
		// Check diagonals
		(board[0] == board[4] && board[4] == board[8] && board[8] == c) ||
		(board[2] == board[4] && board[4] == board[6] && board[6] == c);
}

bool TicTacToe::is_boardFilled() const {
	for (int i = 0; i < TTT_BOARD_SIZE; ++i) {
		if (board[i] == ' ') return false;
	}
	return true;
}

bool TicTacToe::is_gameTied() const {
	return is_boardFilled() && !(is_winner('X') || is_winner('O'));
}

