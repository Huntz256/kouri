#include "defs.h"
#include <iostream>
#include <limits>
#include <algorithm>
using namespace std;

MoveListGenerator movegen;
int numEvaluatedMoves;

//Returns the best move found by the nega-max function
Move findBestMove(BoardStructure board, int depth){
	Move bestMove;
	movegen.generateMoveList(board);

	int a = -numeric_limits<int>::max(); //largest negative int possible
	int b = numeric_limits<int>::max(); //largest positive int possible
	for (Move m : movegen.moves){
		int value = negaMax(board, depth, a, b);
		if (value > a){
			a = value;
			bestMove = m;
		}
	}
	cout << numEvaluatedMoves << " moves evaluated\n";
	return bestMove;
}

//nega-max with alpha-beta pruning
int negaMax(BoardStructure board, int depth, int a, int b)
{
	if (depth <= 0)
		numEvaluatedMoves++;
		return evaluate(board);
	movegen.generateMoveList(board);
	for (Move m : movegen.moves) {
		board.makeMove(m);
		a = max(a, -negaMax(board, depth - 1, -a, -b));
		//board.undo();
		if (b <= a){
			return b;
		}
	}
	return a;
} 

//Evaluates board from WHITE's perspective
int evaluate(BoardStructure board){
	board.countPieces();
	int materialScore = 200 * (board.pieceCount[W_KING] - board.pieceCount[B_KING])
		+ 9 * (board.pieceCount[W_QUEEN] - board.pieceCount[B_QUEEN])
		+ 5 * (board.pieceCount[W_ROOK] - board.pieceCount[B_ROOK])
		+ 3 * (board.pieceCount[W_BISHOP] - board.pieceCount[B_BISHOP] + board.pieceCount[W_KNIGHT] - board.pieceCount[B_KNIGHT])
		+ 1 * (board.pieceCount[W_PAWN] - board.pieceCount[B_PAWN]);
	return (board.sideToMove == WHITE) ? materialScore : -materialScore;
}