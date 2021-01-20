#include "defs.h"
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <sstream>
using namespace std;

Board_Structure board; //Board_Structure* boardpt;
U64 piece_square_key[13][BOARD_SQUARE_COUNT], side_key, castle_perm_key[16];
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
    //rand() gives a 15 bit random number. Let's say that this random number is 101010101010101
    //We want a 64 bit random number such as 0000 000000000000000 000000000000000 000000000000000 000000000000000

    //(U64) rand() = 0000 000000000000000 000000000000000 000000000000000 101010101010101
    //(U64) rand() << 15 = 0000 000000000000000 000000000000000 101010101010101 000000000000000 
    //(U64) rand() << 30 = 0000 000000000000000 101010101010101 000000000000000 000000000000000
    //(U64) rand() << 45 = 0000 101010101010101 000000000000000 000000000000000 000000000000000
    //( (U64) rand() & 0xf ) << 60 = 1010 000000000000000 000000000000000 000000000000000 000000000000000

    //So, if we add the above values up, we should get a 64 bit random number!
    return ((U64)rand()) + ((U64)rand() << 15) + ((U64)rand() << 30) + ((U64)rand() << 45) + (((U64)rand() & 0xf) << 60);
}

//Make a move from the move list using user input
void player_move()
{
    int x;
    do {
        cout << "\n\nIt's your turn. Choose a move from the list above and enter the move number:";
        cin >> x;
        board.make_move(move_list.moves[x]);
    } while ((x < 0) || (x >= move_list.moves_count));
}

//Assumes input is a move command. Returns move integer if valid, else returns -1
int translate_move_command(string com) noexcept
{
    //Castling
    if (com.length() == 3 && board.side_to_move == WHITE && com.compare("O-O") == 0) return MOVE(25, 27, 0, 0, 1);
    if (com.length() == 5 && board.side_to_move == WHITE && com.compare("O-O-O") == 0) return MOVE(25, 23, 0, 0, 2);
    if (com.length() == 3 && board.side_to_move == BLACK && com.compare("O-O") == 0) return MOVE(95, 97, 0, 0, 3);
    if (com.length() == 5 && board.side_to_move == BLACK && com.compare("O-O-O") == 0) return MOVE(95, 93, 0, 0, 4);

    //Make sure com is valid 
    if (com.length() < 4 || com[0] - 'a' < 0 || com[0] - 'a' > 7 || com[2] - 'a' < 0 || com[2] - 'a' > 7 ||
        com[1] < '1' || com[1] > '8' || com[3] < '1' || com[3] > '8') {
        return -1;
    }

    const int from = (com[0] - 'a' + 1) + (com[1] - '1' + 2) * 10;
    const int to = (com[2] - 'a' + 1) + (com[3] - '1' + 2) * 10;

    //Castling
    if (com.length() == 4 && board.side_to_move == WHITE && from == 25 && to == 27) return MOVE(25, 27, 0, 0, 1);
    if (com.length() == 4 && board.side_to_move == WHITE && from == 25 && to == 23) return MOVE(25, 23, 0, 0, 2);
    if (com.length() == 4 && board.side_to_move == BLACK && from == 95 && to == 97) return MOVE(95, 97, 0, 0, 3);
    if (com.length() == 4 && board.side_to_move == BLACK && from == 95 && to == 93) return MOVE(95, 93, 0, 0, 4);

    //Regular move with possible promotion
    const int prom = (com.length() == 5) ? char_to_piece_int(com[4]) : 0;

    ///cout << "Attempting to make non-castling move:" << from << " " << to << " " << board.pieces[to] << " " << prom << "\n";
    return MOVE(from, to, board.pieces[to], prom, 0);
}

//Parse an FEN string
int test_function1()
{
    string fen;
    board.display_board();
    cout << "\nGive me a valid FEN string:\n";
    getline(cin, fen);

    while (true) {
        if (board.setup_board_using_FEN(fen) == -1) {
            return -1;
        }
        board.display_board(); test_is_square_attacked(WHITE, board); test_is_square_attacked(BLACK, board);
        move_list.generate_move_list(board); move_list.print_move_list(board);
        cout << "\nGive me a valid FEN string:\n";
        getline(cin, fen);
    }
}

//Castling testing
int test_function2()
{
    Move m; string next;

    //Set up board for castling tests
    cout << "\nTesting white castling king side\n";
    if (board.setup_board_using_FEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
        return -1;
    }
    board.display_board(); cout << "\n";
    cout << "Press enter to continue:";
    getline(cin, next);

    //White castle king side (e1g1)
    m.move = MOVE(25, 27, 0, 0, 1);
    board.make_move(m);
    //Output fromSquare and toSquare board indices
    cout << "from: " << m.get_from_square() << " to: " << m.get_to_square() << "\n\n";

    //Display the board
    board.display_board(); cout << "\n";

    cout << "Press enter to continue:";
    getline(cin, next);

    /******************New test*********************/
    cout << "\nTesting white castling queen side\n";
    if (board.setup_board_using_FEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
        return -1;
    }
    board.display_board(); cout << "\n";
    cout << "Press enter to continue:";
    getline(cin, next);

    //White castle queen side (d7d5)
    m.move = MOVE(25, 23, 0, 0, 2);
    board.make_move(m);
    //Output move's from square and to square
    cout << "from: " << m.get_from_square() << " to: " << m.get_to_square() << "\n\n";

    //Display the board
    board.display_board(); cout << "\n";

    cout << "Press enter to continue:";
    getline(cin, next);

    /******************New test*********************/
    cout << "\nTesting black castling king side\n";
    if (board.setup_board_using_FEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
        return -1;
    }
    board.display_board(); cout << "\n";
    cout << "Press enter to continue:";
    getline(cin, next);

    //White castle queen side (d7d5)
    m.move = MOVE(95, 97, 0, 0, 3);
    board.make_move(m);
    //Output move's from square and to square
    cout << "from: " << m.get_from_square() << " to: " << m.get_to_square() << "\n\n";

    //Display the board
    board.display_board(); cout << "\n";

    cout << "Press enter to continue:";
    getline(cin, next);

    /******************New test*********************/
    cout << "\nTesting black castling queen side\n";
    if (board.setup_board_using_FEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
        return -1;
    }
    board.display_board(); cout << "\n";
    cout << "Press enter to continue:";
    getline(cin, next);

    //White castle queen side (d7d5)
    m.move = MOVE(95, 93, 0, 0, 4);
    board.make_move(m);
    //Output move's from square and to square
    cout << "from: " << m.get_from_square() << " to: " << m.get_to_square() << "\n\n";

    //Display the board
    board.display_board(); cout << "\n";

    cout << "Press enter to continue:";
    getline(cin, next);

    return 0;
}

//Have kouri play against itself
void test_function3()
{
    string x; board.init(true);

    while (true) {
        board.display_board();
        move_list.generate_move_list(board); move_list.print_move_list(board);
        int moveNum = get_random_integer(0, move_list.moves_count - 1);
        while (!board.make_move(move_list.moves[moveNum])) {
            moveNum = get_random_integer(0, move_list.moves_count - 1);
        }

        cout << "\n\n" << NAME << " has decided to make move " << moveNum << "!";
        //board.side_to_move = board.side_to_move ^ 1;

        getline(cin, x);
    }
}

//Play against kouri by selecting from a move list
void test_function4()
{
    string x; board.init(true);
    //board.setup_board_using_FEN("rnbqkbnr/ppp2ppp/3p4/4p2Q/4P3/8/PPPP1PPP/RNB1KBNR w KQkq - 0 1 ");

    while (true) {
        board.display_board();
        move_list.generate_move_list(board); move_list.print_move_list(board);

        player_move();

        board.display_board();
        move_list.generate_move_list(board); move_list.print_move_list(board);
        int move_num = get_random_integer(0, move_list.moves_count - 1);

        while (!board.make_move(move_list.moves[move_num])) {
            move_num = get_random_integer(0, move_list.moves_count - 1);
        }

        cout << "\n\nI, " << NAME << ", have decided to make move " << move_num << ".";
    }
}

//Play against kouri without a move list
void test_function5()
{
    string x; Move m;
    board.init(true);

    string help = "To make a move, type a command in the form: e2e4 \nPawn promotions are done as such: a1b2Q \nCastling is done using: O-O or O-O-O \nFor a movelist, type: hint \nTo forfeit, type: f ";

    while (true) {
        move_list.generate_move_list(board);

        board.display_board();

        cout << "For a list of commands, type: help \nEnter your command: ";
        getline(cin, x);

        //While user command is not valid
        while (x.compare("f") == 0 || x.compare("hint") == 0 || x.compare("help") == 0 || !(move_list.is_move_valid(board, translate_move_command(x)))) {
            if (x.compare("help") != 0 && x.compare("hint") != 0) {

                cout << "\n\nThat is not a valid move or command. For a list of commands, type: help \n";
            }
            board.display_board();

            if (x.compare("hint") == 0) move_list.print_move_list(board);
            else if (x.compare("f") == 0) break;
            else if (x.compare("help") == 0) cout << help << "\n";

            cout << "Enter your command: ";
            getline(cin, x);
        }

        if (x.compare("f") == 0) break; //exit game loop

        m.move = translate_move_command(x);

        while (!board.make_move(m)) {
            m.move = translate_move_command(x);
        }

        board.display_board();
        move_list.generate_move_list(board); move_list.print_move_list(board);

        int move_num = get_random_integer(0, move_list.moves_count - 1);


        while (!board.make_move(move_list.moves[move_num])) {
            move_num = get_random_integer(0, move_list.moves_count - 1);
        }

        cout << "\n\nI, " << NAME << ", have decided to make move " << move_num << ".";
    }
}

//Does a small promotion test
void test_function20()
{
    board.setup_board_using_FEN("rnbqk2r/ppppppPp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.display_board();
    Move m; m.move = MOVE(87, 98, B_ROOK, W_QUEEN, 0);
    board.make_move(m);
    board.display_board();

    string x; getline(cin, x);
}

//Board evaluation tests
void test_function21()
{
    string x;
    board.setup_board_using_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.display_board();
    cout << "White: " << ai.evaluate(board) << "\n";
    cout << "Black: " << ai.evaluate(board) << "\n";
    getline(cin, x);

    board.setup_board_using_FEN("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.display_board();
    cout << "White: " << ai.evaluate(board) << "\n";
    cout << "Black: " << ai.evaluate(board) << "\n";
    getline(cin, x);

    board.setup_board_using_FEN("rnbqkbnr/8/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.display_board();
    cout << "White: " << ai.evaluate(board) << "\n";
    cout << "Black: " << ai.evaluate(board) << "\n";
    getline(cin, x);

    board.setup_board_using_FEN("8/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.display_board();
    cout << "White: " << ai.evaluate(board) << "\n";
    cout << "Black: " << ai.evaluate(board) << "\n";
    getline(cin, x);

    board.setup_board_using_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1");
    board.display_board();
    cout << "White: " << ai.evaluate(board) << "\n";
    cout << "Black: " << ai.evaluate(board) << "\n";
    getline(cin, x);
}

//Used for testing make_move() and undo_move()
void test_function70()
{
    //Init board
    string x; board.init(true);
    board.display_board();

    //Make a move, display board, pause
    Move m; m.move = MOVE(35, 55, 0, 0, 0); board.make_move(m); cout << "MOVE MADE.\n";
    board.display_board(); getline(cin, x);

    //Undo move, display board, pause
    board.undo_move();  cout << "MOVE UNDONE.\n";
    board.display_board(); getline(cin, x);

    //Make and undo more moves
    for (int i = 0; i < 30; i++) {
        move_list.generate_move_list(board); 
        const int moveNum = get_random_integer(0, move_list.moves_count - 1);
        m.move = move_list.moves[moveNum].move; board.make_move(m);
        cout << "i:" << i << "\n";
        cout << "move_list.moves_count:" << move_list.moves_count << "\n";
        cout << "move_num:" << moveNum << "\n";
    }
    board.display_board(); getline(cin, x);
    for (int i = 0; i < 15; i++) {
        board.undo_move();  cout << "board.ply: " << board.ply << ". MOVE UNDONE.\n";
    }

    board.display_board(); getline(cin, x);

    for (int i = 0; i < 15; i++) {
        board.undo_move(); cout << "board.ply: " << board.ply << ". MOVE UNDONE.\n";
    }

    board.display_board(); getline(cin, x);

    //Make and undo more moves
    for (int i = 0; i < 50; i++) {
        cout << "i:" << i << "\n";

        const int move_num = get_random_integer(0, move_list.moves_count - 1);
        cout << "move_num:" << move_num << "\n";
        move_list.generate_move_list(board);
        cout << "move_list.moves_count:" << move_list.moves_count << "\n";
        m.move = move_list.moves[move_num].move; board.make_move(m);

    }
    for (int i = 0; i < 50; i++) {
        board.undo_move();
    }
    cout << "MOVESSS UNDONE.\n";

    board.display_board(); getline(cin, x);
}

//Play against AI-enabled kouri
void test_function23()
{
    string x; Move m; board.init(true);
    string help = "\nTo make a move, type a command in the form: a2a4 \nPawn promotions are done as such: a1b2=Q \nCastling is done using: O-O or O-O-O \nFor a movelist, type: hint \nTo forfeit, type: f \n";
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
            quit = true; break;
        }

        // Get player input
        cout << "For a list of commands, type: help \nEnter your command (e.g. e2e4): ";
        getline(cin, x);

        while (x.compare("f") == 0 || x.compare("hint") == 0 || x.compare("help") == 0 || !(move_list.is_move_valid(board, translate_move_command(x)))) {
            if (x.compare("help") != 0 && x.compare("hint") != 0 && x.compare("f") != 0) {
                cout << "\n\nThat is not a valid move or command.\n";
            }

            if (x.compare("hint") == 0) move_list.print_move_list(board);
            else if (x.compare("f") == 0) break;
            else if (x.compare("help") == 0) cout << help << "\n";

            cout << "Enter your command (e.g. e2e4): ";
            getline(cin, x);
        }

        // Quit game if player requests so
        if (x.compare("f") == 0) break;

        // Make player's move
        m.move = translate_move_command(x);

        if (!board.make_move(m)) {
            cout << "Error";
            quit = true; break;
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
            quit = true; break;
        }

        // Get move from AI and make AI's move
        m = ai.find_best_move(board, ai.max_depth);
        board.make_move(m);
    }

    cout << "\nGAME OVER\n";

    //Windows-exclusive "Press any key to continue" command. Used in place of the getline() function
    system("PAUSE");
}

//Play against yourself; used to test pv table and rep rule
void test_function422()
{
    board.init(true);
    Move m; 
    string x;
    int max = 0;

    while (true) {
        board.display_board();

        cout << "Enter your command: "; getline(cin, x);

        //While user command is not valid
        while (!(x.compare("p") == 0 || x.compare("u") == 0 || move_list.is_move_valid(board, translate_move_command(x)))) {
            if (x.compare("p") != 0 && x.compare("u") != 0) {
                cout << "\n\nThat is not a valid move or command. Enter p, u, or a valid move.\n";
            }
            board.display_board();
            cout << "Enter your command: "; getline(cin, x);
        }

        if (x.compare("u") == 0) {
            board.undo_move();
        }
        else if (x.compare("p") == 0) {
            ///max = table.get_PV_line(board, 4);
            cout << "max: " << max << "\n";
            for (int i = 0; i < max; i++) {
                m.move = pv_array[i];
                move_list.uci_print_move_given_move(m);
            }

        }
        else {
            m.move = translate_move_command(x);

            if (m.move != 0) {
                table.store_PV_move(board, m.move);
                board.make_move(m);
            }
            else {
                cout << "Move not Parsed.\n";
            }
        }

        if (board.is_repetition()) {
            cout << "REP REP REP REP REP REP REP REP. \n";
        }
    }
}

//Init piece_square_key[][] with random 64 bit integers. 
//Used to generate position ids for the threefold repetition rule
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

// Interface with UCI GUI
void uci(string in)
{
    string uci_command = in; Move m;

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
                    m.move = translate_move_command(word);
                    board.make_move(m);
                }
            }


        }
        else if (uci_command.substr(0, 2) == "go") {
            m = ai.find_best_move(board, ai.max_depth);

            cout << "info depth " << ai.max_depth << " score cp " << ai.get_best_move_score() << " nodes " << ai.get_node_count() << " pv ";
            move_list.uci_print_move_given_move(m);

            cout << "\nbestmove ";
            move_list.uci_print_move_given_move(m);
            cout << "\n";
        }
        else if (uci_command.substr(0, 4) == "quit") {
            break;
        }
    } while (getline(cin, uci_command));
}

int main()
{
    init_keys();

    cout << "~Kouri~\nIf you are not a chess GUI, enter \"human\".\n>>";
    string in;
    getline(cin, in);

    // Toggle UCI mode off if not interfacing with UCI GUI
    if (in.compare("human") == 0) {
        uci_mode = false;
    }
    else {
        uci_mode = true;
    }

    // Interface with UCI GUI
    if (uci_mode) {
        uci(in);
    }

    // Interface with human
    else {
        cout << "\nHello. My name is " << NAME << ".\n";
        cout << "\nI have been created by Minter (Hunter and Minh).";
        cout << "\nCurrently, I understand most rules of chess.";
        cout << "\nI also have some idea regarding what makes one move better than another.\n\n";

        cout << "Choose one:\n \"1\" - Play against kouri\n\n "
            << "\n>> ";
        getline(cin, in);

        if (in.compare("1") == 0) {
            test_function23();
        }
        else if (in.compare("2") == 0) {
            test_function3();
        }
        else if (in.compare("3") == 0) {
            test_function422();
        }

        // Castling testing
        else if (in.compare("4") == 0) {
            test_function2();
        }

        // Parse a FEN string
        else if (in.compare("5") == 0) {
            test_function1();
        }

        // Player vs kouri that makes random moves
        else if (in.compare("6") == 0) {
            test_function5();
        }
    }

    return 0;
}
