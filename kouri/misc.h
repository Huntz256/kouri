#pragma once

#include <string>

const std::string name = "kouri";

//True if we want to use kouri with a UCI GUI
extern bool uci_mode;

//Define the number of squares, board_square_count, on our board representation as 120
constexpr int board_square_count = 120;

typedef unsigned long long U64;

//Define RANK_1 as 0, RANK_2 as 1, etc. Define FILE_1 as 0, etc.
enum Rank { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
enum File { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };

//Define WHITE as 0 and BLACK as 1
enum Color { WHITE, BLACK };

//Define MOVE(from,to,capture,promotion,castling)
constexpr int MOVE(int from, int to, int capture, int promotion, int castling) noexcept { return from | (to << 7) | (capture << 14) | (promotion << 18) | (castling << 22); }

//Define B_PAWN = 1, W_PAWN = 2, B_BISHOP = 3, W_BISHOP = 4, B_KNIGHT = 5, W_KNIGHT = 6, B_ROOK = 7, W_ROOK = 8, B_QUEEN = 9, W_QUEEN = 10, B_KING = 11, W_KING = 12
enum Piece {
    EMPTY, B_PAWN, W_PAWN, B_BISHOP, W_BISHOP, B_KNIGHT, W_KNIGHT,
    B_ROOK, W_ROOK, B_QUEEN, W_QUEEN, B_KING, W_KING
};

//Piece material values, in centipawns
constexpr int piece_value[13] = { 0, 100, 100, 300, 300, 300, 300, 500, 500, 900, 900, 220000, 220000 };

//Square ids for each square on a board
constexpr int square_ID_120[64] = {
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
//Initialized in init_keys(). Used for generating a position id.
extern U64 piece_square_key[13][board_square_count];

//A random number representing "white to move." Initialized in init_keys().
extern U64 side_key;

//A bunch of random numbers, one for each castling perm combination. Initialized in init_keys().
extern U64 castle_perm_key[16];

constexpr int files[board_square_count] = {
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

constexpr int ranks[board_square_count] = {
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

constexpr char files_to_char[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };