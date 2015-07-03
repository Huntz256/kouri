// Principal variation table.
//"The Principal variation (PV) is a sequence of moves that programs consider best 
// and therefore expect to be played."
//https://chessprogramming.wikispaces.com/Principal+variation

#include "defs.h"
#include <iostream>
using namespace std;

void PVTable::initPVTable() {
	//The number of entries in the table
	numOfEntries = 2048;

	//Set everything in pvTable to zero
	clearPVTable();
}

//Set all variables of PVEntities in pvTable to 0 values.
void PVTable::clearPVTable() {
	for (int i = 0; i < numOfEntries; i++) {
		pvTable[i].positionID = 0ULL;
		pvTable[i].move = 0;
	}
}

//Store a move and position ID in the PV table
void PVTable::storePVMove(BoardStructure board, int move) {
	int i = board.generateAndGetPositionID() % numOfEntries;
	pvTable[i].move = move;
	pvTable[i].positionID = board.generateAndGetPositionID();
}

//Get the principal variation move
int PVTable::getPVMove(BoardStructure board) {
	int i = board.generateAndGetPositionID() % numOfEntries;

	if (pvTable[i].positionID == board.generateAndGetPositionID()) {
		return pvTable[i].move;
	}

	return 0;
}


int PVTable::getPVLine(BoardStructure board, int d)  {
	Move move; move.move = getPVMove(board);
	int count = 0;

	while (move.move != 0 && count < d) {
		//If the move returned by getPVMove exists and is valid, make the move, add it to pvArray, and increment count
		if (movelist.isMoveValid(board, move.move)) {
			board.makeMove(move);
			pvArray[count++] = move.move;
		}

		//If illegal move, stop 
		else {
			break;
		}

		//Get another move from getPVMove
		move.move = getPVMove(board);
	}

	//Return to org. position
	while (board.ply > 0) {
		board.undoMove();
	}

	return count;
}

