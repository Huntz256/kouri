#include "board.h"

#include "ai.h"
#include "misc.h"
#include "moves.h"
#include "pvtable.h"

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <windows.h>
using namespace std;

const int castle_permissions[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 7, 15, 15, 15, 3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

//Represents where a knight/rook/bishop/queeen/king can move
const int knight_movements[8] = { -19, -8, 12, 21, 19, 8, -12, -21 };
const int rook_movements[4] = { -10, 1, 10, -1 };
const int bishop_movements[4] = { -9, 11, 9, -11 };
const int king_movements[8] = { -10, -9, 1, 11, 10, 9, -1, -11 };

//Returns a character representing a piece, given a piece number.
char num_to_piece_char(int num)
{
    unordered_map<int, char> piece_map;

    if (num >= 0) {
        piece_map[0] = ' ';
        piece_map[B_PAWN] = 'p';  piece_map[W_PAWN] = 'P';
        piece_map[B_BISHOP] = 'b';  piece_map[W_BISHOP] = 'B';
        piece_map[B_KNIGHT] = 'n';  piece_map[W_KNIGHT] = 'N';
        piece_map[B_ROOK] = 'r';  piece_map[W_ROOK] = 'R';
        piece_map[B_QUEEN] = 'q';  piece_map[W_QUEEN] = 'Q';
        piece_map[B_KING] = 'k'; piece_map[W_KING] = 'K';

        return piece_map[num];
    }

    return '~';
}

//Converts char like 'p' or 'Q' to the corresponding integer
//Used for pawn promotion user command
int char_to_piece_int(char c, int side_to_move) noexcept
{
    if (side_to_move == WHITE) {
        switch (c) {
        case 'p': case 'P':
            return W_PAWN;
        case 'b': case 'B':
            return W_BISHOP;
        case 'n': case 'N':
            return W_KNIGHT;
        case 'r': case 'R':
            return W_ROOK;
        case 'q': case 'Q':
            return W_QUEEN;
        case 'k': case 'K':
            return W_KING;
        default:
            return -1;
        }
    }
    else {
        switch (c) {
        case 'p': case 'P':
            return B_PAWN;
        case 'b': case 'B':
            return B_BISHOP;
        case 'n': case 'N':
            return B_KNIGHT;
        case 'r': case 'R':
            return B_ROOK;
        case 'q': case 'Q':
            return B_QUEEN;
        case 'k': case 'K':
            return B_KING;
        default:
            return -1;
        }
    }
}

//Outputs full 10x12 board to console
void Board_Structure::display_full_board(bool disp_pieces)
{
    for (int i = 0; i < board_square_count; i++) {
        if (i % 10 == 0) {
            cout << "\n------------------------------\n";
        }
        if (!disp_pieces && pieces[i] < 10 && pieces[i] >= 0) {
            cout << 0;
        }
        if (disp_pieces) {
            cout << " " << num_to_piece_char(pieces[i]) << "|";
        }
        else {
            cout << pieces[i] << "|";
        }
    }
    cout << "\n------------------------------\n";

    //Also display some more infomation
    cout << "Side to move: " << (side_to_move == 0 ? "White" : "Black");
    cout << "\nCastling permissions: " << castle_perms << '\n';
}

//Outputs 8x8 board to console
void Board_Structure::display_board()
{
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);

    //Display the board with white on bottom
    cout << "\n";
    for (int rank = RANK_8; rank >= RANK_1; rank--) {
        cout << rank + 1 << " ";
        for (int file = FILE_A; file <= FILE_H; file++) {
            const int piece_num = pieces[square_ID_120[rank * 8 + file]];
            if (piece_num % 2 == 1) {
                SetConsoleTextAttribute(h_console, 12); // Set console text color to red
            }
            else {
                SetConsoleTextAttribute(h_console, 14); // Set console text color to yellow
            }
            cout << " " << num_to_piece_char(piece_num);
            SetConsoleTextAttribute(h_console, 7); // Set console text color to white
            cout << "|";
        }
        cout << "\n  ------------------------\n";
    }
    cout << "   a  b  c  d  e  f  g  h\n\n";

    //Also display some more infomation
    cout << "Side to move: " << (side_to_move == 0 ? "White" : "Black");
    //cout << "\nCastling permissions: " << castle_perms << '\n';
    ///cout << "Enpass. square: " << en_pass_square << '\n';

    // Display estimated probability of winning
    const float p = -((float)ai.evaluate(*this)) / 100;
    cout << "\nKouri's evaluation: I think I have a " <<
        round((0.5 + 0.5 * p / ((double)abs(p) + 2 / ((double)abs(p) + 1))) * 100) <<
        "% chance of winning.";

    for (const auto i : ai.get_eval_breakdown(*this))
        cout << ' ' << i;
    cout << "\n";

}

//Sets up the board for a standard chess match
void Board_Structure::init(bool go_first)
{
    if (go_first) {
        setup_board_using_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
    else {
        setup_board_using_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    }
}

//Resets the board
void Board_Structure::reset_board_to_empty() noexcept
{
    //Set all squares on the board to the value -1 representing off board squares
    for (int i = 0; i < board_square_count; i++) {
        pieces[i] = -1;
    }

    //Set only the middle 8 x 8 squares to the value 0 representing empty squares
    for (int i = 0; i < 64; i++) {
        pieces[square_ID_120[i]] = 0;
    }

    //Clear the pawns bitboard and reset material value to zero
    for (int i = 0; i < 2; i++) {
        pawns[i] = 0ULL;
        material[i] = 0;
    }

    //No side can castle on an empty board
    castle_perms = 0;

    //Reset en pass square
    en_pass_square = 0;

    //Reset king's square
    king_square[0] = 0; king_square[1] = 0;

    //Reset ply and history_ply
    ply = 0; history_ply = 0;

    //Reset side to move 
    side_to_move = WHITE;

    //Reset position ID by setting all bits to zero
    position_ID = 0ULL;

    //Init pv table
    table.init_PV_table();
}

//Sets up pieces given a FEN string. Returns 0 if successful.
int Board_Structure::setup_board_using_FEN(string fen)
{
    reset_board_to_empty();

    int current_rank = RANK_8, current_file = FILE_A, empty_num = 0, piece = 0;
    size_t i = 0;

    //Set up board pieces using the infomation in fen
    while ((current_rank >= RANK_1) && (i < fen.size())) {
        empty_num = 1;

        switch (fen[i]) {
        case 'p': piece = B_PAWN; break; case 'P': piece = W_PAWN; break;
        case 'b': piece = B_BISHOP; break; case 'B': piece = W_BISHOP; break;
        case 'n': piece = B_KNIGHT; break; case 'N': piece = W_KNIGHT; break;
        case 'r': piece = B_ROOK; break; case 'R': piece = W_ROOK; break;
        case 'q': piece = B_QUEEN; break; case 'Q': piece = W_QUEEN; break;
        case 'k': piece = B_KING; break; case 'K': piece = W_KING; break;

        case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
            piece = 0; empty_num = fen[i] - '0'; break;

        case '/': case ' ':
            current_rank--; current_file = FILE_A; i++; continue;
        default:
            return -1;
        }

        for (int j = 0; j < empty_num; j++) {
            if (piece != 0) {
                pieces[square_ID_120[current_rank * 8 + current_file]] = piece;
            }
            current_file++;
        }

        i++;
    }

    //Set the side to move using infomation in FEN
    side_to_move = (fen[i] == 'w') ? 0 : 1;

    //Set castling permissions using infomation in FEN
    fen += 2;
    for (int j = 0; j < 4; j++) {
        if (fen[i] == ' ') {
            break;
        }

        //for a FEN such as ...blah blah w KQkq blah blah...
        switch (fen[i]) {
        case 'K': castle_perms |= 1;  break; //... if there is a K, do castle_perms OR 0001; e.g. 0000 | 0001 = 0001
        case 'Q': castle_perms |= 2;  break; //... if there is a Q, do castle_perms OR 0010; e.g. 0001 | 0010 = 0011
        case 'k': castle_perms |= 4;  break; //... if there is a k, do castle_perms OR 0100; e.g. 0011 | 0100 = 0111
        case 'q': castle_perms |= 8; break; //... if there is a q, do castle_perms OR 1000; e.g. 0111 | 1000 = 1111
        default: break;
        }
        i++;
    }

    //Generate position id
    position_ID = generate_and_get_position_ID();

    //Update material[] and king_square[]
    for (int j = 0; j < board_square_count; j++) {
        if (pieces[j] > 0) {
            int color = get_piece_color(pieces[j]);
            material[color] += piece_value[pieces[j]];
            piece_count[pieces[j]]++;

            if (pieces[j] == W_KING) {
                king_square[WHITE] = j;
            }
            else if (pieces[j] == B_KING) {
                king_square[BLACK] = j;
            }

        }
    }

    return 0;
}

//Retrieves the color of a piece
int Board_Structure::get_piece_color(int piece_num) noexcept
{
    switch (piece_num) {
    case B_PAWN: case B_BISHOP: case B_KNIGHT: case B_ROOK: case B_QUEEN: case B_KING:
        return BLACK; break;
    case W_PAWN: case W_BISHOP: case W_KNIGHT: case W_ROOK: case W_QUEEN: case W_KING:
        return WHITE; break;
    default:
        ///cout << "ERROR: INVALID PIECE COLOR. piece_num: " << piece_num << "\n";
        return -42; break;
    }
}

//Displays all the moves so far as move integers
void Board_Structure::display_history()
{
    for (int i = 0; i < 1028; i++) {
        if (history[i].move == 0) break;
        cout << history[i].move << " ";
    }
    cout << "\n";
}

//Prints a square in algebraic notation. For example, print_square(55) prints "e4"
void print_square(int square)
{
    cout << files_to_char[files[square]] << (1 + ranks[square]);
}

//Returns true if square square is being attacked by a piece from the side attacking_side
bool Board_Structure::is_square_attacked(int square, int attacking_side)
{
    //cout << "is_square_attacked(): attacking_side = " << attacking_side << "\n";
    ///cout << "is_square_attacked(): bloop\n";
    //Make sure arguments of this function are valid first
    if (pieces[square] == -1) {
        cout << "EXCEPTION: INVALID SQUARE ARGUMENT";
    }

    //Check if square square is being attacked by attacking_side by any pawns...
    if (attacking_side == WHITE) {

        //If square square is being attacked by a white pawn, return true
        if (pieces[square - 11] == 2 || pieces[square - 9] == 2) {
            ///cout << "is_square_attacked(): yes. square " << square << " is being attacked by a pawn\n"; 
            return true;
        }

    }
    else {
        //If square square is being attacked by a black pawn, return true
        if (pieces[square + 11] == 1 || pieces[square + 9] == 1) {
            ///cout << "is_square_attacked(): yes. square " << square << " is being attacked by a pawn\n"; 
            return true;
        }
    }

    //Check if square square is being attacked by attacking_side by any knights...
    for (int i = 0; i < 8; i++) {

        //If square square is being attacked by a white knight, return true
        if (attacking_side == WHITE) {
            if (pieces[square + knight_movements[i]] == 6) {
                ///cout << "is_square_attacked(): yes. square " << square << " is being attacked by a white knight\n"; 
                return true;
            }
        }
        else {
            //If square square is being attacked by a black knight, return true
            if (pieces[square + knight_movements[i]] == 5) {
                ///cout << "is_square_attacked(): yes. square " << square << " is being attacked by a black knight\n"; 
                return true;
            }
        }
    }

    //Check if square square is being attacked by attacking_side by any rooks or queens...
    int temp_square = 0;
    for (int i = 0; i < 4; i++) {
        temp_square = square + rook_movements[i];
        while (pieces[temp_square] != -1) {
            if (pieces[temp_square] != 0) {
                if (((pieces[temp_square] == 8) || (pieces[temp_square] == 10)) && attacking_side == WHITE) {
                    ///cout << "is_square_attacked(): yes. square " << square << " is being attacked by a white rook or queen\n"; 
                    return true;
                }
                if (((pieces[temp_square] == 7) || (pieces[temp_square] == 9)) && attacking_side == BLACK) {
                    ///cout << "is_square_attacked(): yes. square " << square << " is being attacked by a black rook or queen\n"; 
                    return true;
                }
                break;
            }
            temp_square += rook_movements[i];
        }
    }

    //Check if square square is being attacked by attacking_side by any bishops or queens...
    temp_square = 0;
    for (int i = 0; i < 4; i++) {
        temp_square = square + bishop_movements[i];
        while (pieces[temp_square] != -1) {
            if (pieces[temp_square] != 0) {
                if (((pieces[temp_square] == 4) || (pieces[temp_square] == 10)) && attacking_side == WHITE) {
                    ///cout << "is_square_attacked(): yes. square " << square << " is being attacked by a white bishop or queen\n";
                    return true;
                }
                if (((pieces[temp_square] == 3) || (pieces[temp_square] == 9)) && attacking_side == BLACK) {
                    ///cout << "is_square_attacked(): yes. square " << square << " is being attacked by a white bishop or queen\n"; 
                    return true;
                }
                break;
            }
            temp_square += bishop_movements[i];
        }
    }

    //Check if square square is being attacked by attacking_side by any kings (with the courage to attack)...
    for (int i = 0; i < 8; i++) {

        //If square square is being attacked by a white knight, return true
        if (attacking_side == WHITE && pieces[square + king_movements[i]] == 12) {
            ///cout << "is_square_attacked(): yes. square " << square << " is being attacked by a white king\n"; 
            return true;
        }
        else {
            //If square square is being attacked by a black knight, return true
            if (pieces[square + king_movements[i]] == 11 && attacking_side == BLACK) {
                ///cout << "is_square_attacked(): yes. square " << square << " is being attacked by a black king\n"; 
                return true;
            }
        }
    }

    ///cout << "is_square_attacked(): no. square " << square << " is not being attacked by anyone\n";
    return false;
}

//Remove the piece from square square and update variables as needed
void Board_Structure::remove_piece_from_square(int square) noexcept
{
    //Update position_ID, material, and piece_count
    position_ID ^= piece_square_key[pieces[square]][square];
    material[get_piece_color(pieces[square])] -= piece_value[pieces[square]];
    piece_count[pieces[square]]--;

    //Set square to EMPTY
    pieces[square] = EMPTY;
}

//Add piece piece to square square and update variables as needed
void Board_Structure::add_piece_to_square(int square, int piece) noexcept
{
    //Update position_ID, material, and piece_count
    position_ID ^= piece_square_key[pieces[square]][square];
    material[get_piece_color(pieces[square])] += piece_value[pieces[square]];
    piece_count[pieces[square]]++;

    //Set square to the piece
    pieces[square] = piece;
}

//Move piece piece from square from_square to square to_square
void Board_Structure::move_piece_to_square(int fromSquare, int toSquare) noexcept
{
    const int piece = pieces[fromSquare];

    //Update position_ID and set to_square to the piece and set from_square to EMPTY
    position_ID ^= piece_square_key[pieces[fromSquare]][fromSquare];
    pieces[fromSquare] = EMPTY;
    position_ID ^= piece_square_key[pieces[toSquare]][toSquare];
    pieces[toSquare] = piece;
}

//Modifies the board and stores the move in history[]. 
//Returns false if after making the move the side making the move leaves themselves in check,
//or in other cases when the move is invalid
bool Board_Structure::make_move(Move m)
{
    //Inits and validations before we even attempt to make the move--------------------------------------	
    const int from_square = m.get_from_square();
    const int to_square = m.get_to_square();
    const int captured_piece = m.get_captured_piece();
    const int castling = m.get_castling();
    const int promoted_piece = m.get_promoted();
    const int side = side_to_move;

    //If board is not valid, show an error message
    if (!is_board_valid()) {
        cout << "\nERROR: BOARD NOT VALID.\n";
        return false;
    }

    //If piece on from_square is not valid, show an error message
    if ((pieces[from_square] < B_PAWN) || (pieces[from_square] > W_KING)) {
        cout << "\nERROR: PIECE NUMBER " << pieces[from_square] << " ON SQUARE " << from_square << " IS NOT VALID.\n";
        return false;
    }

    //If square from_square or to_square is not on the board, show an error message
    if (files[from_square] == -1) {
        cout << "\nERROR: FROMSQUARE " << from_square << " IS NOT VALID.\n";
        return false;
    }
    if (files[to_square] == -1) {
        cout << "\nERROR: TOSQUARE " << to_square << " IS NOT VALID.\n";
        return false;
    }

    //End of validations -------------------------------------------------------------------------

    //Start of performing the special movement portions of special moves (castling, en passant) -----------------------------

    //Update history[]'s position ID
    history[history_ply].position_ID = position_ID;

    //If en passant move, make the en passant portion (i.e. clearing a pawn) of the move
        //to do later

    //If castling move, make the rook movement portion of the castling move
    if (castling != 0) {
        switch (to_square) {
            //White Kingside Castling
        case 27:
            move_piece_to_square(28, 26); break;
            //White Queenside Castling
        case 23:
            move_piece_to_square(21, 24); break;
            //Black Kingside Castling
        case 97:
            move_piece_to_square(98, 96); break;
            //Black Queenside Castling
        case 93:
            move_piece_to_square(91, 94); break;
        default:
            cout << "\nERROR: INVALID CASTILING TOSQUARE.\n"; break;
        }
    }

    //End of performing the special movement portions of special moves (castling, en passant) -----------------------------

    //Update history[]
    history[history_ply].move = m.move;
    history[history_ply].castle_perms = castle_perms;
    history[history_ply].en_pass_square = en_pass_square;

    //Update castling permissions
    //If a piece moves from a8, a1, e1, or e8, update castling permissions as needed
    castle_perms &= castle_permissions[from_square];
    castle_perms &= castle_permissions[to_square];

    //Update en_pass_square to 0, as the enpassant portion of the move has already been made
    en_pass_square = 0;

    //If a capturing move, remove captured piece
    if (captured_piece != EMPTY) {
        remove_piece_from_square(to_square);
    }

    //Update history_ply and ply
    history_ply++; ply++;

    //Update en pass square if needed
    /*if (pieces[m.get_from_square()] == W_PAWN || (pieces[m.get_from_square()] == B_PAWN)) {
        if (side_to_move == WHITE) {
            en_pass_square = m.get_from_square() + 10;
        }
        else {
            en_pass_square = m.get_from_square() - 10;
        }
    }*/

    //Now that the captured piece is removed, we can now actually move the piece we want to move
    move_piece_to_square(from_square, to_square);

    //If this is a promotion move, then promote the pawn
    if (castling == 0 && promoted_piece != 0) {
        remove_piece_from_square(to_square);
        add_piece_to_square(to_square, promoted_piece);
    }

    //Update king square if the king moved
    if ((pieces[to_square] == W_KING) || (pieces[to_square] == B_KING)) {
        king_square[side_to_move] = to_square;
    }

    //Flip side to move and update position_ID
    side_to_move ^= 1; position_ID = generate_and_get_position_ID();

    //If the king is under attack (in check) now, undo the move and return false
    if (is_square_attacked(king_square[side], side_to_move) == true) {
        undo_move();
        return false;
    }

    return true;
}

//Undoes the last move
void Board_Structure::undo_move()
{
    //Update ply and history_ply
    ply--; history_ply--;

    //Init variables
    const int m = history[history_ply].move;
    const int from_square = m & 0x7F;
    const int to_square = (m >> 7) & 0x7F;
    const int captured = (m >> 14) & 0xF;
    const int promoted_piece = (m >> 18) & 0xF;
    const int castling = (m >> 22) & 0x7;

    //If square from_square or to_square is not on the board, output an error message
    if (from_square >= board_square_count || files[from_square] == -1) {
        cout << "\nERROR: FROMSQUARE " << from_square << " IS NOT VALID.\n";
    }
    if (to_square >= board_square_count || files[to_square] == -1) {
        cout << "\nERROR: TOSQUARE " << to_square << " IS NOT VALID.\n";
    }

    //Revert castle_perms and en_pass_square
    castle_perms = history[history_ply].castle_perms;
    en_pass_square = history[history_ply].en_pass_square;

    //Flip side to move and update position_ID
    side_to_move ^= 1; position_ID = generate_and_get_position_ID();

    //If en passant move, undo the en passant portion (i.e. clearing a pawn) of the move
        //to do later

    //If castling move, undo the rook movement portion of the castling move
    //In other words move the rooks back to their home squares if needed
    if (castling != 0) {
        switch (to_square) {
            //White Kingside Castling
        case 27:
            move_piece_to_square(26, 28); break;
            //White Queenside Castling
        case 23:
            move_piece_to_square(24, 21); break;
            //Black Kingside Castling
        case 97:
            move_piece_to_square(96, 98); break;
            //Black Queenside Castling
        case 93:
            move_piece_to_square(94, 91); break;
        default:
            cout << "\nERROR: INVALID CASTILING TOSQUARE.\n"; break;
        }
    }

    //Move piece back to from_square.
    move_piece_to_square(to_square, from_square);

    //Update KingSq if the king moved
    if ((pieces[from_square] == W_KING) || (pieces[from_square] == B_KING)) {
        king_square[side_to_move] = from_square;
    }

    //Then, undo clearing of piece due to capture if there was a capture
    if (captured != EMPTY) {
        add_piece_to_square(to_square, captured);
    }

    //Then, undo promotion if there was a promotion
    if (promoted_piece != EMPTY) {
        remove_piece_from_square(from_square);
        add_piece_to_square(from_square, ((get_piece_color(pieces[from_square]) == WHITE) ? W_PAWN : B_PAWN));
    }

    //Update posiition ID
    position_ID = generate_and_get_position_ID();

}

// Counts all the pieces on the board and records them in the piece_count[] array
void Board_Structure::count_pieces() noexcept
{
    //Initialize piece_count[] array
    for (int i = 0; i < 13; i++) {
        piece_count[i] = 0;
    }

    //Loop through the entire board
    for (int i = 0; i < board_square_count; i++) {
        switch (pieces[i]) {
        case EMPTY: piece_count[EMPTY]++; break;
        case B_PAWN: piece_count[B_PAWN]++; break;
        case W_PAWN: piece_count[W_PAWN]++; break;
        case B_BISHOP: piece_count[B_BISHOP]++; break;
        case W_BISHOP: piece_count[W_BISHOP]++; break;
        case B_KNIGHT: piece_count[B_KNIGHT]++; break;
        case W_KNIGHT: piece_count[W_KNIGHT]++; break;
        case B_ROOK: piece_count[B_ROOK]++; break;
        case W_ROOK: piece_count[W_ROOK]++; break;
        case B_QUEEN: piece_count[B_QUEEN]++; break;
        case W_QUEEN: piece_count[W_QUEEN]++; break;
        case B_KING: piece_count[B_KING]++; break;
        case W_KING: piece_count[W_KING]++; break;
        }
    }
}

//Has this position occured before in the game? If yes, return true. Used for checking threefold repetition.
bool Board_Structure::is_repetition() noexcept
{
    for (int i = 0; i < history_ply - 1; i++) {
        if (position_ID == history[i].position_ID) {
            return true;
        }
    }
    return false;
}

//Generate and return a position id representing this board's position
U64 Board_Structure::generate_and_get_position_ID() noexcept
{
    U64 id = 0;

    //Go through each square on the board
    for (int i = 0; i < board_square_count; i++) {

        //If the square contains a piece, modify id 
        if (pieces[i] > 0) {

            //Set id to id XOR piece_square_key
            //cout << "i:" << i << " pieces[i]: " << pieces[i] << "\n";
            id ^= piece_square_key[pieces[i]][i];
        }
    }

    //Modify id if white to move
    if (side_to_move == WHITE) {
        id ^= side_key;
    }

    //Modify id if en passant square exists
    if (en_pass_square != EMPTY) {
        id ^= piece_square_key[EMPTY][en_pass_square];
    }

    //Modify id using castling permissions
    id ^= castle_perm_key[castle_perms];

    ///cout << "\nPosition ID generated: " << id << "\n";

    return id;
}

//Return false if there is something wrong with the current board
bool Board_Structure::is_board_valid()
{
    //Make sure position ID is good
    if (position_ID != generate_and_get_position_ID()) {
        cout << "ERROR: POSITION ID " << position_ID << " INVALID\n";
        return false;
    }

    //Make sure en pass square is valid
    if (en_pass_square != 0 && ranks[en_pass_square] != RANK_6 && ranks[en_pass_square] != RANK_3) {
        cout << "ERROR: EN PASS SQUARE," << en_pass_square << " IS INVALID\n";
        return false;
    }

    //Make sure KingSq arrays are valid
    if (pieces[king_square[WHITE]] != W_KING) {
        cout << "ERROR: pieces[king_square[WHITE]]  pieces[" << king_square[WHITE] << "] = " << pieces[king_square[WHITE]] << " WHICH IS NOT A WHITE KING.\n";
        return false;
    }
    if (pieces[king_square[BLACK]] != B_KING) {
        cout << "ERROR: pieces[king_square[BLACK]] = pieces[" << king_square[BLACK] << "] = " << pieces[king_square[BLACK]] << " WHICH IS NOT A BLACK KING.\n";
        return false;
    }


    //Make sure side makes sense
    if ((side_to_move != WHITE) && (side_to_move != BLACK)) {
        cout << "ERROR: SIDE " << side_to_move << " IS INVALID.\n";
        return false;
    }

    //Return true otherwise
    return true;
}
