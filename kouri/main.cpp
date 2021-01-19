#include "defs.h"
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <sstream>
using namespace std;

BoardStructure board; //BoardStructure* boardpt;
U64 pieceSquareKey[13][BOARD_SQUARE_COUNT], sideKey, castlePermKey[16];
MoveListGenerator movelist;
PVTable table;
AI ai;
int pvArray[64];
bool UCIMODE = true;

int getRandomInteger(int min, int max) noexcept
{
    srand((int)time(nullptr));
    return rand() % (max - min + 1) + min;
}

U64 getRandom64BitInteger() noexcept
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
void playerMove()
{
    int x;
    do {
        cout << "\n\nIt's your turn. Choose a move from the list above and enter the move number:";
        cin >> x;
        board.makeMove(movelist.moves[x]);
    } while ((x < 0) || (x >= movelist.numberOfMoves));
}

//Assumes input is a move command. Returns move integer if valid, else returns -1
int translateMoveCommand(string com) noexcept
{
    //Castling
    if (com.length() == 3 && board.sideToMove == WHITE && com.compare("O-O") == 0) return MOVE(25, 27, 0, 0, 1);
    if (com.length() == 5 && board.sideToMove == WHITE && com.compare("O-O-O") == 0) return MOVE(25, 23, 0, 0, 2);
    if (com.length() == 3 && board.sideToMove == BLACK && com.compare("O-O") == 0) return MOVE(95, 97, 0, 0, 3);
    if (com.length() == 5 && board.sideToMove == BLACK && com.compare("O-O-O") == 0) return MOVE(95, 93, 0, 0, 4);

    //Make sure com is valid 
    if (com.length() < 4 || com[0] - 'a' < 0 || com[0] - 'a' > 7 || com[2] - 'a' < 0 || com[2] - 'a' > 7 ||
        com[1] < '1' || com[1] > '8' || com[3] < '1' || com[3] > '8') {
        return -1;
    }

    //Regular move with possible promotion
    const int from = (com[0] - 'a' + 1) + (com[1] - '1' + 2) * 10;
    const int to = (com[2] - 'a' + 1) + (com[3] - '1' + 2) * 10;
    const int prom = (com.length() == 5) ? charToPieceInt(com[4]) : 0;

    ///cout << "Attempting to make non-castling move:" << from << " " << to << " " << board.pieces[to] << " " << prom << "\n";
    return MOVE(from, to, board.pieces[to], prom, 0);
}

//Parse an FEN string
int testFunction1()
{
    string fen;
    board.displayBoard();
    cout << "\nGive me a valid FEN string:\n";
    getline(cin, fen);

    while (true) {
        if (board.setUpBoardUsingFEN(fen) == -1) {
            return -1;
        }
        board.displayBoard(); testIsSquareAttacked(WHITE, board); testIsSquareAttacked(BLACK, board);
        movelist.generateMoveList(board); movelist.printMoveList(board);
        cout << "\nGive me a valid FEN string:\n";
        getline(cin, fen);
    }
}

//Castling testing
int testFunction2()
{
    Move m; string next;

    //Set up board for castling tests
    cout << "\nTesting white castling king side\n";
    if (board.setUpBoardUsingFEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
        return -1;
    }
    board.displayBoard(); cout << "\n";
    cout << "Press enter to continue:";
    getline(cin, next);

    //White castle king side (e1g1)
    m.move = MOVE(25, 27, 0, 0, 1);
    board.makeMove(m);
    //Output fromSquare and toSquare board indices
    cout << "from: " << m.getFromSquare() << " to: " << m.getToSquare() << "\n\n";

    //Display the board
    board.displayBoard(); cout << "\n";

    cout << "Press enter to continue:";
    getline(cin, next);

    /******************New test*********************/
    cout << "\nTesting white castling queen side\n";
    if (board.setUpBoardUsingFEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
        return -1;
    }
    board.displayBoard(); cout << "\n";
    cout << "Press enter to continue:";
    getline(cin, next);

    //White castle queen side (d7d5)
    m.move = MOVE(25, 23, 0, 0, 2);
    board.makeMove(m);
    //Output move's from square and to square
    cout << "from: " << m.getFromSquare() << " to: " << m.getToSquare() << "\n\n";

    //Display the board
    board.displayBoard(); cout << "\n";

    cout << "Press enter to continue:";
    getline(cin, next);

    /******************New test*********************/
    cout << "\nTesting black castling king side\n";
    if (board.setUpBoardUsingFEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
        return -1;
    }
    board.displayBoard(); cout << "\n";
    cout << "Press enter to continue:";
    getline(cin, next);

    //White castle queen side (d7d5)
    m.move = MOVE(95, 97, 0, 0, 3);
    board.makeMove(m);
    //Output move's from square and to square
    cout << "from: " << m.getFromSquare() << " to: " << m.getToSquare() << "\n\n";

    //Display the board
    board.displayBoard(); cout << "\n";

    cout << "Press enter to continue:";
    getline(cin, next);

    /******************New test*********************/
    cout << "\nTesting black castling queen side\n";
    if (board.setUpBoardUsingFEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1") == -1) {
        return -1;
    }
    board.displayBoard(); cout << "\n";
    cout << "Press enter to continue:";
    getline(cin, next);

    //White castle queen side (d7d5)
    m.move = MOVE(95, 93, 0, 0, 4);
    board.makeMove(m);
    //Output move's from square and to square
    cout << "from: " << m.getFromSquare() << " to: " << m.getToSquare() << "\n\n";

    //Display the board
    board.displayBoard(); cout << "\n";

    cout << "Press enter to continue:";
    getline(cin, next);

    return 0;
}

//Have kouri play against itself
void testFunction3()
{
    string x; board.init(true);

    while (true) {
        board.displayBoard();
        movelist.generateMoveList(board); movelist.printMoveList(board);
        int moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);
        while (!board.makeMove(movelist.moves[moveNum])) {
            moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);
        }

        cout << "\n\n" << NAME << " has decided to make move " << moveNum << "!";
        //board.sideToMove = board.sideToMove ^ 1;

        getline(cin, x);
    }
}

//Play against kouri by selecting from movelist
void testFunction4()
{
    string x; board.init(true);
    //board.setUpBoardUsingFEN("rnbqkbnr/ppp2ppp/3p4/4p2Q/4P3/8/PPPP1PPP/RNB1KBNR w KQkq - 0 1 ");

    while (true) {
        board.displayBoard();
        movelist.generateMoveList(board); movelist.printMoveList(board);

        playerMove();

        board.displayBoard();
        movelist.generateMoveList(board); movelist.printMoveList(board);
        int moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);

        while (!board.makeMove(movelist.moves[moveNum])) {
            moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);
        }

        cout << "\n\nI, " << NAME << ", have decided to make move " << moveNum << ".";
    }
}

//Play against kouri without a movelist
void testFunction5()
{
    string x; Move m;
    board.init(true);

    string help = "To make a move, type a command in the form: e2e4 \nPawn promotions are done as such: a1b2Q \nCastling is done using: O-O or O-O-O \nFor a movelist, type: hint \nTo forfeit, type: f ";

    while (true) {
        movelist.generateMoveList(board);

        board.displayBoard();

        cout << "For a list of commands, type: help \nEnter your command: ";
        getline(cin, x);

        //While user command is not valid
        while (x.compare("f") == 0 || x.compare("hint") == 0 || x.compare("help") == 0 || !(movelist.isMoveValid(board, translateMoveCommand(x)))) {
            if (x.compare("help") != 0 && x.compare("hint") != 0) {

                cout << "\n\nThat is not a valid move or command. For a list of commands, type: help \n";
            }
            board.displayBoard();

            if (x.compare("hint") == 0) movelist.printMoveList(board);
            else if (x.compare("f") == 0) break;
            else if (x.compare("help") == 0) cout << help << "\n";

            cout << "Enter your command: ";
            getline(cin, x);
        }

        if (x.compare("f") == 0) break; //exit game loop

        m.move = translateMoveCommand(x);

        while (!board.makeMove(m)) {
            m.move = translateMoveCommand(x);
        }

        board.displayBoard();
        movelist.generateMoveList(board); movelist.printMoveList(board);

        int moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);


        while (!board.makeMove(movelist.moves[moveNum])) {
            moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);
        }

        cout << "\n\nI, " << NAME << ", have decided to make move " << moveNum << ".";
    }
}

//Does a small promotion test
void testFunction20()
{
    board.setUpBoardUsingFEN("rnbqk2r/ppppppPp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.displayBoard();
    Move m; m.move = MOVE(87, 98, B_ROOK, W_QUEEN, 0);
    board.makeMove(m);
    board.displayBoard();

    string x; getline(cin, x);
}

//Board evaluation tests
void testFunction21()
{
    string x;
    board.setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.displayBoard();
    cout << "White: " << ai.evaluate(board) << "\n";
    cout << "Black: " << ai.evaluate(board) << "\n";
    getline(cin, x);

    board.setUpBoardUsingFEN("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.displayBoard();
    cout << "White: " << ai.evaluate(board) << "\n";
    cout << "Black: " << ai.evaluate(board) << "\n";
    getline(cin, x);

    board.setUpBoardUsingFEN("rnbqkbnr/8/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.displayBoard();
    cout << "White: " << ai.evaluate(board) << "\n";
    cout << "Black: " << ai.evaluate(board) << "\n";
    getline(cin, x);

    board.setUpBoardUsingFEN("8/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.displayBoard();
    cout << "White: " << ai.evaluate(board) << "\n";
    cout << "Black: " << ai.evaluate(board) << "\n";
    getline(cin, x);

    board.setUpBoardUsingFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1");
    board.displayBoard();
    cout << "White: " << ai.evaluate(board) << "\n";
    cout << "Black: " << ai.evaluate(board) << "\n";
    getline(cin, x);
}

//Used for testing makeMove() and undoMove()
void testFunction70()
{
    //Init board
    string x; board.init(true);
    board.displayBoard();

    //Make a move, display board, pause
    Move m; m.move = MOVE(35, 55, 0, 0, 0); board.makeMove(m); cout << "MOVE MADE.\n";
    board.displayBoard(); getline(cin, x);

    //Undo move, display board, pause
    board.undoMove();  cout << "MOVE UNDONE.\n";
    board.displayBoard(); getline(cin, x);

    //Make and undo more moves
    for (int i = 0; i < 30; i++) {
        movelist.generateMoveList(board); 
        const int moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);
        m.move = movelist.moves[moveNum].move; board.makeMove(m);
        cout << "i:" << i << "\n";
        cout << "movelist.numberOfMoves:" << movelist.numberOfMoves << "\n";
        cout << "moveNum:" << moveNum << "\n";
    }
    board.displayBoard(); getline(cin, x);
    for (int i = 0; i < 15; i++) {
        board.undoMove();  cout << "board.ply: " << board.ply << ". MOVE UNDONE.\n";
    }

    board.displayBoard(); getline(cin, x);

    for (int i = 0; i < 15; i++) {
        board.undoMove(); cout << "board.ply: " << board.ply << ". MOVE UNDONE.\n";
    }

    board.displayBoard(); getline(cin, x);

    //Make and undo more moves
    for (int i = 0; i < 50; i++) {
        cout << "i:" << i << "\n";

        const int moveNum = getRandomInteger(0, movelist.numberOfMoves - 1);
        cout << "moveNum:" << moveNum << "\n";
        movelist.generateMoveList(board);
        cout << "movelist.numberOfMoves:" << movelist.numberOfMoves << "\n";
        m.move = movelist.moves[moveNum].move; board.makeMove(m);

    }
    for (int i = 0; i < 50; i++) {
        board.undoMove();
    }
    cout << "MOVESSS UNDONE.\n";

    board.displayBoard(); getline(cin, x);
}

//Play against AI-enabled kouri
void testFunction23()
{
    string x; Move m; board.init(true);
    string help = "\nTo make a move, type a command in the form: a2a4 \nPawn promotions are done as such: a1b2=Q \nCastling is done using: O-O or O-O-O \nFor a movelist, type: hint \nTo forfeit, type: f \n";
    bool quit = false;

    /***Set depth here***/
    ai.maxDepth = 4;
    /********************/

    board.init(true);

    while (!quit) {
        board.displayBoard();

        // Check if ai has won
        movelist.generateMoveList(board);
        if (movelist.getNumLegalMoves(board) == 0) {
            if (board.isSquareAttacked(board.kingSquare[board.sideToMove], board.sideToMove ^ 1)) {
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

        while (x.compare("f") == 0 || x.compare("hint") == 0 || x.compare("help") == 0 || !(movelist.isMoveValid(board, translateMoveCommand(x)))) {
            if (x.compare("help") != 0 && x.compare("hint") != 0 && x.compare("f") != 0) {
                cout << "\n\nThat is not a valid move or command.\n";
            }

            if (x.compare("hint") == 0) movelist.printMoveList(board);
            else if (x.compare("f") == 0) break;
            else if (x.compare("help") == 0) cout << help << "\n";

            cout << "Enter your command (e.g. e2e4): ";
            getline(cin, x);
        }

        // Quit game if player requests so
        if (x.compare("f") == 0) break;

        // Make player's move
        m.move = translateMoveCommand(x);

        if (!board.makeMove(m)) {
            cout << "Error";
            quit = true; break;
        }

        // Check if player has won
        movelist.generateMoveList(board);
        if (movelist.getNumLegalMoves(board) == 0) {
            if (board.isSquareAttacked(board.kingSquare[board.sideToMove], board.sideToMove ^ 1)) {
                cout << "\nYou won. >:( \n";
            }
            else {
                cout << "\nIt's a tie!\n";
            }
            quit = true; break;
        }

        // Get move from AI and make AI's move
        m = ai.findBestMove(board, ai.maxDepth);
        board.makeMove(m);
    }

    cout << "\nGAME OVER\n";

    //Windows-exclusive "Press any key to continue" command. Used in place of the getline() function
    system("PAUSE");
}

//Play against yourself; used to test pv table and rep rule
void testFunction422()
{
    board.init(true);
    Move m; 
    string x;
    int max = 0;

    while (true) {
        board.displayBoard();

        cout << "Enter your command: "; getline(cin, x);

        //While user command is not valid
        while (!(x.compare("p") == 0 || x.compare("u") == 0 || movelist.isMoveValid(board, translateMoveCommand(x)))) {
            if (x.compare("p") != 0 && x.compare("u") != 0) {
                cout << "\n\nThat is not a valid move or command. Enter p, u, or a valid move.\n";
            }
            board.displayBoard();
            cout << "Enter your command: "; getline(cin, x);
        }

        if (x.compare("u") == 0) {
            board.undoMove();
        }
        else if (x.compare("p") == 0) {
            ///max = table.getPVLine(board, 4);
            cout << "max: " << max << "\n";
            for (int i = 0; i < max; i++) {
                m.move = pvArray[i];
                movelist.uciPrintMoveGivenMove(m);
            }

        }
        else {
            m.move = translateMoveCommand(x);

            if (m.move != 0) {
                table.storePVMove(board, m.move);
                board.makeMove(m);
            }
            else {
                cout << "Move not Parsed.\n";
            }
        }

        if (board.isRepetition()) {
            cout << "REP REP REP REP REP REP REP REP. \n";
        }
    }
}

//Init pieceSquareKey[][] with random 64 bit integers. 
//Used to generate position ids for the threefold repetition rule
void initKeys() noexcept
{
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 120; j++) {
            pieceSquareKey[i][j] = getRandom64BitInteger();
        }
    }

    sideKey = getRandom64BitInteger();

    for (int i = 0; i < 16; i++) {
        castlePermKey[i] = getRandom64BitInteger();
    }
}

// Interface with UCI GUI
void uci(string in)
{
    string uciCommand = in; Move m;
    ai.maxDepth = 4;

    do {
        if (uciCommand == "uci") {
            cout << "id name Kouri\n";
            cout << "id author Hunter and Minh\n";
            cout << "uciok\n";
        }
        else if (uciCommand == "isready") {
            cout << "readyok\n";
        }
        else if (uciCommand.substr(0, 17) == "position startpos") {
            board.init(true);

            string buf;
            stringstream ss(uciCommand);
            vector<string> words;
            while (ss >> buf)
                words.push_back(buf);

            for (auto& word : words) {
                if (word != "position" && word != "startpos" && word != "moves") {
                    m.move = translateMoveCommand(word);
                    board.makeMove(m);
                }
            }


        }
        else if (uciCommand.substr(0, 2) == "go") {
            m = ai.findBestMove(board, ai.maxDepth);

            cout << "bestmove ";
            movelist.uciPrintMoveGivenMove(m);
            cout << "\n";
        }
    } while (getline(cin, uciCommand));
}

int main()
{
    initKeys();

    cout << "~Kouri~\nIf you are not a chess GUI, enter \"human\".\n>>";
    string in;
    getline(cin, in);

    // Toggle UCI mode off if not interfacing with UCI GUI
    if (in.compare("human") == 0) {
        UCIMODE = false;
    }
    else {
        UCIMODE = true;
    }

    // Interface with UCI GUI
    if (UCIMODE) {
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
            testFunction23();
        }
        else if (in.compare("2") == 0) {
            testFunction3();
        }
        else if (in.compare("3") == 0) {
            testFunction422();
        }

        // Castling testing
        else if (in.compare("4") == 0) {
            testFunction2();
        }

        // Parse a FEN string
        else if (in.compare("5") == 0) {
            testFunction1();
        }

        // Player vs kouri that makes random moves
        else if (in.compare("6") == 0) {
            testFunction5();
        }
    }

    return 0;
}
