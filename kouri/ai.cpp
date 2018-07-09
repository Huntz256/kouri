#include "defs.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
#include <windows.h>
#include <ctime>

using namespace std;

// Piece square tables
// These are used in evaluation to assign values to specific pieces on specific locations.

// The d2 and e2 pawns should advance before the other pawns. 
const int PAWN_TABLE[64] = {
	0,   0,  0,  0,    0,  0,  0,  0,
	10, 10,  0, -10, -10,  0, 10, 10,
	5,   0,  0,  5,    5,  0,  0,  5,
	0,   0,  10, 25,  25, 10,  0,  0,
	5,   5,  5,  10,  10,  5,  5,  5,
	10, 10, 10,  20,  20, 10, 10, 10,
	20, 20, 20,  30,  30, 20, 20, 20,
	0,   0,  0,  0,    0,  0,  0,  0
};

// Knights should go toward the center
const int KNIGHT_TABLE[64] = {
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
const int BISHOP_TABLE[64] = {
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
// 7th rank if possible.
const int ROOK_TABLE[64] = {
	0,   0,  5, 10, 10,  5,  0,  0,
	0,   0,  5, 10, 10,  5,  0,  0,
	0,   0,  5, 10, 10,  5,  0,  0,
	0,   0,  5, 10, 10,  5,  0,  0,
	0,   0,  5, 10, 10,  5,  0,  0,
	0,   0,  5, 10, 10,  5,  0,  0,
	25, 25, 25, 25, 25, 25, 25, 25,
	0,   0,  5, 10, 10,  5, 0,  0
};

const int MIRROR_64[64] = {
	56, 57, 58, 59, 60, 61, 62, 63,
	48, 49, 50, 51, 52, 53, 54, 55,
	40, 41, 42, 43, 44, 45, 46, 47,
	32, 33, 34, 35, 36, 37, 38, 39,
	24, 25, 26, 27, 28, 29, 30, 31,
	16, 17, 18, 19, 20, 21, 22, 23,
	 8,  9, 10, 11, 12, 13, 14, 15,
	 0,  1,  2,  3,  4,  5,  6,  7
};

//Inits AI. This is called in findBestMove().
void AI::init(BoardStructure& board) {
	// Reset bestMove to no move
	bestMove.move = 0;

	//Note that histPly stores # of half-moves for the entire game,
	// while ply stores # of half-moves for the current search
	board.ply = 0;

	table.clearPVTable();
	numOfNodes = 0;
	numOfEvals = 0;
}

//Returns the best move found by the negamax function
Move AI::findBestMove(BoardStructure& board, int depth) {

	int bestScore = -INFIN, pvMovesCount = 0;

	//Clock to retrieve system time
	clock_t timer = clock();

	//Inits the AI
	init(board);

	//Negamax searches for the best move
	bestScore = negamax(-INFIN, INFIN, board, depth);

	//Print the best move
	cout << "bestMove is:"; movelist.uciPrintMoveGivenMove(board, bestMove);

	//Fill pvArray and get number of moves in pv
	pvMovesCount = table.getPVLine(board, depth);

	//Find time difference between previously retrieved system time and current time
	timer = clock() - timer;

	if (!UCIMODE) {
		cout << "\n\nI, " << NAME << ", have decided to make move ";
		movelist.uciPrintMoveGivenMove(board, bestMove);
		cout << " after searching to depth " << ai.maxDepth << ".\n";
		cout << "\nNumber of nodes scanned: " << numOfNodes << ". Number of evaluations made: " << numOfEvals << ".\n";
		cout << "Total calculation time: " << ((float)timer) / CLOCKS_PER_SEC << " seconds. \n"; //Convert clock_t object to time in seconds and print out

		//Print the pv (principal variation)
		cout << "\nPrincipal variation is:\n";
		for (int i = 0; i < pvMovesCount; i++) {
			movelist.uciPrintMoveGivenMoveInt(board, pvArray[i]);
		}
	}

	return bestMove;
}

// Negamax with alpha-beta pruning.
// Searches recursively for the best move in the board position given by 
// the parameter board.
int AI::negamax(int alpha, int beta, BoardStructure& board, int depth)
{
	numOfNodes++;

	//If depth == 0 or ply too high, go back up the tree
	if (depth == 0 || board.ply > 63) {
		numOfEvals++;
		return evaluate(board);
	}

	//If this is a tie, return 0
	if (board.isRepetition()) {
		return 0;
	}

	//Generate all moves for this position
	MoveListGenerator gen1;
	gen1.generateMoveList(board);

	//Default best move
	if (depth == maxDepth) {
		bestMove = gen1.moves[0];
	}

	Move pvBestMove; pvBestMove.move = 0;
	int numOfLegalMoves = 0, score = -INFIN, oldAlpha = alpha;

	//Go through all of the generated moves
	for (int i = 0; i < gen1.numberOfMoves; i++) {

		//Make move i. If move i is invalid, go to the next move in the move list
		if (!board.makeMove(gen1.moves[i])) {
			if (!UCIMODE && depth == maxDepth) {
				cout << "\ni:" << i << " move ";
				gen1.uciPrintMoveGivenMove(board, gen1.moves[i]); cout << " is invalid.";
			}
			continue;
		}

		//If move i is valid (legal), continue and increment legal counter
		numOfLegalMoves++;

		//Print what kouri is thinking about
		if (!UCIMODE && depth == maxDepth) {
			cout << "\ni:" << i << " thinking about valid move ";
			gen1.uciPrintMoveGivenMove(board, gen1.moves[i]); cout << "...";
		}

		//Call negaMax() to get the move's score
		score = -negamax(-beta, -alpha, board, depth - 1);

		if (!UCIMODE && depth == maxDepth) {
			cout << " score: " << score;
		}

		//Undo the move we just made
		board.undoMove();

		if (score > alpha) {
			if (score >= beta) {
				return beta;
			}
			alpha = score;

			pvBestMove = gen1.moves[i];
			if (depth == maxDepth) {
				bestMove = gen1.moves[i];
				if (!UCIMODE) {
					cout << "\nSetting bestMove to ";
					gen1.uciPrintMoveGivenMove(board, gen1.moves[i]);
				}
			}
		}

	}

	//What if there are no legal moves found? We must be in checkmate or stalemate!
	if (numOfLegalMoves == 0) {

		//If our king is attacked, it is checkmate! Game over.
		if (board.isSquareAttacked(board.kingSquare[board.sideToMove], board.sideToMove ^ 1)) {

			//Note that adding board.ply means that a mate in 3 is better than a mate in 6
			return -MATE + board.ply;
		}

		//Otherwise, it is stalemate. Good game.
		else {
			return 0;
		}

	}

	//Store the best move in pv table
	if (alpha != oldAlpha) {
		table.storePVMove(board, pvBestMove.move);
	}

	return alpha;
}

// Returns an an evaluation of the board.
//  - A positive score represents a board in favor of the size to move, while a negative score means that
//  the other side has an advantage.
//  - Material: See PIECE_VALUE[]. A pawn is worth 100 points, a bishop 300 points, etc.
int AI::evaluate(BoardStructure& board) {
	//Evaluate material
	board.countPieces();
	int materialScore = PIECE_VALUE[12] * (board.pieceCount[W_KING] - board.pieceCount[B_KING])
		+ PIECE_VALUE[10] * (board.pieceCount[W_QUEEN] - board.pieceCount[B_QUEEN])
		+ PIECE_VALUE[8] * (board.pieceCount[W_ROOK] - board.pieceCount[B_ROOK])
		+ PIECE_VALUE[6] * (board.pieceCount[W_BISHOP] - board.pieceCount[B_BISHOP] + board.pieceCount[W_KNIGHT] - board.pieceCount[B_KNIGHT])
		+ PIECE_VALUE[2] * (board.pieceCount[W_PAWN] - board.pieceCount[B_PAWN]);

	//Evaluate position
	int positionalScore = 0;
	for (int i = 21; i < 98; i++) {
		if (RANKS[i] != -1) {
			if (board.pieces[i] == W_PAWN) {
				positionalScore += PAWN_TABLE[squareID64[i]];
			}
			else if (board.pieces[i] == B_PAWN) {
				positionalScore -= PAWN_TABLE[MIRROR_64[squareID64[i]]];
			}
			else if (board.pieces[i] == W_KNIGHT) {
				positionalScore += KNIGHT_TABLE[squareID64[i]];
			}
			else if (board.pieces[i] == B_KNIGHT) {
				positionalScore -= KNIGHT_TABLE[MIRROR_64[squareID64[i]]];
			}
			else if (board.pieces[i] == W_BISHOP) {
				positionalScore += BISHOP_TABLE[squareID64[i]];
			}
			else if (board.pieces[i] == B_BISHOP) {
				positionalScore -= BISHOP_TABLE[MIRROR_64[squareID64[i]]];
			}
			else if (board.pieces[i] == W_ROOK) {
				positionalScore += ROOK_TABLE[squareID64[i]];
			}
			else if (board.pieces[i] == B_ROOK) {
				positionalScore -= ROOK_TABLE[MIRROR_64[squareID64[i]]];
			}
		}
	}

	// Evaluate mobility
	int mobility = 0;
	movelist.generateMoveList(board);
	mobility += movelist.numberOfMoves;
	board.sideToMove ^= 1;
	movelist.generateMoveList(board);
	mobility -= movelist.numberOfMoves;
	board.sideToMove ^= 1;

	if (board.sideToMove == WHITE) {
		return (materialScore + positionalScore + mobility);
	}
	else {
		return -(materialScore + positionalScore + mobility);
	}
}

vector<int> AI::getEvaluationBreakdown(BoardStructure& board) {

	vector<int> evaluation;

	//Evaluate material
	board.countPieces();
	int materialScore = PIECE_VALUE[12] * (board.pieceCount[W_KING] - board.pieceCount[B_KING])
		+ PIECE_VALUE[10] * (board.pieceCount[W_QUEEN] - board.pieceCount[B_QUEEN])
		+ PIECE_VALUE[8] * (board.pieceCount[W_ROOK] - board.pieceCount[B_ROOK])
		+ PIECE_VALUE[6] * (board.pieceCount[W_BISHOP] - board.pieceCount[B_BISHOP] + board.pieceCount[W_KNIGHT] - board.pieceCount[B_KNIGHT])
		+ PIECE_VALUE[2] * (board.pieceCount[W_PAWN] - board.pieceCount[B_PAWN]);

	//Evaluate position
	int positionalScore = 0;
	for (int i = 21; i < 98; i++) {
		if (RANKS[i] != -1) {
			if (board.pieces[i] == W_PAWN) {
				positionalScore += PAWN_TABLE[squareID64[i]];
			}
			else if (board.pieces[i] == B_PAWN) {
				positionalScore -= PAWN_TABLE[MIRROR_64[squareID64[i]]];
			}
			else if (board.pieces[i] == W_KNIGHT) {
				positionalScore += KNIGHT_TABLE[squareID64[i]];
			}
			else if (board.pieces[i] == B_KNIGHT) {
				positionalScore -= KNIGHT_TABLE[MIRROR_64[squareID64[i]]];
			}
			else if (board.pieces[i] == W_BISHOP) {
				positionalScore += BISHOP_TABLE[squareID64[i]];
			}
			else if (board.pieces[i] == B_BISHOP) {
				positionalScore -= BISHOP_TABLE[MIRROR_64[squareID64[i]]];
			}
			else if (board.pieces[i] == W_ROOK) {
				positionalScore += ROOK_TABLE[squareID64[i]];
			}
			else if (board.pieces[i] == B_ROOK) {
				positionalScore -= ROOK_TABLE[MIRROR_64[squareID64[i]]];
			}
		}
	}

	// Evaluate mobility
	int mobility = 0;
	movelist.generateMoveList(board);
	mobility += movelist.numberOfMoves;
	board.sideToMove ^= 1;
	movelist.generateMoveList(board);
	mobility -= movelist.numberOfMoves;
	board.sideToMove ^= 1;

	if (board.sideToMove == WHITE) {
		evaluation.push_back(materialScore);
		evaluation.push_back(positionalScore);
		evaluation.push_back(mobility);
	}
	else {
		evaluation.push_back(-materialScore);
		evaluation.push_back(-positionalScore);
		evaluation.push_back(-mobility);
	}

	return evaluation;
}

