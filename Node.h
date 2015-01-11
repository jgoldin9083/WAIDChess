/*
 * Node.h
 *
 *  Created on: Nov 16, 2014
 *      Author: jonathan
 */

#ifndef NODE_H_
#define NODE_H_

#include "Board.h"
#include "MoveGenerator.h"
#include "util.h"

class Node {
public:
	static int count;
	static void resetCount();
	MoveGenerator* mg;
	int depth;
	MOVE m;
	MOVE bestMove;
	void setBoard(Board* b,int depth,MOVE m);
	Node();
	Node(Board* board, int d, MOVE move);
	virtual ~Node();
};

#endif /* NODE_H_ */
