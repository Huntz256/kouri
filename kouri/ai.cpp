#include "defs.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
#include <windows.h>
using namespace std;

int numEvaluatedMoves; int numEvalsPerMove;
string x;

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
	cout << "bestMove is:"; movelist.uciPrintMoveGivenMove(board, bestMove);

	//Fill pvArray and get number of moves in pv
	pvMovesCount = table.getPVLine(board, depth);

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
	if (depth == maxDepth) {
		gen1.printMoveList(board);
	}

	///cout << "haha movelist.moves[0]:"; movelist.uciPrintMoveGivenMove(board, movelist.moves[0]);

	//Go through all of the generated moves
	Move bestMove = gen1.moves[0];
	int numOfLegalMoves = 0, score = -INFIN, oldAlpha = alpha;

	for (int i = 0; i < gen1.numberOfMoves; i++) {

		//Make move i. If move i is invalid, go to the next move in the move list
		if (!board.makeMove(gen1.moves[i])) {
			continue;
		}

		if (depth == maxDepth) {
			cout << "d:" << depth << " i:" << i << " Thinking about valid move ";
			gen1.uciPrintMoveGivenMove(board, gen1.moves[i]); cout << "...";
			///cout << " (f:" << gen1.moves[i].getFromSquare() << " t:" << gen1.moves[i].getToSquare() << " c:" << gen1.moves[i].getCapturedPiece() << " p: " << gen1.moves[i].getPromoted() << ")\n";
		}

		//If move i is valid (legal), continue 
		
		//Increment legal counter
		numOfLegalMoves++; 

		//Call negaMax() to get the move's score
		score = -negaMax(-beta, -alpha, board, depth - 1);

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
			cout << "CHECKMATE FOUND.\n";
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
int AI::evaluate(BoardStructure board) {
	int score = board.material[WHITE] - board.material[BLACK];
	if (board.sideToMove == WHITE) {
		//cout << "evaluate() for side " << board.sideToMove << "is " << score;
		return score;
	}
	else {
		//cout << "evaluate() for side " << board.sideToMove << "is " << (-score);
		return -score;
	}
}
