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

void PVTable::clearPVTable() {
	//Set all variables of PVEntities in pvTable to 0 values.
	for (int i = 0; i < numOfEntries; i++) {
		pvTable[i].positionID = 0ULL;
		pvTable[i].move = 0;
	}
}

void PVTable::storePVMove(BoardStructure board, int move) {
	//Store a move and position ID in the PV table
	int i = board.generateAndGetPositionID() % numOfEntries;
	//int i = 0;
	pvTable[i].move = move;
	pvTable[i].positionID = board.generateAndGetPositionID();

	cout << "Storing move "; 
	movelist.uciPrintMoveGivenMoveInt(board, move);
	cout << " in pvTable[" << i << "]\n";
	//cout << "Storing positionID " << board.generateAndGetPositionID() << " in pvTable[" << i << "]\n";
	///cout << "pvTable[" << i << "].move is now " << pvTable[i].move << "\n";
	///cout << "pvTable[" << i << "].positionID is now " << pvTable[i].positionID << "\n";
}

int PVTable::getPVMove(BoardStructure board) {

	//Get the principal variation move
	int i = board.generateAndGetPositionID() % numOfEntries;
	//int i = 0;
	///cout << "board.positionID:" << board.generateAndGetPositionID() << " numOfEntries: " << numOfEntries << "\n";
	///cout << "pvTable[i].positionID is " << pvTable[i].positionID << " while board.positionID is " << board.generateAndGetPositionID() << "\n";
	if (pvTable[i].positionID == board.generateAndGetPositionID()) {
		///cout << "Getting pvTable[" << i << "].move as " << pvTable[i].move << "\n";
		//cout << "pvTable[i].move: " << pvTable[i].move <<  "\n";
		return pvTable[i].move;
	}
	return 0;
}


int PVTable::getPVLine(BoardStructure board, int d)  {
	Move move;
	move.move = getPVMove(board);
	int count = 0;
	//cout << "d: " << d << "\n";
	while (move.move != 0 && count < d) {
		//If the move returned by getPVMove exists and is valid, make the move, add it to pvArray, and increment count
	///cout << "Yo\n";
	///cout << "move is"; movelist.uciPrintMoveGivenMove(board,move);
		if (movelist.isMoveValid(board, move.move)) {
			//cout << "count:" << count << "\n";
			board.makeMove(move);
			cout << "Adding move";
			movelist.uciPrintMoveGivenMove(board, move);
			pvArray[count++] = move.move;	

			///cout << "board.pvArray[0]:" << board.pvArray[0] << "\n";
			///cout << "board.pvArray[1]:" << board.pvArray[1] << "\n";
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

///	cout << "L board.pvArray[0]:" << board.pvArray[0] << "\n";
	///cout << "L board.pvArray[1]:" << board.pvArray[1] << "\n";

	return count;
}

