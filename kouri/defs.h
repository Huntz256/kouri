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

//Square ids for each square on a board
const int squareID64[BOARD_SQUARE_COUNT] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
	-1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
	-1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
	-1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
	-1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
	-1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
	-1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
	-1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

const int squareID120[64] = {
	21, 22, 23, 24, 25, 26, 27, 28,
	31, 32, 33, 34, 35, 36, 37, 38,
	41, 42, 43, 44, 45, 46, 47, 48,
	51, 52, 53, 54, 55, 56, 57, 58,
	61, 62, 63, 64, 65, 66, 67, 68,
	71, 72, 73, 74, 75, 76, 77, 78,
	81, 82, 83, 84, 85, 86, 87, 88,
	91, 92, 93, 94, 95, 96, 97, 98
};

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
	void resetBoardToEmpty(); //Resets the board
} board;

#endif