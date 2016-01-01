#include "GameState.h"
#include "TicTacToe.h"
#include <cassert>

//////// PUBLIC METHODS //////////

TicTacToe::TicTacToe()
	: board{' ',' ',' ',
					' ',' ',' ',
					' ',' ',' '},
		xMovesNext(true) {}

TicTacToe::TicTacToe(char board_in[TTT_BOARD_SIZE]) 
	: xMovesNext(true) {
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

std::vector<std::string> TicTacToe::generateMoves() const {
	// TO DO
	std::vector<std::string> test;
	return test;
}

bool TicTacToe::is_gameOver() const {
	return is_winner('X') || is_winner('O') || is_gameTied();
}

void TicTacToe::printWinner() const {
	assert(is_gameOver()); // Makes sure that the game is over

	using namespace std;
	if (is_winner('X')) cout << "Game over. 'X' wins!" << endl;
	else if (is_winner('O')) cout << "Game over. 'O' wins!" << endl;
	else cout << "Game over. It's a tie!" << endl;
}

void TicTacToe::requestUserInput() const {
	std::cout <<
		"It's your turn. The board is numbered as follows:\n" <<
		"0|1|2\n" <<
		"-----\n" <<
		"3|4|5\n" <<
		"-----\n" <<
		"6|7|8\n" <<
		"Enter a number from 0 through 8 to indicate where you wish to\n" <<
		"place your piece:" <<
		std::endl;
}

bool TicTacToe::is_validMoveCommand(const std::string &com) const {
	return 
		// Check that com is a number from 0 through 8
		(com == "0" || com == "1" || com == "2" ||
		com == "3" || com == "4" || com == "5" ||
		com == "6" || com == "7" || com == "8") &&
		// Check that the spot on the board is empty
		board[std::stoi(com)] == ' ';
}

void TicTacToe::applyMoveCommand(const std::string &com) {
	// Check to make sure the com is valid (just in case)
	assert(is_validMoveCommand(com));

	if (xMovesNext) board[std::stoi(com)] = 'X';
	else board[std::stoi(com)] = 'O';

	xMovesNext = !xMovesNext; // Flip the bool
}

/////// PRIVATE METHODS /////////

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

int TicTacToe::evaluate() const {
	// TO DO
	return 0;
}