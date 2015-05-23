#include <iostream>
#include "defs.h"
using namespace std;

//Program execution starts here
int main()  {
	cout << "Hello. My name is " << NAME << ".\n";
	
	BOARD_STRUCTURE board;

	for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {
		board.pieces[i] = 0;
	}

	for (int i = 0; i < BOARD_SQUARE_COUNT; i++) {
		if (i % 10 == 0) {
			cout << '\n';
		}
		cout << board.pieces[i];
	}
	
	int x; cin >> x;

	return 0;
}