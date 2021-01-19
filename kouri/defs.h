//Definitions Header File

//Header guard. When this file is included, check if not previously defined.
//If so, use the following defintions. Otherwise, ignore this file.
#ifndef DEFS_H
#define DEFS_H

#include <fstream>
#include <vector>
#include <string>

//Define NAME as "kouri"
#define NAME "kouri"

//True if we want to use kouri with a UCI GUI
extern bool uci_mode;

//Define the number of squares, BOARD_SQUARE_COUNT, on our board representation as 120
#define BOARD_SQUARE_COUNT 120

//Define a new type called U64 as an unsigned long long integer
typedef unsigned long long U64;

//Define RANK_1 as 0, RANK_2 as 1, etc. Define FILE_1 as 0, etc.
enum Rank { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
enum File { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };

//Define WHITE as 0 and BLACK as 1
enum Color { WHITE, BLACK };

//Define MOVE(from,to,capture,promotion,castling)
#define MOVE(from,to,capture,promotion,castling) ( (from) | ((to) << 7) | ((capture) << 14) | ((promotion) << 18) | ((castling) << 22))

//Define B_PAWN = 1, W_PAWN = 2, B_BISHOP = 3, W_BISHOP = 4, B_KNIGHT = 5, W_KNIGHT = 6, B_ROOK = 7, W_ROOK = 8, B_QUEEN = 9, W_QUEEN = 10, B_KING = 11, W_KING = 12
enum Piece {
    EMPTY, B_PAWN, W_PAWN, B_BISHOP, W_BISHOP, B_KNIGHT, W_KNIGHT,
    B_ROOK, W_ROOK, B_QUEEN, W_QUEEN, B_KING, W_KING
};

//Piece material values, in centipawns
const int piece_value[13] = { 0, 100, 100, 300, 300, 300, 300, 500, 500, 900, 900, 220000, 220000 };

//Square ids for each square on a board
const int square_ID_120[64] = {
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
};

const int square_ID_64[BOARD_SQUARE_COUNT] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, -1,
    -1, 8, 9, 10, 11, 12, 13, 14, 15, -1,
    -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
    -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
    -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
    -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
    -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
    -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

//A bunch of random numbers, one for each piece-square combination.
//Initialized in init_keys(). Used for generating a position id.
extern U64 piece_square_key[13][BOARD_SQUARE_COUNT];

//A random number representing "white to move." Initialized in init_keys().
extern U64 side_key;

//A bunch of random numbers, one for each castling perm combination. Initialized in init_keys().
extern U64 castle_perm_key[16];

//Define infinity
///#define INFIN numeric_limits<int>::max()
#define INFIN 999999

//Define MATE as a large number less than infinity
#define MATE 200000

const int files[BOARD_SQUARE_COUNT] = {
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

const int ranks[BOARD_SQUARE_COUNT] = {
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

const char files_to_char[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

//An array of these represent a move history 
class Move_History {
public:
    int move; //Represents a move
    int castle_perms; //Describes what types of castling were allowed right before the move was made
    int en_pass_square;//The square that en pass can be done on right before the move was made
    U64 position_ID; //Used for threefold repeition detection. The position (how the pieces are arranged) right before the move was made. 
};

//Move class represents a move
class Move {
public:
    int move; //Stores all infomation regarding a single move
    int get_from_square() noexcept;
    int get_to_square() noexcept;
    int get_captured_piece() noexcept;
    int get_promoted() noexcept;
    int get_castling() noexcept;
};

//Board structure class represents a board
class Board_Structure {
public:
    //Represents previous moves that have been played on this board. Assumes that amount of half moves < 2048.
    Move_History history[2048];

    //Represents every piece in a position using the 10x12 board representation.
    //E.g. pieces[21] represents the square a1, pieces[98] represents the square h8.
    int pieces[BOARD_SQUARE_COUNT];

    //The current side to move on this board - 0 (White) or 1 (Black)
    int side_to_move;

    //Describes what types of castling is allowed on this board
    //This is an integer from 0000 (no castling is allowed) to 1111 (all castling is allowed)
    //0001 = white kingside castling allowed
    //0010 = white queenside castling allowed
    //0100 = black kingside castling allowed
    //1000 = black queenside castling allowed
    //Example: 1010 means queenside castling is allowed for both sides.
    //Note that this integer is DIFFERENT from that used in Move
    int castle_perms;

    //The square that en pass can be done on
    int en_pass_square;

    //pawns[0] represents all white pawns on this board, pawns[1] is the same for black
    U64 pawns[2];

    //material[0] represents white material, material[1] represents black material
    //pawn = 1 point, knight = bishop = 3 points, rook = 5 points, queen = 9 points
    int material[2];

    //The square that the king is on; king_square[0] is for white, king_square[1] is for black
    int king_square[2];

    //The half-turn we are on
    int ply, history_ply;

    //Each position (how the pieces are arranged on a board) has a unique identifier.
    //This is like an fen string, but it is easier to generate. 
    //It is used to detect repetition for the threefold repetition rule.
    U64 position_ID;

    //Contains the number of pieces. Piece index corresponds with previously defined enums
    //Ex: piece_count[1] returns the number of black pawns since B_PAWN has been previously defined as 1
    int piece_count[13];

    void display_full_board(bool dispPieces = true); //Outputs full 10x12 board to console
    void display_board(); //Outputs 8x8 board to console
    void init(bool goFirst); //Sets up pieces for a standard chess match
    void reset_board_to_empty() noexcept; //Resets the board
    int setup_board_using_FEN(std::string fen); //Sets up the board given a FEN string. Returns 0 if successful.
    void remove_piece_from_square(int square) noexcept;
    void add_piece_to_square(int square, int piece) noexcept;
    void move_piece_to_square(int from_square, int to_square) noexcept;
    bool make_move(Move move); //Modifies the board and stores the move in history[]. Return true if successful.
    void undo_move(); //Undoes the last move
    int get_piece_color(int piece_number) noexcept; //Retrieves the color of a piece
    void display_history(); //Displays all the moves so far as move integers
    bool is_square_attacked(int square, int attacking_side); //Returns true if square square is being attacked by a piece from the side attacking_side
    void count_pieces() noexcept; //Counts all the pieces on the board and records them in the piece_count[] array
    bool is_repetition() noexcept; //Has this position occured before in the game? If yes, return true.  Used for checking threefold repetition.
    U64 generate_and_get_position_ID() noexcept; //Generate and return a position id representing this board's position. Used for checking threefold repetition.
    bool is_board_valid(); //Looks at some aspects of the board and returns false if there is something wrong with the current board
};

//Principal variation entity
class PV_Entity {
public:
    int move;
    U64 position_ID;
};

//Principal variation table: contains many pv entities
class PV_Table {
public:
    PV_Entity pv_table[2048];
    int entries_count;

    void init_PV_table() noexcept;
    void clear_PV_table() noexcept;
    void store_PV_move(Board_Structure& board, int move) noexcept;
    int get_PV_move(Board_Structure& board) noexcept;
    int get_PV_line(Board_Structure& board, int depth);

};

extern PV_Table table;


void test_is_square_attacked(int side, Board_Structure& board); //Used only for testing is_square_attacked()
char num_to_piece_char(int num); //Converts integer to char representing a piece
void print_square(int square); //Prints square in alg. notation
int char_to_piece_int(char c) noexcept; //Converts char like 'p' or 'Q' to the corresponding integer

//Contains all move generation functions
class Move_List_Generator {
public:
    Move moves[2048];
    int moves_count;
    void generate_move_list(Board_Structure& board);
    void generate_pawn_moves(Board_Structure& board);
    void generate_slider_moves(Board_Structure& board) noexcept;
    void generate_non_slider_moves(Board_Structure& board) noexcept;
    void generate_castling_moves(Board_Structure& board);

    void add_pawn_capturing_move(int from_square, int to_square, int capture, int side);
    void add_pawn_move(int from_square, int to_square, int side) noexcept;
    void print_move_list(const Board_Structure& board);
    void uci_print_move_given_move_list_number(int move_num);
    void uci_print_move_given_move(Move m);
    void uci_print_move_given_move_int(int move);

    bool is_move_valid(Board_Structure& board, int move);
    int get_legal_moves_count(Board_Structure& board);
};

extern Move_List_Generator move_list;

//A principal variation array
extern int pv_array[64];

//AI Stuff
class AI {
public:
    void init(Board_Structure& board) noexcept; // Init AI by clearing the PV table and other variables
    int evaluate(Board_Structure& board); // Does a basic evaluation of a board from the perspective of the size to move
    std::vector<int> get_eval_breakdown(Board_Structure& board); // Gets the evaluation of the board but as a vector
    int negamax(int alpha, int beta, Board_Structure& board, int depth); // Negamax algorithm with alpha-beta pruning
    Move find_best_move(Board_Structure& board, int depth); // Uses negamax to find best move
    int max_depth = 6;
    int get_node_count() noexcept { return node_count; };
    int get_best_move_score() noexcept { return best_move_score; };
private:
    int node_count; //Number of nodes scanned for a search
    int eval_count; //Number of evaluations done for a search
    Move best_move;
    int best_move_score;
};

extern AI ai;

#endif