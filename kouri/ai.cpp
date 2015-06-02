#include "defs.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
using namespace std;

MoveListGenerator movegen;
int numEvaluatedMoves;
string x;

//Returns the best move found by the nega-max function
Move findBestMove(BoardStructure board, int depth){
	movegen.generateMoveList(board);
	Move bestMove = movegen.movesLegal[0]; int bestMoveNum = 0;
	numEvaluatedMoves = 0;

	int a = -numeric_limits<int>::max(); //largest negative int possible
	int b = numeric_limits<int>::max(); //largest positive int possible
	for (int i = 0; i < movegen.numberOfMovesLegal; i++){
		//cout << "findBestMove() calling negaMax(). iteration: " << i << "\n";
		int value = -1 * negaMax(board, depth, a, b); //Positive: from WHITE's perspective, Negative: from BLACK's perspective
		if (value > a){
			a = value;
			bestMove = movegen.movesLegal[i];
			bestMoveNum = i;
		}
	}
	cout << numEvaluatedMoves << " moves evaluated this turn\n";
	cout << "Best move determined to be: " << bestMoveNum << "\n";
	return bestMove;
}

//nega-max with alpha-beta pruning
int negaMax(BoardStructure &board, int depth, int a, int b)
{
	//board.displayBoard();
	//cout << "nega-max called with d: " << depth << "\n";
	if (depth <= 0){
		cout << "move number " << numEvaluatedMoves << " evaluated to " << evaluate(board) << "\n";
		numEvaluatedMoves++;
		//board.displayBoard();
		return evaluate(board);
	}

	movegen.generateMoveList(board);
	//cout << "depth: " << depth << "   num moves generated: " << movegen.numberOfMovesLegal << "\n\n";
	//getline(cin, x);

	for (int i = 0; i < movegen.numberOfMovesLegal; i++) {
		//cout << "move ID: " << movegen.movesLegal[i].move << "\n";
		board.makeMove(movegen.movesLegal[i]);
		a = max(a, -1 * negaMax(board, depth - 1, -a, -b));
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
	int materialScore = 200 * (board.pieceCount[W_KING] - board.pieceCount[B_KING])
		+ 9 * (board.pieceCount[W_QUEEN] - board.pieceCount[B_QUEEN])
		+ 5 * (board.pieceCount[W_ROOK] - board.pieceCount[B_ROOK])
		+ 3 * (board.pieceCount[W_BISHOP] - board.pieceCount[B_BISHOP] + board.pieceCount[W_KNIGHT] - board.pieceCount[B_KNIGHT])
		+ 1 * (board.pieceCount[W_PAWN] - board.pieceCount[B_PAWN]);
	//int mobilityScore = 
	return (board.sideToMove == WHITE) ? materialScore : -materialScore;
}

//Takes a board and a move, and returns a board with the move applied
BoardStructure applyMove(BoardStructure board, Move m){
	board.makeMove(m);
	return board;
}