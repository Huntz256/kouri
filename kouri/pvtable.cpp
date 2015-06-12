// Principal variation table.
//"The Principal variation (PV) is a sequence of moves that programs consider best 
// and therefore expect to be played."
//https://chessprogramming.wikispaces.com/Principal+variation

#include "defs.h"
#include <iostream>
using namespace std;

const int PV_TABLE_SIZE = 0x100000 * 4; //4 megabytes

void PVTable::initPVTable() {

	//The number of entries in the table is the size of the table divided by the size of each entity
	numOfEntries = PV_TABLE_SIZE / sizeof(PVEntity);

	//Make sure that we don't go over the end
	numOfEntries -= 2;

	//Frees any memory that pvTable is pointing at
	delete pvTable;

	//Allocate memory to pvTable
	pvTable = new PVEntity[numOfEntries * sizeof(PVEntity)];

	//Set everything in pvTable to zero
	clearPVTable();

	cout << "numOfEntries: " << numOfEntries << "\n";
}

void PVTable::clearPVTable() {

	//Set all variables of PVEntities in pvTable to 0 values.
	for (PVEntity *i = pvTable; i < pvTable + numOfEntries; i++) {
		(*i).positionID = 0ULL;
		(*i).move = 0;
	}
}

void PVTable::storePVMove(BoardStructure board, int move) {

	//Store a move and position ID in the PV table
	int i = board.generateAndGetPositionID() % numOfEntries;
	pvTable[i].move = move;
	pvTable[i].positionID = board.generateAndGetPositionID();

	cout << "Storing move " << move << " in pvTable[" << i << "]\n";
	cout << "Storing positionID " << board.generateAndGetPositionID() << " in pvTable[" << i << "]\n";
	cout << "pvTable[" << i << "].move is now " << pvTable[i].move << "\n";
	cout << "pvTable[" << i << "].positionID is now " << pvTable[i].positionID << "\n";
}

int PVTable::getPVMove(BoardStructure board) {

	//Get the principal variation move
	//int i = board.generateAndGetPositionID() % numOfEntries;
	//cout << "board.positionID:" << board.generateAndGetPositionID() << " numOfEntries: " << numOfEntries << "\n";
	//cout << "pvTable[i].positionID is " << pvTable[i].positionID << " while board.positionID is " << board.generateAndGetPositionID() << "\n";
	//if (pvTable[i].positionID == board.generateAndGetPositionID()) {
	//	cout << "Getting pvTable[" << i << "].move as " << pvTable[i].move << "\n";
	//	return pvTable[i].move;
	//}
	return 0;
}

int PVTable::getPVLine(BoardStructure board, int d)  {
	Move move;
	move.move = getPVMove(board);
	int count = 0;
	cout << "d: " << d << "\n";
	while (move.move != 0 && count < d) {
		//If the move returned by getPVMove exists and is valid, make the move, add it to pvArray, and increment count
		if (movelist.isMoveValid(board, move.move)) {
			cout << "count:" << count << "\n";
			board.makeMove(move);
			board.pvArray[count++] = move.move;	

			cout << "board.pvArray[0]:" << board.pvArray[0] << "\n";
				cout << "board.pvArray[1]:" << board.pvArray[1] << "\n";
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

	cout << "BBboard.pvArray[0]:" << board.pvArray[0] << "\n";
	cout << "BBboard.pvArray[1]:" << board.pvArray[1] << "\n";

	return count;
}