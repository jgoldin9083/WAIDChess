/*
 * ComputerPlayer.h
 *
 *  Created on: Nov 16, 2014
 *      Author: jonathan
 */

#ifndef COMPUTERPLAYER_H_
#define COMPUTERPLAYER_H_

#include <algorithm>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>

#include "util.h"
#include "Board.h"
#include "Node.h"
#include "MoveGenerator.h"
#include "HashTable.h"
#include "OpeningBook.h"

#include "PrintUtil.h"

#define MAX_LEVELS 25

#define LAZY_EPSILON 150
#define INIT_WINDOW 10

#define TEMPO 100
#define MATE 10000
#define DRAW 0

#define WEIGHT 12

class ComputerPlayer {
public:
	MOVE getMove(Board* b);
	long evaluate(MoveGenerator* mg,long alpha,long beta);
	long alphaBeta(Node* n,long alpha,long beta, MOVE* l);
	long quiesce(Node* n, long alpha,long beta);
	ComputerPlayer(int minLevel,int seconds);
	virtual ~ComputerPlayer();
	bool timesup;
private:
	bool hitMinL;
	bool inBook;
	int timeLimit;
	int difficulty;
	int currentDepth;
	long endValue;
	HashTable* table;
	long killHash[MAX_LEVELS][4096];
	void clearHash();
	Node nodesArray[MAX_LEVELS];
	static unsigned int valueOf(PIECE p,SQUARE s);
	MOVE getMoveFromBook(Node* n);
	std::string getLine(MOVE* line, int moveCount);
	MOVE pv[MAX_LEVELS];
};

struct MoveWrapper{
	long key;
	MOVE m;
};

#endif /* COMPUTERPLAYER_H_ */
