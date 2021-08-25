#include "moves.h"

#include "board.h"
#include "misc.h"

#include <iostream>
using namespace std;

// Represents where a knight/rook/bishop/queeen/king can move
const int knight_movements[8] = { -19, -8, 12, 21, 19, 8, -12, -21 };
const int rook_movements[4] = { -10, 1, 10, -1 };
const int bishop_movements[4] = { -9, 11, 9, -11 };
const int king_movements[8] = { -10, -9, 1, 11, 10, 9, -1, -11 };

// Used in generate_slider_moves()
const int sliding_pieces[8] = { W_BISHOP, W_ROOK, W_QUEEN, 0, B_BISHOP, B_ROOK, B_QUEEN };
const int sliding_pieces_start_index[2] = { 0, 4 };

// Used in generate_non_slider_moves()
const int non_sliding_pieces[6] = { W_KNIGHT, W_KING, 0, B_KNIGHT, B_KING, 0 };
const int non_sliding_pieces_start_index[2] = { 0, 3 };

// For a piece i, number_of_directions[i] is the max number of directions it can move
const int number_of_directions[13] = { 0, 0, 0, 4, 4, 8, 8, 4, 4, 8, 8, 8, 8 };

// Used in generate_slider_moves() and generate_non_slider_moves()
const int piece_movements[13][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { -9, 11, 9, -11 },
    { -9, 11, 9, -11 },
    { -19, -8, 12, 21, 19, 8, -12, -21 },
    { -19, -8, 12, 21, 19, 8, -12, -21 },
    { -10, 1, 10, -1 },
    { -10, 1, 10, -1 },
    { -10, -9, 1, 11, 10, 9, -1, -11 },
    { -10, -9, 1, 11, 10, 9, -1, -11 },
    { -10, -9, 1, 11, 10, 9, -1, -11 },
    { -10, -9, 1, 11, 10, 9, -1, -11 }
};

int Move::get_from_square() noexcept
{
    return move_ & 0x7F;
}
int Move::get_to_square() noexcept
{
    return (move_ >> 7) & 0x7F;
}
int Move::get_captured_piece() noexcept
{
    return (move_ >> 14) & 0xF;
}
int Move::get_promoted() noexcept
{
    return (move_ >> 18) & 0xF;
}
int Move::get_castling() noexcept
{
    return (move_ >> 22) & 0x7;
}

void Move_List_Generator::add_pawn_capturing_move(int from_square, int to_square, int capture, int side)
{
    if (files[from_square] == -1) {
        throw "ERROR3";
    }
    if (files[to_square] == -1) {
        throw "ERROR4";
    }

    if (side == WHITE) {
        if (ranks[from_square] == RANK_7) {
            moves[moves_count] = Move(MOVE(from_square, to_square, capture, W_QUEEN, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, capture, W_ROOK, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, capture, W_KNIGHT, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, capture, W_BISHOP, 0));
            moves_count += 4;
        }
        else {
            moves[moves_count] = Move(MOVE(from_square, to_square, capture, 0, 0));
            moves_count++;
        }
    }
    else {
        if (ranks[from_square] == RANK_2) {
            moves[moves_count] = Move(MOVE(from_square, to_square, capture, B_QUEEN, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, capture, B_ROOK, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, capture, B_KNIGHT, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, capture, B_BISHOP, 0));
            moves_count += 4;
        }
        else {
            moves[moves_count] = Move(MOVE(from_square, to_square, capture, 0, 0));
            moves_count++;
        }
    }
}
void Move_List_Generator::add_pawn_move(int from_square, int to_square, int side) noexcept
{
    // White----
    if (side == WHITE) {
        // Promotions
        if (ranks[from_square] == RANK_7) {
            moves[moves_count] = Move(MOVE(from_square, to_square, 0, W_QUEEN, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, 0, W_ROOK, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, 0, W_KNIGHT, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, 0, W_BISHOP, 0));
            moves_count += 4;
        }
        else {
            moves[moves_count] = Move(MOVE(from_square, to_square, 0, 0, 0));
            moves_count++;
        }

    }

    // Black----
    else {
        // Promotions
        if (ranks[from_square] == RANK_2) {
            moves[moves_count] = Move(MOVE(from_square, to_square, 0, B_QUEEN, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, 0, B_ROOK, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, 0, B_KNIGHT, 0));
            moves[moves_count] = Move(MOVE(from_square, to_square, 0, B_BISHOP, 0));
            moves_count += 4;
        }
        else {
            moves[moves_count] = Move(MOVE(from_square, to_square, 0, 0, 0));
            moves_count++;
        }
    }
}
void Move_List_Generator::generate_pawn_moves(Board_Structure& board)
{
    // Go through every square on the board
    for (int i = 0; i < board_square_count; i++) {

        if (board.side_to_move == WHITE) {

            // If the current square is a white pawn...
            if (board.pieces[i] == 2) {

                // ... and the square in front of it is empty, add a move
                if (board.pieces[i + 10] == 0) {

                    // .. and the pawn is on starting rank
                    if (ranks[i] == RANK_2 && board.pieces[i + 20] == 0) {
                        add_pawn_move(i, i + 20, WHITE);
                    }

                    // .. regardless ...
                    add_pawn_move(i, i + 10, WHITE);
                }

                // Pawn capturing
                if (board.pieces[i + 9] > 0 && board.get_piece_color(board.pieces[i + 9]) == BLACK) {
                    add_pawn_capturing_move(i, i + 9, board.pieces[i + 9], WHITE);
                }

                if (board.pieces[i + 11] > 0 && board.get_piece_color(board.pieces[i + 11]) == BLACK) {
                    add_pawn_capturing_move(i, i + 11, board.pieces[i + 11], WHITE);
                }

                if (board.en_pass_square != 0) {
                    // En passant 
                    if (i + 9 == board.en_pass_square) {
                        add_pawn_capturing_move(i, i + 9, 0, WHITE);
                    }
                    if (i + 11 == board.en_pass_square) {
                        add_pawn_capturing_move(i, i + 11, 0, WHITE);
                    }
                }
            }
        }
        else {

            // If the current square is a black pawn...
            if (board.pieces[i] == 1) {

                // ... and the square in front of it is empty, add a move
                if (i >= 10 && board.pieces[i - 10] == 0) {

                    // .. and the pawn is on starting rank
                    if (ranks[i] == RANK_7 && board.pieces[i - 20] == 0) {
                        add_pawn_move(i, i - 20, BLACK);
                    }

                    // .. regardless...
                    add_pawn_move(i, i - 10, BLACK);
                }

                // Pawn capturing
                if (i >= 9 && board.pieces[i - 9] > 0 && board.get_piece_color(board.pieces[i - 9]) == WHITE) {
                    add_pawn_capturing_move(i, i - 9, board.pieces[i - 9], BLACK);
                }

                if (i >= 11 && board.pieces[i - 11] > 0 && board.get_piece_color(board.pieces[i - 11]) == WHITE) {
                    add_pawn_capturing_move(i, i - 11, board.pieces[i - 11], BLACK);
                }

                if (board.en_pass_square != 0) {
                    // En passant 
                    if (i - 9 == board.en_pass_square) {
                        add_pawn_capturing_move(i, i - 9, 0, BLACK);
                    }
                    if (i - 11 == board.en_pass_square) {
                        add_pawn_capturing_move(i, i - 11, 0, BLACK);
                    }
                }
            }
        }
    }
}

void Move_List_Generator::generate_slider_moves(Board_Structure& board) noexcept
{
    int piece_index = sliding_pieces_start_index[board.side_to_move];
    int piece = sliding_pieces[piece_index];
    piece_index++;

    while (piece != 0) {
        // Go through every square on the board
        for (int i = 0; i < board_square_count; i++) {

            // If there is a piece on square i that is a slider piece...
            if (board.pieces[i] == piece) {

                // For each direction...
                for (int j = 0; j < number_of_directions[piece]; j++) {
                    int temp_square = i + piece_movements[piece][j];

                    // While temp_square is not off board...
                    while (board.pieces[temp_square] != -1) {

                        // If temp_square is not empty, generate a capturing move
                        if (board.pieces[temp_square] != 0) {
                            // If temp_square has a piece opposite in color to the piece on square i
                            if (board.get_piece_color(board.pieces[temp_square]) == (board.side_to_move ^ 1)) {
                                moves[moves_count] = Move(MOVE(i, temp_square, board.pieces[temp_square], 0, 0));
                                moves_count++;
                            }
                            break;
                        }

                        // If temp_square is empty, generate a non capture move 
                        else if (board.pieces[temp_square] == 0) {
                            moves[moves_count] = Move(MOVE(i, temp_square, 0, 0, 0));
                            moves_count++;
                        }

                        temp_square += piece_movements[piece][j];
                    }
                }
            }
        }

        piece = sliding_pieces[piece_index];
        piece_index++;
    }
}

void Move_List_Generator::generate_non_slider_moves(Board_Structure& board) noexcept
{
    int piece_index = non_sliding_pieces_start_index[board.side_to_move];
    int piece = non_sliding_pieces[piece_index];
    piece_index++;

    while (piece != 0) {

        // Go through every square on the board
        for (int i = 0; i < board_square_count; i++) {

            // If there is a piece on square i that is a non slider piece...
            if (board.pieces[i] == piece) {

                if (piece == 0) {
                    break;
                }

                // For each direction...
                for (int j = 0; j < number_of_directions[piece]; j++) {
                    const int tempSquare = i + piece_movements[piece][j];

                    // If temp_square is off board, move on to the next direction
                    if (board.pieces[tempSquare] == -1) {
                        continue;
                    }

                    // If temp_square is not empty, generate a capturing move
                    if (board.pieces[tempSquare] != 0) {

                        // If temp_square has a piece opposite in color to the piece on square i
                        if (board.get_piece_color(board.pieces[tempSquare]) == (board.side_to_move ^ 1)) {
                            moves[moves_count] = Move(MOVE(i, tempSquare, board.pieces[tempSquare], 0, 0));
                            moves_count++;
                        }
                    }

                    // If temp_square is empty, generate a non capture move 
                    else if (board.pieces[tempSquare] == 0) {
                        moves[moves_count] = Move(MOVE(i, tempSquare, 0, 0, 0));
                        moves_count++;
                    }
                }
            }
        }

        piece = non_sliding_pieces[piece_index];
        piece_index++;
    }
}

void Move_List_Generator::generate_castling_moves(Board_Structure& board)
{
    if (board.side_to_move == WHITE) {
        // White Kingside Castling
        if ((board.castle_perms & 1) && (board.pieces[26] == 0 && board.pieces[27] == 0)) {
            if (!board.is_square_attacked(25, BLACK) && !board.is_square_attacked(26, BLACK) && !board.is_square_attacked(27, BLACK)) {
                moves[moves_count] = Move(MOVE(25, 27, 0, 0, 1));
                moves_count++;
            }
        }

        // White Queenside Castling
        if ((board.castle_perms & 2) && (board.pieces[22] == 0 && board.pieces[23] == 0 && board.pieces[24] == 0)) {
            if (!board.is_square_attacked(23, BLACK) && !board.is_square_attacked(24, BLACK) && !board.is_square_attacked(25, BLACK)) {
                moves[moves_count] = Move(MOVE(25, 23, 0, 0, 2));
                moves_count++;
            }
        }
    }
    else {
        // Black Kingside Castling
        if ((board.castle_perms & 4) && (board.pieces[96] == 0 && board.pieces[97] == 0)) {
            if (!board.is_square_attacked(95, WHITE) && !board.is_square_attacked(96, WHITE) && !board.is_square_attacked(97, WHITE)) {
                moves[moves_count] = Move(MOVE(95, 97, 0, 0, 3));
                moves_count++;
            }
        }
        
        // Black Queenside Castling
        if ((board.castle_perms & 8) && (board.pieces[92] == 0 && board.pieces[93] == 0 && board.pieces[94] == 0)) {
            if (!board.is_square_attacked(93, WHITE) && !board.is_square_attacked(94, WHITE) && !board.is_square_attacked(95, WHITE)) {
                moves[moves_count] = Move(MOVE(95, 93, 0, 0, 4));
                moves_count++;
            }
        }
    }
}

void Move_List_Generator::generate_move_list(Board_Structure& board)
{
    // Clear move arrays
    for (int i = 0; i <= moves_count; i++) {
        moves[i] = Move(0);
    }

    moves_count = 0;

    // Generate psuedo-legal moves
    generate_pawn_moves(board);
    generate_slider_moves(board);
    generate_non_slider_moves(board);
    generate_castling_moves(board);

    // Regenerate position key
    board.position_ID = board.generate_and_get_position_ID();
}

// Prints all of the moves in our move list in algebraic notation
// MUST be called immediately after generate_move_list() for piece num printing to be accuarate
void Move_List_Generator::print_move_list(const Board_Structure& board)
{
    const char piece_num_to_char[13] = { '\0', '\0', '\0', 'B', 'B', 'N', 'N', 'R', 'R', 'Q', 'Q', 'K', 'K' };

    for (int i = 0; i < moves_count; i++) {
        const int from_square = moves[i].get_from_square();
        const int to_square = moves[i].get_to_square();
        const int cap_piece = moves[i].get_captured_piece();
        const int prom_piece = moves[i].get_promoted();

        cout << "Move " << i << " Found: ";

        if ((moves[i].get_castling() == 1) || (moves[i].get_castling() == 3)) {
            cout << "O-O\n";
        }
        else if ((moves[i].get_castling() == 2) || (moves[i].get_castling() == 4)) {
            cout << "O-O-O\n";
        }
        else {
            cout << piece_num_to_char[board.pieces[moves[i].get_from_square()]];

            if (moves[i].get_captured_piece() != 0) {
                cout << files_to_char[files[moves[i].get_from_square()]] << "x";
            }

            print_square(to_square);

            cout << " (f:" << from_square << " t:" << to_square << " c:" << cap_piece << " p: " << prom_piece << ")\n";
        }
    }

    cout << "# of moves: " << moves_count << "\n\n";
}

// Prints a move in UCI format given a move
void Move_List_Generator::print_move_uci(Move m)
{
    const char piece_num_to_char[13] = { ' ', ' ', ' ', 'B', 'B', 'N', 'N', 'R', 'R', 'Q', 'Q', 'K', 'K' };

    const int from_square = m.get_from_square();
    const int to_square = m.get_to_square();

    print_square(from_square);
    print_square(to_square);
}

// Prints a move in algebraic notation given a move
void Move_List_Generator::print_move_algebraic(Move m, const Board_Structure& board)
{
    const char piece_num_to_char[13] = {'\0', '\0', '\0', 'B', 'B', 'N', 'N', 'R', 'R', 'Q', 'Q', 'K', 'K'};
    const int from_square = m.get_from_square();
    const int to_square = m.get_to_square();

    if ((m.get_castling() == 1) || (m.get_castling() == 3)) {
        cout << "O-O\n";
    }
    else if ((m.get_castling() == 2) || (m.get_castling() == 4)) {
        cout << "O-O-O\n";
    }
    else {
        cout << piece_num_to_char[board.pieces[m.get_from_square()]];

        if (m.get_captured_piece() != 0) {
            cout << files_to_char[files[m.get_from_square()]] << "x";
        }

        print_square(to_square);
    }
}

// Checks if a move integer is contained in the generated movelist and is valid
bool Move_List_Generator::is_move_valid(Board_Structure& board, int move)
{
    const int from_square = move & 0x7F;
    const int to_square = (move >> 7) & 0x7F;

    if (from_square < 21 || from_square > 98) {
        return false;
    }
    if (to_square < 21 || to_square > 98) {
        return false;
    }
    if (board.pieces[from_square] <= 0) {
        return false;
    }

    generate_move_list(board);

    for (int i = 0; i < moves_count; i++) {
        if (!board.make_move(moves[i])) {
            continue;
        }
        board.undo_move();
        if (moves[i].move() == move) {
            return true;
        }
    }
    return false;
}

int Move_List_Generator::get_legal_moves_count(Board_Structure& board)
{
    int legal_moves_count = 0;

    generate_move_list(board);

    for (int i = 0; i < moves_count; i++) {
        if (board.make_move(moves[i])) {
            legal_moves_count++;
            board.undo_move();
        }
    }

    return legal_moves_count;
}
