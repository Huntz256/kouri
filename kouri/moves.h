#pragma once

#include "board.h"

// Move class represents a move
class Move {
public:
    Move() noexcept { move_ = 0; };
    Move(int move) noexcept : move_{move} {};
    int get_from_square() noexcept;
    int get_to_square() noexcept;
    int get_captured_piece() noexcept;
    int get_promoted() noexcept;
    int get_castling() noexcept;
    int move() noexcept { return move_; }
private:
    int move_; // Stores all information regarding a single move
};

// Contains all move generation functions
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
    void uci_print_move_given_move(Move m);
    void uci_print_move_given_move_int(int move);

    bool is_move_valid(Board_Structure& board, int move);
    int get_legal_moves_count(Board_Structure& board);
};

extern Move_List_Generator move_list;