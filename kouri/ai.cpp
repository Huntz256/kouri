#include "defs.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
using namespace std;

MoveListGenerator movechooser;
MoveListGenerator movegen;
int numEvaluatedMoves; int numEvalsPerMove;
string x;

//Returns the best move found by the nega-max function
Move findBestMove(BoardStructure board, int depth){
	movechooser.generateMoveList(board);
	Move bestMove = movechooser.movesLegal[0]; int bestMoveNum = 0;
	numEvaluatedMoves = 0;

	int a = -numeric_limits<int>::max(); //largest negative int possible
	int b = numeric_limits<int>::max(); //largest positive int possible

	for (int i = 0; i < movechooser.numberOfMovesLegal; i++){
		numEvalsPerMove = 0;
		//cout << "findBestMove() calling negaMax(). iteration: " << i << "\n";
		int value = -1 * negaMax(board, depth, a, b); //Positive: from WHITE's perspective, Negative: from BLACK's perspective
		cout << "\nmove #" << i << " final eval: " << value << "\n";
		cout << "num of evals: " << numEvalsPerMove << "\n\n";
		if (value > a){
			a = value;
			bestMove = movechooser.movesLegal[i];
			bestMoveNum = i;
		}
	}
	cout << "\n" << numEvaluatedMoves << " moves evaluated this turn\n";
	cout << "Best move determined to be: " << bestMoveNum << "\n";
	return bestMove;
}

//nega-max with alpha-beta pruning
int negaMax(BoardStructure &board, int depth, int a, int b)
{
	//board.displayBoard();
	//cout << "nega-max called with d: " << depth << "\n";
	if (depth <= 0){
		int e = evaluate(board);
		cout << e << " ";
		numEvaluatedMoves++;
		numEvalsPerMove++;
		//board.displayBoard();
		return e;
	}

	movegen.generateMoveList(board);
	for (int i = 0; i < movegen.numberOfMovesLegal; i++) {
		board.makeMove(movegen.movesLegal[i]);
		a = max(a, -1 * negaMax(board, depth - 1, -b, -a));
		board.undoMove();
		if (b <= a){
			return b;
		}
	}
	return a;
} 

//Evaluates board from WHITE's perspective
int evaluate(BoardStructure &board){
	//bd = *board;
	//board.displayBoard();
	board.countPieces();
	int materialScore = 2000 * (board.pieceCount[W_KING] - board.pieceCount[B_KING])
		+ 90 * (board.pieceCount[W_QUEEN] - board.pieceCount[B_QUEEN])
		+ 50 * (board.pieceCount[W_ROOK] - board.pieceCount[B_ROOK])
		+ 30 * (board.pieceCount[W_BISHOP] - board.pieceCount[B_BISHOP] + board.pieceCount[W_KNIGHT] - board.pieceCount[B_KNIGHT])
		+ 10 * (board.pieceCount[W_PAWN] - board.pieceCount[B_PAWN]);
	//int mobilityScore = 
	
	//return (board.sideToMove == WHITE) ? materialScore : -materialScore;
	return materialScore;
}

//Takes a board and a move, and returns a board with the move applied
BoardStructure applyMove(BoardStructure board, Move m){
	board.makeMove(m);
	return board;
}