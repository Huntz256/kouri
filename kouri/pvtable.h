#pragma once

#include "board.h"
#include "misc.h"

// Principal variation entity
class PV_Entity {
public:
    int move;
    U64 position_ID;
};

// Principal variation table: contains many pv entities
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

// A principal variation array
extern int pv_array[64];

extern PV_Table table;