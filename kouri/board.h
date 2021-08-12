#pragma once

#include "misc.h"
#include <string>

class Move;

//An array of these represent a move history 
class Move_History {
public:
    int move; //Represents a move
    int castle_perms; //Describes what types of castling were allowed right before the move was made
    int en_pass_square;//The square that en pass can be done on right before the move was made
    U64 position_ID; //Used for threefold repeition detection. The position (how the pieces are arranged) right before the move was made. 
};

//Board structure class represents a board
class Board_Structure {
public:
    //Represents previous moves that have been played on this board. Assumes that amount of half moves < 2048.
    Move_History history[2048];

    //Represents every piece in a position using the 10x12 board representation.
    //E.g. pieces[21] represents the square a1, pieces[98] represents the square h8.
    int pieces[board_square_count];

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

void print_square(int square);
int char_to_piece_int(char c, int side_to_move) noexcept;
