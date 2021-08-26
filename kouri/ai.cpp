#include "ai.h"

#include "board.h"
#include "misc.h"
#include "moves.h"
#include "pvtable.h"

#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
#include <windows.h>
#include <ctime>

using namespace std;

// Piece square tables
// These are used in evaluation to assign values to specific pieces on specific locations

// The d2 and e2 pawns should advance before the other pawns
const int pawn_table[64] = {
    0,   0,  0,  0,    0,  0,  0,  0,
    10, 10, 10, -10, -10, 10, 10, 10,
    5,   0,  0,  5,    5,  0,  0,  5,
    0,   0,  0,  25,  25,  0,  0,  0,
    5,   5,  5,  10,  10,  5,  5,  5,
    10, 10, 10,  20,  20, 10, 10, 10,
    20, 20, 20,  30,  30, 20, 20, 20,
    0,   0,  0,  0,    0,  0,  0,  0
};

// Knights should go toward the center
const int knight_table[64] = {
    0, -25, 0,  -5,  -5,  0, -25, 0,
    0,  0,  0,  5,  5,  0,   0, 0,
    -5,  0, 10, 10, 10, 10,   0, -5,
    0,  0, 10, 20, 20, 10,   5, 0,
    5, 10, 15, 20, 20, 15,  10, 5,
    5, 10, 10, 20, 20, 10,  10, 5,
    0,  0,  5, 10, 10,  5,   0, 0,
    0,  0,  0,  0,  0,  0,   0, 0
};

// Bishops should also go toward the center
const int bishop_table[64] = {
    0, 0, -10, 0,   0, -10,  0, 0,
    0, 0,   0, 10, 10,   0,  0, 0,
    0, 0,  10, 15, 15,  10,  0, 0,
    0, 10, 15, 20, 20,  15, 10, 0,
    0, 10, 15, 20, 20,  15, 10, 0,
    0, 0,  10, 15, 15,  10,  0, 0,
    0, 0,  0,  10, 10,   0,  0, 0,
    0, 0,  0,  0,   0,   0,  0, 0
};

// Rooks should be in the center files or the
// 7th rank if possible
const int rook_table[64] = {
    0,   0,  5, 10, 10,  5,  0,  0,
    0,   0,  5, 10, 10,  5,  0,  0,
    0,   0,  5, 10, 10,  5,  0,  0,
    0,   0,  5, 10, 10,  5,  0,  0,
    0,   0,  5, 10, 10,  5,  0,  0,
    0,   0,  5, 10, 10,  5,  0,  0,
    25, 25, 25, 25, 25, 25, 25, 25,
    0,   0,  5, 10, 10,  5, 0,  0
};

const int mirror_64[64] = {
    56, 57, 58, 59, 60, 61, 62, 63,
    48, 49, 50, 51, 52, 53, 54, 55,
    40, 41, 42, 43, 44, 45, 46, 47,
    32, 33, 34, 35, 36, 37, 38, 39,
    24, 25, 26, 27, 28, 29, 30, 31,
    16, 17, 18, 19, 20, 21, 22, 23,
     8,  9, 10, 11, 12, 13, 14, 15,
     0,  1,  2,  3,  4,  5,  6,  7
};

// Inits AI. This is called in find_best_move()
void AI::init(Board_Structure& board)
{
    // Set default best move
    Move_List_Generator gen1;
    gen1.generate_move_list(board);
    best_move_ = gen1.moves[0];

    // Note that histPly stores # of half-moves for the entire game,
    // while ply stores # of half-moves for the current search
    board.ply = 0;

    table.clear_PV_table();
    node_count_ = 0;
    eval_count_ = 0;
}

// Returns the best move found by the negamax function
Move AI::find_best_move(Board_Structure& board, int depth)
{
    int pv_moves_count = 0;

    // Clock to retrieve system time
    clock_t timer = clock();

    // Init the AI
    init(board);

    // Search for the best move using negamax
    best_move_score_ = negamax(-infinity, infinity, board, depth);

    // Print the best move
    if (!uci_mode) {
        cout << "\nbest_move_ is:";
        move_list.print_move_uci(best_move_);
    }

    // Fill pv_array and get number of moves in pv
    pv_moves_count = table.get_PV_line(board, depth);

    // Find time difference between previously retrieved system time and current time
    timer = clock() - timer;

    if (!uci_mode) {
        cout << "\n\nI, " << name << ", have decided to make move ";
        move_list.print_move_uci(best_move_);
        cout << " after searching to depth " << ai.max_depth << ".\n";
        cout << "\nNumber of nodes scanned: " << node_count_ << ". Number of evaluations made: " << eval_count_ << ".\n";
        cout << "Total calculation time: " << ((float)timer) / CLOCKS_PER_SEC << " seconds. \n"; //Convert clock_t object to time in seconds and print out

        // Print the pv (principal variation)
        cout << "\nPrincipal variation is:\n";
        for (int i = 0; i < pv_moves_count; i++) {
            move_list.print_move_uci(Move(pv_array[i]));
            cout << "\n";
        }
    }

    return best_move_;
}

// Negamax with alpha-beta pruning
// Searches recursively for the best move in the board position given by 
// the parameter board
int AI::negamax(int alpha, int beta, Board_Structure& board, int depth)
{
    node_count_++;

    // If depth == 0 or ply too high, go back up the tree
    if (depth == 0 || board.ply > 63) {
        eval_count_++;
        return evaluate(board);
    }

    // If threefold repetition, assume the game is a draw
    if (board.is_threefold_repetition()) {
        return 0;
    }

    // Generate all moves for this position
    Move_List_Generator gen1;
    gen1.generate_move_list(board);

    Move pv_best_move(0);
    int legal_moves_count = 0, score = -infinity;
    const int old_alpha = alpha;

    // Go through all of the generated moves
    for (int i = 0; i < gen1.moves_count; i++) {

        // Make move i. If move i is invalid, go to the next move in the move list
        if (!board.make_move(gen1.moves[i])) {
            if (!uci_mode && depth == max_depth) {
                cout << "\ni:" << i << " move ";
                gen1.print_move_uci(gen1.moves[i]); cout << " is invalid.";
            }
            continue;
        }

        // If move i is valid (legal), continue and increment legal counter
        legal_moves_count++;

        // Print what kouri is thinking about
        if (!uci_mode && depth == max_depth) {
            cout << "\ni:" << i << " thinking about valid move ";
            gen1.print_move_uci(gen1.moves[i]); cout << "...";
        }

        // Call negamax() to get the move's score
        score = -negamax(-beta, -alpha, board, depth - 1);

        if (!uci_mode && depth == max_depth) {
            cout << " score: " << score;
        }

        // Undo the move we just made
        board.undo_move();

        if (score > alpha) {
            if (score >= beta) {
                return beta;
            }
            alpha = score;

            pv_best_move = gen1.moves[i];
            if (depth == max_depth) {
                best_move_ = gen1.moves[i];
                best_move_score_ = score;
                if (!uci_mode) {
                    cout << "\nSetting best_move_ to ";
                    gen1.print_move_uci(gen1.moves[i]);
                }
            }
        }

    }

    // What if there are no legal moves found? We must be in checkmate or stalemate!
    if (legal_moves_count == 0) {

        // If our king is attacked, it is checkmate!
        if (board.is_square_attacked(board.king_square[board.side_to_move], board.side_to_move ^ 1)) {

            // Adding board.ply means that a mate in 3 is better than a mate in 6
            return -mate + board.ply;
        }

        // Otherwise, it is stalemate and the game is a draw
        else {
            return 0;
        }

    }

    // Store the best move in pv table
    if (alpha != old_alpha) {
        table.store_PV_move(board, pv_best_move.move());
    }

    return alpha;
}

// Returns an an evaluation of the board
//  - A positive score represents a board in favor of the size to move, while a negative score means that
//  the other side has an advantage
//  - Material: See piece_value[]. A pawn is worth 100 points, a bishop 300 points, etc
int AI::evaluate(Board_Structure& board)
{
    // Evaluate material
    board.count_pieces();
    const int material_score = piece_value[12] * (board.piece_count[W_KING] - board.piece_count[B_KING])
        + piece_value[10] * (board.piece_count[W_QUEEN] - board.piece_count[B_QUEEN])
        + piece_value[8] * (board.piece_count[W_ROOK] - board.piece_count[B_ROOK])
        + piece_value[6] * (board.piece_count[W_BISHOP] - board.piece_count[B_BISHOP] + board.piece_count[W_KNIGHT] - board.piece_count[B_KNIGHT])
        + piece_value[2] * (board.piece_count[W_PAWN] - board.piece_count[B_PAWN]);

    // Evaluate position
    int positional_score = 0;
    for (int i = 21; i < 98; i++) {
        if (ranks[i] != -1) {
            if (board.pieces[i] == W_PAWN) {
                positional_score += pawn_table[square_ID_64[i]];
            }
            else if (board.pieces[i] == B_PAWN) {
                positional_score -= pawn_table[mirror_64[square_ID_64[i]]];
            }
            else if (board.pieces[i] == W_KNIGHT) {
                positional_score += knight_table[square_ID_64[i]];
            }
            else if (board.pieces[i] == B_KNIGHT) {
                positional_score -= knight_table[mirror_64[square_ID_64[i]]];
            }
            else if (board.pieces[i] == W_BISHOP) {
                positional_score += bishop_table[square_ID_64[i]];
            }
            else if (board.pieces[i] == B_BISHOP) {
                positional_score -= bishop_table[mirror_64[square_ID_64[i]]];
            }
            else if (board.pieces[i] == W_ROOK) {
                positional_score += rook_table[square_ID_64[i]];
            }
            else if (board.pieces[i] == B_ROOK) {
                positional_score -= rook_table[mirror_64[square_ID_64[i]]];
            }
        }
    }

    // Evaluate mobility
    int mobility = 0;
    move_list.generate_move_list(board);
    mobility += move_list.moves_count;
    board.side_to_move ^= 1;
    move_list.generate_move_list(board);
    mobility -= move_list.moves_count;
    board.side_to_move ^= 1;

    if (board.side_to_move == WHITE) {
        return (material_score + positional_score + mobility);
    }
    else {
        return -(material_score + positional_score + mobility);
    }
}

vector<int> AI::get_eval_breakdown(Board_Structure& board)
{
    vector<int> evaluation;

    // Evaluate material
    board.count_pieces();
    const int material_score = piece_value[12] * (board.piece_count[W_KING] - board.piece_count[B_KING])
        + piece_value[10] * (board.piece_count[W_QUEEN] - board.piece_count[B_QUEEN])
        + piece_value[8] * (board.piece_count[W_ROOK] - board.piece_count[B_ROOK])
        + piece_value[6] * (board.piece_count[W_BISHOP] - board.piece_count[B_BISHOP] + board.piece_count[W_KNIGHT] - board.piece_count[B_KNIGHT])
        + piece_value[2] * (board.piece_count[W_PAWN] - board.piece_count[B_PAWN]);

    // Evaluate position
    int positional_score = 0;
    for (int i = 21; i < 98; i++) {
        if (ranks[i] != -1) {
            if (board.pieces[i] == W_PAWN) {
                positional_score += pawn_table[square_ID_64[i]];
            }
            else if (board.pieces[i] == B_PAWN) {
                positional_score -= pawn_table[mirror_64[square_ID_64[i]]];
            }
            else if (board.pieces[i] == W_KNIGHT) {
                positional_score += knight_table[square_ID_64[i]];
            }
            else if (board.pieces[i] == B_KNIGHT) {
                positional_score -= knight_table[mirror_64[square_ID_64[i]]];
            }
            else if (board.pieces[i] == W_BISHOP) {
                positional_score += bishop_table[square_ID_64[i]];
            }
            else if (board.pieces[i] == B_BISHOP) {
                positional_score -= bishop_table[mirror_64[square_ID_64[i]]];
            }
            else if (board.pieces[i] == W_ROOK) {
                positional_score += rook_table[square_ID_64[i]];
            }
            else if (board.pieces[i] == B_ROOK) {
                positional_score -= rook_table[mirror_64[square_ID_64[i]]];
            }
        }
    }

    // Evaluate mobility
    int mobility = 0;
    move_list.generate_move_list(board);
    mobility += move_list.moves_count;
    board.side_to_move ^= 1;
    move_list.generate_move_list(board);
    mobility -= move_list.moves_count;
    board.side_to_move ^= 1;

    if (board.side_to_move == WHITE) {
        evaluation.push_back(material_score);
        evaluation.push_back(positional_score);
        evaluation.push_back(mobility);
    }
    else {
        evaluation.push_back(-material_score);
        evaluation.push_back(-positional_score);
        evaluation.push_back(-mobility);
    }

    return evaluation;
}
