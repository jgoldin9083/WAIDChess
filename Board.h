/*
 * Board.h
 *
 *  Created on: Nov 16, 2014
 *      Author: jonathan
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "util.h"
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <omp.h>



class Board {
public:
	SQUARE epTarget;
	uint8_t metadata;
	uint8_t drawCount;
	SQUARE kings[2];
	PIECE board[64];

	PIECE pieceAt(SQUARE s);
	void putPiece(SQUARE s, PIECE p);
	void makeMove(MOVE m);
	COLOR getTurn();
	void setBoard(Board* b);
	Board();
	Board(Board* b);
	virtual ~Board();
	std::string toFEN();
	bool checkForRepetitions();
	bool hasMatingMaterial();
	std::vector<MOVE> moves;
};

#endif /* BOARD_H_ */
