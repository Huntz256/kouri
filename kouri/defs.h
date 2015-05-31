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

//Define RANK_1 as 0, RANK_2 as 1, etc. Define FILE_1 as 0, etc.
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };

//Define WHITE as 0 and BLACK as 1
enum { WHITE, BLACK };

//Define MOVE(from,to,capture,promotion,castling
#define MOVE(from,to,capture,promotion,castling) ( (from) | ((to) << 7) | ((capture) << 14) | ((promotion) << 18) | ((castling) << 21))

//Define B_PAWN = 1, W_PAWN = 2, etc.
enum { EMPTY, B_PAWN, W_PAWN, B_BISHOP, W_BISHOP, B_KNIGHT, W_KNIGHT, 
B_ROOK, W_ROOK, B_QUEEN, W_QUEEN, B_KING, W_KING };

//Square ids for each square on a board
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

const int FILES[BOARD_SQUARE_COUNT] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, -1,
	-1, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, -1,
	-1, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, -1,
	-1, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, -1,
	-1, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, -1,
	-1, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, -1,
	-1, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, -1,
	-1, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

const int RANKS[BOARD_SQUARE_COUNT] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, -1,
	-1, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, -1,
	-1, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, -1,
	-1, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, -1,
	-1, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, -1,
	-1, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, -1,
	-1, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, -1,
	-1, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

//An array of these represent a move history 
class MoveHistory {
	public:
		int move; //Represents a move
		int castlePerms; //Describes what types of castling were allowed right before the move was made
};

//Move class represents a move
class Move {
	public:
		int move; //Stores all infomation regarding a single move
		int getFromSquare();
		int getToSquare();
		int getCapturedPiece();
		int getPromoted();
		int getCastling();
};

//Board structure class represents a board
class BoardStructure {
	public:
		//Represents previous moves that have been played on this board. Assumes that amount of half moves < 1028.
		MoveHistory history[1028]; 

		//Represents every piece in a position using the 10x12 board representation.
		//E.g. pieces[21] represents the square a1, pieces[98] represents the square h8.
		int pieces[BOARD_SQUARE_COUNT];

		//The current side to move on this board - 0 (White) or 1 (Black)
		int sideToMove; 

		//Describes what types of castling is allowed on this board
		//This is an integer from 0000 (no castling is allowed) to 1111 (all castling is allowed)
		int castlePerms; 

		//The square that en pass can be done on
		int enPassSquare; 

		//pawns[0] represents all white pawns on this board, pawns[1] is the same for black
		U64 pawns[2]; 

		//material[0] represents white material, material[1] represents black material
		//pawn = 1 point, knight = bishop = 3 points, rook = 5 points, queen = 9 points
		int material[2]; 

		void displayFullBoard(bool dispPieces = true); //Outputs full 10x12 board to console
		void displayBoard(); //Outputs 8x8 board to console
		void init(bool goFirst); //Sets up pieces for a standard chess match
		void resetBoardToEmpty(); //Resets the board
		int setUpBoardUsingFEN(char* fen); //Sets up pieces given a FEN string. Returns 0 if successful.
		void makeMove(Move move); //Assuming the move passed in is valid, modifies the board and stores the move in history[]
		int getPieceColor(int pieceNumber); //Retrieves the piece color of a piece
		void displayHistory(); //Displays all the moves so far as move integers
};

void testIsSquareAttacked(int side, BoardStructure board);
char numToPieceChar(int num);
void printSquare(int square); //Prints square in alg. notation

//Move list class reprsents a list of moves
class MoveList {
	public:
		Move moves[512];
		int numberOfMoves;
		void generateMoveList(BoardStructure board);
		void generatePawnMoves(BoardStructure board);
		void generateSliderMoves(BoardStructure board);
		void generateNonSliderMoves(BoardStructure board);

		void addPawnCapturingMove(BoardStructure board, int fromSquare, int toSquare, int capture, int side);
		void addPawnMove(BoardStructure board, int fromSquare, int toSquare, int side);
		void printMoveList();
};


#endif