#include "defs.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
#include <windows.h>
#include <fstream>

using namespace std;

int numEvaluatedMoves; int numEvalsPerMove;
string x; ofstream out("out.txt", std::ios_base::out | std::ios_base::app);
bool debug = false;

const int PAWN_TABLE[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	10, 10, 0, -10, -10, 0, 10, 10,
	5, 0, 0, 5, 5, 0, 0, 5,
	0, 0, 10, 20, 20, 10, 0, 0,
	5, 5, 5, 10, 10, 5, 5, 5,
	10, 10, 10, 20, 20, 10, 10, 10,
	20, 20, 20, 30, 30, 20, 20, 20,
	0, 0, 0, 0, 0, 0, 0, 0
};

const int KNIGHT_TABLE[64] = {
	0, -10, 0, 0, 0, 0, -10, 0,
	0, 0, 0, 5, 5, 0, 0, 0,
	0, 0, 10, 10, 10, 10, 0, 0,
	0, 0, 10, 20, 20, 10, 5, 0,
	5, 10, 15, 20, 20, 15, 10, 5,
	5, 10, 10, 20, 20, 10, 10, 5,
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

const int BISHOP_TABLE[64] = {
	0, 0, -10, 0, 0, -10, 0, 0,
	0, 0, 0, 10, 10, 0, 0, 0,
	0, 0, 10, 15, 15, 10, 0, 0,
	0, 10, 15, 20, 20, 15, 10, 0,
	0, 10, 15, 20, 20, 15, 10, 0,
	0, 0, 10, 15, 15, 10, 0, 0,
	0, 0, 0, 10, 10, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

const int ROOK_TABLE[64] = {
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 5, 10, 10, 5, 0, 0,
	25, 25, 25, 25, 25, 25, 25, 25,
	0, 0, 5, 10, 10, 5, 0, 0
};

const int MIRROR_64[64] = {
	56, 57, 58, 59, 60, 61, 62, 63,
	48, 49, 50, 51, 52, 53, 54, 55,
	40, 41, 42, 43, 44, 45, 46, 47,
	32, 33, 34, 35, 36, 37, 38, 39,
	24, 25, 26, 27, 28, 29, 30, 31,
	16, 17, 18, 19, 20, 21, 22, 23,
	8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7
};

//Prints a move in UCI format to a file given a move. E.g. uciPrintMove(board, m)
void uciPrintMoveToFileGivenMove(BoardStructure board, Move m) {
	const char PIECE_NUM_TO_CHAR[13] = { ' ', ' ', ' ', 'B', 'B', 'N', 'N', 'R', 'R', 'Q', 'Q', 'K', 'K' };

	int fromSquare = m.getFromSquare();
	int toSquare = m.getToSquare();

	out << FILES_TO_CHAR[FILES[fromSquare]] << (1 + RANKS[fromSquare]);
	out << FILES_TO_CHAR[FILES[toSquare]] << (1 + RANKS[toSquare]);
}

//Inits AI. This is called in findBestMove().
void AI::init(BoardStructure board) {
	//Note that histPly stores # of half-moves for the entire game,
	// while ply stores # of half-moves for the current search
	board.ply = 0;

	table.clearPVTable();
	startTime = GetTickCount();
	endTime = 0; numOfNodes = 0;
}

//Returns the best move found by the nega-max function
Move AI::findBestMove(BoardStructure board, int depth){
	
	Move bestMove; bestMove.move = 0;
	int bestScore = -INFIN, pvMovesCount = 0;
	
	init(board);

	//Negamax
	bestScore = negaMax(-INFIN, INFIN, board, depth);

	//Fill pvArray and get number of moves in pv
	pvMovesCount = table.getPVLine(board, depth);

	//Get the best move from the pvArray
	bestMove.move = pvArray[0];
	///cout << "bestMove is:"; movelist.uciPrintMoveGivenMove(board, bestMove);

	//Fill pvArray and get number of moves in pv
	pvMovesCount = table.getPVLine(board, depth);

	cout << "\n\nI, " << NAME << ", have decided to make move ";
	movelist.uciPrintMoveGivenMove(board, bestMove);
	cout << " after searching to depth " << ai.maxDepth << ".\n";

	//Print the pv (principal variation)
	cout << "\nPrincipal variation is:\n";
	for (int i = 0; i < pvMovesCount; i++) {
		movelist.uciPrintMoveGivenMoveInt(board, pvArray[i]);
	}

	return bestMove;
}

//Negamax with alpha-beta pruning
int AI::negaMax(int alpha, int beta, BoardStructure board, int depth)
{
	numOfNodes++;

	//If depth == 0, we have reached a leaf and must go back up the tree
	if (depth == 0) {
		return evaluate(board);
	}

	//If this is a tie, return 0
	if (board.isRepetition()) {
		return 0;
	}

	//Make sure ply isn't too high
	if (board.ply > 63) {
		return evaluate(board);
	}

	//Generate all moves for this position
	MoveListGenerator gen1;
	gen1.generateMoveList(board);

	///cout << "haha movelist.moves[0]:"; movelist.uciPrintMoveGivenMove(board, movelist.moves[0]);

	//Go through all of the generated moves
	Move bestMove; bestMove.move = 0; 
	int numOfLegalMoves = 0, score = -INFIN, oldAlpha = alpha;

	for (int i = 0; i < gen1.numberOfMoves; i++) {

		//Make move i. If move i is invalid, go to the next move in the move list
		if (!board.makeMove(gen1.moves[i])) {
			continue;
		}

		//Outputting stuff to log file debugging purposes
		if (debug == true) {
			if (depth == maxDepth) {
				if (i == 0) {
					cout << "Check out out.txt for kouri's full thoughts on this.\n";
				}
				out << "d:" << depth << " i:" << i << " Thinking about valid move ";
				uciPrintMoveToFileGivenMove(board, gen1.moves[i]); out << "...\n";
			}
			if (depth == maxDepth - 1) {
				out << "    d:" << depth << " i:" << i << " move ";
				uciPrintMoveToFileGivenMove(board, gen1.moves[i]);  out << "...\n";
			}
			if (depth == maxDepth - 2) {
				out << "       d:" << depth << " i:" << i << " move ";
				uciPrintMoveToFileGivenMove(board, gen1.moves[i]);
				out << " has score ";
			}
		}

		//Print what kouri is thinking about
		if (depth == maxDepth) {
			cout << "\ni:" << i << " thinking about valid move ";
			gen1.uciPrintMoveGivenMove(board, gen1.moves[i]); cout << "...";
		}

		//If move i is valid (legal), continue 
		
		//Increment legal counter
		numOfLegalMoves++; 

		//Call negaMax() to get the move's score
		score = -negaMax(-beta, -alpha, board, depth - 1);

		if (depth == maxDepth) {
			cout << " score: " << score;
		}

		//Outputting stuff to log file debugging purposes
		if (debug == true) {
			if (depth == maxDepth - 2) {
				out << score << "\n";
			}
			if (depth == maxDepth - 1) {
				out << "    Therefore, ";
				uciPrintMoveToFileGivenMove(board, gen1.moves[i]); out << "...";
				out << " has score " << score << "\n";
			}
			if (depth == maxDepth) {
				out << "Therefore, ";
				uciPrintMoveToFileGivenMove(board, gen1.moves[i]); out << "...";
				out << " has score " << score << "\n";
			}
		}

		//Undo the move we just made
		board.undoMove();
		
		if (score > alpha) {
			if (score >= beta) {
				return beta;
			}
			alpha = score;

			//if (depth == maxDepth) {
				bestMove = gen1.moves[i];
			//}
		}

	}

	//What if there are no legal moves found? We must be in checkmate or stalemate!
	if (numOfLegalMoves == 0) {
		///cout << "No legal moves!";
		//If our king is attacked, it is checkmate! Game over.
		if (board.isSquareAttacked(board.kingSquare[board.sideToMove], board.sideToMove ^ 1)) {
			///cout << "CHECKMATE FOUND.\n";
			//Note that adding board.ply means that a mate in 3 is better than a mate in 6
			return -MATE + board.ply;
		}

		//Otherwise, it is stalemate. Good game.
		else {
			return 0;
		}

	}

	//Store the best move
	if (alpha != oldAlpha) {
		table.storePVMove(board, bestMove.move);
	}

	return alpha;
} 

//Evaluates board FROM THE PERSECTIVE OF board.sideToMove
int AI::evaluate(BoardStructure board){
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

	if (board.sideToMove == WHITE) {
		return (materialScore + positionalScore);
	}
	else {
		return -(materialScore + positionalScore);
	}
}

