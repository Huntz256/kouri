#include "ai.h"
#include "board.h"
#include "misc.h"
#include "moves.h"
#include "pvtable.h"

#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <sstream>
using namespace std;

Board_Structure board;
U64 piece_square_key[13][board_square_count], side_key, castle_perm_key[16];
Move_List_Generator move_list;
PV_Table table;
AI ai;
int pv_array[64];
bool uci_mode = true;

int get_random_integer(int min, int max) noexcept
{
    srand((int)time(nullptr));
    return rand() % (max - min + 1) + min;
}

U64 get_random_64_bit_integer() noexcept
{
    // If rand() gives a 15 bit random number:
    //
    // (U64) rand() = 0000 000000000000000 000000000000000 000000000000000 ???????????????
    // (U64) rand() << 15 = 0000 000000000000000 000000000000000 ??????????????? 000000000000000 
    // (U64) rand() << 30 = 0000 000000000000000 ??????????????? 000000000000000 000000000000000
    // (U64) rand() << 45 = 0000 ??????????????? 000000000000000 000000000000000 000000000000000
    // ( (U64) rand() & 0xf ) << 60 = ???? 000000000000000 000000000000000 000000000000000 000000000000000

    return ((U64)rand()) + ((U64)rand() << 15) + ((U64)rand() << 30) + ((U64)rand() << 45) + (((U64)rand() & 0xf) << 60);
}

// Assumes input is a move command. Returns move integer if valid, else returns -1
int translate_move_command(string com) noexcept
{
    // Castling
    if (com.length() == 3 && board.side_to_move == WHITE && com.compare("O-O") == 0) return MOVE(25, 27, 0, 0, 1);
    if (com.length() == 5 && board.side_to_move == WHITE && com.compare("O-O-O") == 0) return MOVE(25, 23, 0, 0, 2);
    if (com.length() == 3 && board.side_to_move == BLACK && com.compare("O-O") == 0) return MOVE(95, 97, 0, 0, 3);
    if (com.length() == 5 && board.side_to_move == BLACK && com.compare("O-O-O") == 0) return MOVE(95, 93, 0, 0, 4);

    // Make sure com is valid 
    if (com.length() < 4 || com[0] - 'a' < 0 || com[0] - 'a' > 7 || com[2] - 'a' < 0 || com[2] - 'a' > 7 ||
        com[1] < '1' || com[1] > '8' || com[3] < '1' || com[3] > '8') {
        return -1;
    }

    const int from = (com[0] - 'a' + 1) + (com[1] - '1' + 2) * 10;
    const int to = (com[2] - 'a' + 1) + (com[3] - '1' + 2) * 10;

    // Castling
    if (com.length() == 4 && board.side_to_move == WHITE && from == 25 && to == 27) return MOVE(25, 27, 0, 0, 1);
    if (com.length() == 4 && board.side_to_move == WHITE && from == 25 && to == 23) return MOVE(25, 23, 0, 0, 2);
    if (com.length() == 4 && board.side_to_move == BLACK && from == 95 && to == 97) return MOVE(95, 97, 0, 0, 3);
    if (com.length() == 4 && board.side_to_move == BLACK && from == 95 && to == 93) return MOVE(95, 93, 0, 0, 4);

    // Regular move with possible promotion
    const int prom = (com.length() == 5) ? char_to_piece_int(com[4], board.side_to_move) : 0;

    ///cout << "Attempting to make non-castling move:" << from << " " << to << " " << board.pieces[to] << " " << prom << "\n";
    return MOVE(from, to, board.pieces[to], prom, 0);
}

// Pause until enter is pressed
void pause()
{
    string s;
    cout << "\nPress enter to continue...";
    getline(cin, s);
}

// Init piece_square_key[][] with random 64 bit integers. 
// Used to generate position ids for the threefold repetition rule
void init_keys() noexcept
{
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 120; j++) {
            piece_square_key[i][j] = get_random_64_bit_integer();
        }
    }

    side_key = get_random_64_bit_integer();

    for (int i = 0; i < 16; i++) {
        castle_perm_key[i] = get_random_64_bit_integer();
    }
}


// Play against kouri
void test_player_vs_kouri()
{
    string x;
    string help = "\nTo make a move, type a command in the form: a2a4 \nPawn promotions are done as such: a1b2=Q \nCastling is done using: O-O or O-O-O \nFor a movelist, type: hint \nTo undo, type: undo\nTo forfeit, type: f \n";
    bool quit = false;

    board.init(true);

    while (!quit) {
        board.display_board();

        // Check if ai has won
        move_list.generate_move_list(board);
        if (move_list.get_legal_moves_count(board) == 0) {
            if (board.is_square_attacked(board.king_square[board.side_to_move], board.side_to_move ^ 1)) {
                cout << "\nKouri has won. :)\n";
            }
            else {
                cout << "\nIt's a tie!\n";
            }
            quit = true;
            break;
        }

        // Get player input
        cout << "For a list of commands, type: help \nEnter your command (e.g. e2e4): ";
        getline(cin, x);

        while (x == "help" || x == "hint" || x == "f" || x == "undo" || !(move_list.is_move_valid(board, translate_move_command(x)))) {
            if (x != "help" && x != "hint" && x != "f" && x != "undo") {
                cout << "\n\nThat is not a valid move or command.\n";
            }

            if (x == "hint") {
                move_list.print_move_list(board);
            }
            else if (x == "f") {
                break;
            }
            else if (x == "help") {
                cout << help << "\n";
            }
            else if (x == "undo") {
                if (board.history_ply > 1) {
                    board.undo_move();
                    board.undo_move();
                    break;
                }
                else {
                    cout << "Cannot undo.\n";
                }
            }

            cout << "Enter your command (e.g. e2e4): ";
            getline(cin, x);
        }

        // Quit game if player requests so
        if (x == "f") {
            break;
        }

        // Display the board after undo
        else if (x == "undo") {
            continue;
        }

        // Make player's move
        Move m(translate_move_command(x));

        if (!board.make_move(m)) {
            cout << "Error";
            quit = true;
            break;
        }

        // Check if player has won
        move_list.generate_move_list(board);
        if (move_list.get_legal_moves_count(board) == 0) {
            if (board.is_square_attacked(board.king_square[board.side_to_move], board.side_to_move ^ 1)) {
                cout << "\nYou won. >:( \n";
            }
            else {
                cout << "\nIt's a tie!\n";
            }
            quit = true;
            break;
        }

        // Get move from AI and make AI's move
        m = ai.find_best_move(board, ai.max_depth);
        board.make_move(m);
    }

    cout << "\nGAME OVER\n";

    pause();
}

// Have kouri play against itself
void test_kouri_vs_kouri()
{
    Move m(0);
    string x;
    board.init(true);

    while (true) {
        board.display_board();

        // Check if game is over
        move_list.generate_move_list(board);
        if (move_list.get_legal_moves_count(board) == 0) {
            if (board.is_square_attacked(board.king_square[board.side_to_move], board.side_to_move ^ 1)) {
                if ((board.side_to_move ^ 1) == WHITE) {
                    cout << "\nWhite has won.\n";
                }
                else {
                    cout << "\nBlack has won.\n";
                }
            }
            else {
                cout << "\nIt's a tie!\n";
            }
            break;
        }

        // Get move from AI and make AI's move
        m = ai.find_best_move(board, ai.max_depth);
        board.make_move(m);
    }

    pause();
}

// Play against yourself to test pv table and rep rule
void test_player_vs_player()
{
    board.init(true);
    string x;
    int max = 0;

    while (true) {
        board.display_board();

        cout << "Enter your command: ";
        getline(cin, x);

        // While user command is not valid
        while (!(x == "p" || x == "u" || move_list.is_move_valid(board, translate_move_command(x)))) {
            if (x != "p" && x != "u") {
                cout << "\n\nThat is not a valid move or command. Enter p, u, or a valid move.\n";
            }
            board.display_board();
            cout << "Enter your command: ";
            getline(cin, x);
        }

        if (x == "u") {
            board.undo_move();
        }
        else if (x == "p") {
            ///max = table.get_PV_line(board, 4);
            cout << "max: " << max << "\n";
            for (int i = 0; i < max; i++) {
                move_list.print_move_uci(Move(pv_array[i]));
            }
        }
        else {
            Move m(translate_move_command(x));
            if (m.move() != 0) {
                table.store_PV_move(board, m.move());
                board.make_move(m);
            }
            else {
                cout << "Move not parsed.\n";
            }
        }

        if (board.is_repetition()) {
            cout << "REP REP REP REP REP REP REP REP. \n";
        }
    }
}

// Test parsing an FEN string
int test_fen()
{
    string fen;
    board.display_board();
    cout << "\nGive me a valid FEN string:\n";
    getline(cin, fen);

    while (true) {
        if (board.setup_board_using_FEN(fen) == -1) {
            return -1;
        }
        board.display_board();
        move_list.generate_move_list(board);
        move_list.print_move_list(board);

        cout << "Best move: ";
        uci_mode = true;
        move_list.print_move_algebraic(Move(ai.find_best_move(board, ai.max_depth)), board);
        uci_mode = false;

        cout << "\nGive me a valid FEN string:\n";
        getline(cin, fen);
    }
}

// Test castling
int test_castling()
{
    vector<string> messages = {
        "\nTesting white castling king side\n",
        "\nTesting white castling queen side\n",
        "\nTesting black castling king side\n",
        "\nTesting black castling queen side\n"
    };
    vector<string> fens = {
        "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
        "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
        "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq - 0 1",
        "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq - 0 1"
    };
    vector<Move> moves = {
        Move(MOVE(25, 27, 0, 0, 1)), Move(MOVE(25, 23, 0, 0, 2)),
        Move(MOVE(95, 97, 0, 0, 3)), Move(MOVE(95, 93, 0, 0, 4))
    };

    for (size_t i = 0; i != fens.size(); i++) {
        cout << messages[i];
        if (board.setup_board_using_FEN(fens[i]) == -1) {
            return -1;
        }
        board.display_board();
        pause();
        board.make_move(moves[i]);
        board.display_board();
        pause();
    }

    return 0;
}

// Does a small promotion test
void test_promotion()
{
    board.setup_board_using_FEN("rnbqk2r/ppppppPp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.display_board();
    pause();

    const Move m(MOVE(87, 98, B_ROOK, W_QUEEN, 0));
    board.make_move(m);
    board.display_board();
    pause();
}

// Board evaluation tests
void test_evaluation()
{
    vector<string> fens = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "rnbqkbnr/8/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "8/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1"
    };

    for (string const& fen : fens) {
        board.setup_board_using_FEN(fen);
        board.display_board();
        cout << "Evaluation: " << ai.evaluate(board) << "\n";
        pause();
    }
}

// Test make_move() and undo_move()
void test_make_and_undo_move()
{
    // Init board
    board.init(true);
    board.display_board();

    // Make a move and display board
    board.make_move(Move(MOVE(35, 55, 0, 0, 0)));
    cout << "MOVE MADE.\n";
    board.display_board();
    pause();

    // Undo move and display board
    board.undo_move();
    cout << "MOVE UNDONE.\n";
    board.display_board();
    pause();

    // Make and undo more moves
    for (int i = 0; i < 30; i++) {
        move_list.generate_move_list(board);
        const int move_num = get_random_integer(0, move_list.moves_count - 1);
        board.make_move(move_list.moves[move_num]);
        cout << "MOVE MADE.\n";
    }
    board.display_board();
    pause();
    for (int i = 0; i < 15; i++) {
        board.undo_move();
        cout << "board.ply: " << board.ply << ". MOVE UNDONE.\n";
    }
    board.display_board();
    pause();
    for (int i = 0; i < 15; i++) {
        board.undo_move();
        cout << "board.ply: " << board.ply << ". MOVE UNDONE.\n";
    }
    board.display_board();
    pause();
}


// Interface with UCI GUI
void uci_interface(string in)
{
    string uci_command = in;

    do {
        if (uci_command == "uci") {
            cout << "id name Kouri\n";
            cout << "id author Hunter and Minh\n";
            cout << "uciok\n";
        }
        else if (uci_command == "isready") {
            cout << "readyok\n";
        }
        else if (uci_command.substr(0, 17) == "position startpos") {
            board.init(true);

            string buf;
            stringstream ss(uci_command);
            vector<string> words;
            while (ss >> buf)
                words.push_back(buf);

            for (auto& word : words) {
                if (word != "position" && word != "startpos" && word != "moves") {
                    board.make_move(Move(translate_move_command(word)));
                }
            }
        }
        else if (uci_command.substr(0, 2) == "go") {
            const Move m = ai.find_best_move(board, ai.max_depth);

            cout << "info depth " << ai.max_depth << " score cp " << ai.best_move_score() << " nodes " << ai.node_count() << " pv ";
            move_list.print_move_uci(m);

            cout << "\nbestmove ";
            move_list.print_move_uci(m);
            cout << "\n";
        }
        else if (uci_command.substr(0, 4) == "undo") { // For debugging purposes - not an official UCI command
            if (board.history_ply > 0) {
                board.undo_move();
            }
        }
        else if (uci_command.substr(0, 7) == "display") { // For debugging purposes - not an official UCI command
            board.display_board();
        }
        else if (uci_command.substr(0, 4) == "quit") {
            break;
        }
    } while (getline(cin, uci_command));
}

// Interface with human
void human_interface()
{
    cout << "\nHello. My name is " << name << ".\n";
    cout << "\nI have been created by Minter (Hunter and Minh).";
    cout << "\nCurrently, I understand most rules of chess.";
    cout << "\nI also have some idea regarding what makes one move better than another.\n\n";

    cout << "Choose one:\n1 - Play against kouri\n2 - Test kouri vs kouri\n3 - Test player vs player\n4 - Test FEN parsing\n5 - Test castling\n6 - Test promotion\n7 - Test evaluation\n8 - Test make and undo move\n "
        << "\n>> ";
    string in;
    getline(cin, in);

    if (in == "1") {
        test_player_vs_kouri();
    }
    else if (in == "2") {
        test_kouri_vs_kouri();
    }
    else if (in == "3") {
        test_player_vs_player();
    }
    else if (in == "4") {
        test_fen();
    }
    else if (in == "5") {
        test_castling();
    }
    else if (in == "6") {
        test_promotion();
    }
    else if (in == "7") {
        test_evaluation();
    }
    else if (in == "8") {
        test_make_and_undo_move();
    }
    else {
        test_player_vs_kouri();
    }
}

int main()
{
    init_keys();

    cout << "~Kouri~\nIf you are not a chess GUI, enter \"human\".\n>> ";
    string in;
    getline(cin, in);

    // Toggle UCI mode off if not interfacing with UCI GUI
    if (in == "human") {
        uci_mode = false;
    }
    else {
        uci_mode = true;
    }

    // Interface with UCI GUI or a human
    if (uci_mode) {
        uci_interface(in);
    }
    else {
        human_interface();
    }

    return 0;
}
