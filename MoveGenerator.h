/*
 * MoveGenerator.h
 *
 *  Created on: Nov 16, 2014
 *      Author: jonathan
 */

#ifndef MOVEGENERATOR_H_
#define MOVEGENERATOR_H_

#include <vector>
#include <cmath>
#include "Board.h"

#include <stdio.h>

class MoveGenerator {
public:
	Board* b;
	std::vector<MOVE> moves;
	int listenerCount[64];
	void generateMoves();
	void generateListeners();
	MoveGenerator(Board* board);
	virtual ~MoveGenerator();
	MOVE getMove(MOVE m);
	bool squareThreatenedBy(SQUARE square, COLOR c);
	bool inCheck(COLOR c);
	int gameOver();
private:
	void genKingAt(SQUARE square,bool lis);
	void genQueenAt(SQUARE square,bool list);
	void genBishopAt(SQUARE square,bool list);
	void genKnightAt(SQUARE square,bool list);
	void genRookAt(SQUARE square,bool list);
	void genPawnAt(SQUARE square,bool list);
	void addMove(MOVE m);
	void addListener(SQUARE origin,SQUARE target);
	static uint8_t Scount[];
	static uint8_t Ncount[];
	static uint8_t Wcount[];
	static uint8_t Ecount[];
	static uint8_t SWcount[];
	static uint8_t SEcount[];
	static uint8_t NWcount[];
	static uint8_t NEcount[];
};

#endif /* MOVEGENERATOR_H_ */
