//Definitions Header File

//Header guard. When this file is included, check if not previously defined.
//If so, use the following defintions. Otherwise, ignore this file.
#ifndef DEFS_H
#define DEFS_H

//Define NAME as "kouri"
#define NAME "kouri"

//Define the number of squares, BOARD_SQUARE_COUNT, on our board representation as 120
#define BOARD_SQUARE_COUNT 120

//Define a new type called U64 as an unsigned long long integer
typedef unsigned long long U64;

//An array of these represent a move history 
class MoveHistory {
	int move; //Represents a move
	int castlePerms; //Describes what types of castling were allowed right before the move was made
};

//Board structure class represents a board
class BoardStructure {

	//Represents every piece in a position using the 10x12 board representation.
	//E.g. pieces[21] represents the square a1, pieces[98] represents the square h8.
	int pieces[BOARD_SQUARE_COUNT]; 

	U64 pawns[2]; //pawns[0] represents all white pawns on this board, pawns[1] is the same for black

	int sideToMove; //The current side to move on this board

	int castlePerms; //Describes what types of castling is allowed on this board

	MoveHistory history[1028]; //Represents previous moves that have been played on this board. Assumes that amount of half moves < 1028.

public:
	void displayFullBoard(bool dispPieces); //Outputs full 10x10 board to console
	void displayBoard(bool dispPieces); //Outputs 8x8 board to console
	void init(bool goFirst); //Sets up pieces for a standard chess match

} board;

#endif