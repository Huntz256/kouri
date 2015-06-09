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

//Define MOVE(from,to,capture,promotion,castling)
#define MOVE(from,to,capture,promotion,castling) ( (from) | ((to) << 7) | ((capture) << 14) | ((promotion) << 18) | ((castling) << 22))

//Define B_PAWN = 1, W_PAWN = 2, B_BISHOP = 3, W_BISHOP = 4, B_KNIGHT = 5, W_KNIGHT = 6, B_ROOK = 7, W_ROOK = 8, B_QUEEN = 9, W_QUEEN = 10, B_KING = 11, W_KING = 12
enum { EMPTY, B_PAWN, W_PAWN, B_BISHOP, W_BISHOP, B_KNIGHT, W_KNIGHT, 
B_ROOK, W_ROOK, B_QUEEN, W_QUEEN, B_KING, W_KING };

//Piece material values, in centipawns
const int PIECE_VALUE[13] = { 0, 100, 100, 300, 300, 300, 300, 500, 500, 900, 900, 1000000, 1000000 };

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

//A bunch of random numbers, one for each piece-square combination.
//Initialized in initKeys(). Used for generating a position id.
extern U64 pieceSquareKey[13][BOARD_SQUARE_COUNT];

//A random number representing "white to move." Initialized in initKeys().
extern U64 sideKey;

//A bunch of random numbers, one for each castling perm combination. Initialized in initKeys().
extern U64 castlePermKey[16];


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

const char FILES_TO_CHAR[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

//An array of these represent a move history 
class MoveHistory {
	public:
		int move; //Represents a move
		int castlePerms; //Describes what types of castling were allowed right before the move was made
		int enPassSquare;//The square that en pass can be done on right before the move was made
		U64 positionID; //Used for threefold repeition detection. The position (how the pieces are arranged) right before the move was made. 
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
		//Represents previous moves that have been played on this board. Assumes that amount of half moves < 2048.
		MoveHistory history[2048]; 

		//Represents every piece in a position using the 10x12 board representation.
		//E.g. pieces[21] represents the square a1, pieces[98] represents the square h8.
		int pieces[BOARD_SQUARE_COUNT];

		//The current side to move on this board - 0 (White) or 1 (Black)
		int sideToMove; 

		//Describes what types of castling is allowed on this board
		//This is an integer from 0000 (no castling is allowed) to 1111 (all castling is allowed)
		//0001 = white kingside castling allowed
		//0010 = white queenside castling allowed
		//0100 = black kingside castling allowed
		//1000 = black queenside castling allowed
		//Example: 1010 means queenside castling is allowed for both sides.
		//Note that this integer is DIFFERENT from that used in Move
		int castlePerms; 

		//The square that en pass can be done on
		int enPassSquare; 

		//pawns[0] represents all white pawns on this board, pawns[1] is the same for black
		U64 pawns[2]; 

		//material[0] represents white material, material[1] represents black material
		//pawn = 1 point, knight = bishop = 3 points, rook = 5 points, queen = 9 points
		int material[2]; 

		//The square that the king is on; kingSquare[0] is for white, kingSquare[1] is for black
		int kingSquare[2];

		//The half-turn we are on
		int ply, historyPly;

		//Each position (how the pieces are arranged on a board) has a unique identifier.
		//This is like an fen string, but it is easier to generate. 
		//It is used to detect repetition for the threefold repetition rule.
		U64 positionID;

		//Contains the number of pieces. Piece index corresponds with previously defined enums
		//Ex: pieceCount[1] returns the number of black pawns since B_PAWN has been previously defined as 1
		int pieceCount[13];

		//A principal variation array
		int pvArray[64];

		void displayFullBoard(bool dispPieces = true); //Outputs full 10x12 board to console
		void displayBoard(); //Outputs 8x8 board to console
		void init(bool goFirst); //Sets up pieces for a standard chess match
		void resetBoardToEmpty(); //Resets the board
		int setUpBoardUsingFEN(char* fen); //Sets up the board given a FEN string. Returns 0 if successful.
		void removePieceFromSquare(int square);
		void addPieceToSquare(int square, int piece);
		void movePieceToSquare(int fromSquare, int toSquare);
		bool makeMove(Move move); //Modifies the board and stores the move in history[]. Return true if successful.
		void undoMove(); //Undoes the last move
		int getPieceColor(int pieceNumber); //Retrieves the color of a piece
		void displayHistory(); //Displays all the moves so far as move integers
		bool isSquareAttacked(int square, int attackingSide); //Returns true if square square is being attacked by a piece from the side attackingSide
		void countPieces(); //Counts all the pieces on the board and records them in the pieceCount[] array
		bool isRepetition(); //Has this position occured before in the game? If yes, return true.  Used for checking threefold repetition.
		U64 generateAndGetPositionID(); //Generate and return a position id representing this board's position. Used for checking threefold repetition.
		bool isBoardValid(); //Looks at some aspects of the board and returns false if there is something wrong with the current board
};

//Principal variation entity
class PVEntity {
public:
	int move;
	U64 positionID;
};

//Principal variation table: contains many pv entities
class PVTable {
public:
	//Note that this is a pointer! We could use an array instead.
	//Read up on pointers if you are confused.
	PVEntity *pvTable;
	int numOfEntries;

	void initPVTable();
	void clearPVTable();
	void storePVMove(BoardStructure board, int move);
	int getPVMove(BoardStructure board);
	int getPVLine(BoardStructure board, int depth);

};

extern PVTable table;


void testIsSquareAttacked(int side, BoardStructure board); //Used only for testing isSquareAttacked()
char numToPieceChar(int num); //Converts integer to char representing a piece
void printSquare(int square); //Prints square in alg. notation
int charToPieceInt(char c); //Converts char like 'p' or 'Q' to the corresponding integer

//Contains all move generation functions
class MoveListGenerator {
	public:
		Move moves[2048];
		int numberOfMoves;
		void generateMoveList(BoardStructure board);
		void generatePawnMoves(BoardStructure board);
		void generateSliderMoves(BoardStructure board);
		void generateNonSliderMoves(BoardStructure board);
		void generateCastlingMoves(BoardStructure board);

		void addPawnCapturingMove(BoardStructure board, int fromSquare, int toSquare, int capture, int side);
		void addPawnMove(BoardStructure board, int fromSquare, int toSquare, int side);
		void printMoveList(BoardStructure board);
		void uciPrintMoveGivenMoveListNumber(BoardStructure board, int moveNum);
		void uciPrintMoveGivenMove(BoardStructure board, Move m);

		bool isMoveValid(BoardStructure board, int move);
};

extern MoveListGenerator movelist;

// AI Stuff------------------------
int evaluate(BoardStructure &board); //Does a basic evaluation of a board based on piece counts from WHITE's perspective
int negaMax(BoardStructure &board, int depth, int a, int b); //nega-max algorithm with alpha-beta pruning
Move findBestMove(BoardStructure board, int depth); //Uses nega-max to find best move
BoardStructure applyMove(BoardStructure board, Move m);



#endif