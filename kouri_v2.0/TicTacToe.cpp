#include "GameState.h"
#include "TicTacToe.h"
#include <string>
#include <vector>

TicTacToe::TicTacToe()
	: board{' ',' ',' ',
					' ',' ',' ',
					' ',' ',' '} {}

void TicTacToe::printBoard() {
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

std::vector<GameState*> TicTacToe::generateMoves() {
	// TO DO
	std::vector<GameState*> test;
	return test;
}

bool TicTacToe::is_gameOver() {
	// TO DO
	return true;
}

void TicTacToe::displayWinner() {
	// TO DO
}