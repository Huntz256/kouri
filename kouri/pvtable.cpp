// Principal variation table.
// "The Principal variation (PV) is a sequence of moves that programs consider best 
// and therefore expect to be played."
//https://chessprogramming.wikispaces.com/Principal+variation

#include "pvtable.h"

#include "board.h"
#include "moves.h"

#include <iostream>
using namespace std;

void PV_Table::init_PV_table() noexcept
{
    // The number of entries in the table
    entries_count = 2048;

    // Set everything in pv_table to zero
    clear_PV_table();
}

// Set all variables of PVEntities in pv_table to 0 values
void PV_Table::clear_PV_table() noexcept
{
    for (int i = 0; i < entries_count; i++) {
        pv_table[i].position_ID = 0ULL;
        pv_table[i].move = 0;
    }
}

// Store a move and position ID in the PV table
void PV_Table::store_PV_move(Board_Structure& board, int move) noexcept
{
    int i = board.generate_and_get_position_ID() % entries_count;
    pv_table[i].move = move;
    pv_table[i].position_ID = board.generate_and_get_position_ID();
}

// Get the principal variation move
int PV_Table::get_PV_move(Board_Structure& board) noexcept
{
    const int i = board.generate_and_get_position_ID() % entries_count;

    if (pv_table[i].position_ID == board.generate_and_get_position_ID()) {
        return pv_table[i].move;
    }

    return 0;
}


int PV_Table::get_PV_line(Board_Structure& board, int d)
{
    Move move(get_PV_move(board));
    int count = 0;

    while (move.move() != 0 && count < d) {
        // If the move returned by get_PV_move exists and is valid, make the move, add it to pv_array, and increment count
        if (move_list.is_move_valid(board, move.move())) {
            board.make_move(move);
            pv_array[count++] = move.move();
        }

        // If illegal move, stop 
        else {
            break;
        }

        // Get another move from get_PV_move
        move = Move(get_PV_move(board));
    }

    // Return to org. position
    while (board.ply > 0) {
        board.undo_move();
    }

    return count;
}
