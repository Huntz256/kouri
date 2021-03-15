#pragma once

#include "misc.h"
#include "moves.h"
#include <vector>

class Board_Structure;

constexpr int infinity = 999999;

// A large number less than infinity
constexpr int mate = 200000;

constexpr int square_ID_64[board_square_count] = {
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

class AI {
public:
    void init(Board_Structure& board) noexcept; // Init AI by clearing the PV table and other variables
    int evaluate(Board_Structure& board); // Does a basic evaluation of a board from the perspective of the size to move
    std::vector<int> get_eval_breakdown(Board_Structure& board); // Gets the evaluation of the board but as a vector
    int negamax(int alpha, int beta, Board_Structure& board, int depth); // Negamax algorithm with alpha-beta pruning
    Move find_best_move(Board_Structure& board, int depth); // Uses negamax to find best move
    int max_depth = 4;
    int node_count() noexcept { return node_count_; };
    int best_move_score() noexcept { return best_move_score_; };
private:
    int node_count_; //Number of nodes scanned for a search
    int eval_count_; //Number of evaluations done for a search
    Move best_move_;
    int best_move_score_; //For UCI info command
};

extern AI ai;