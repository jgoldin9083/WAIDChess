/*
 * main.cpp
 *
 *  Created on: Nov 16, 2014
 *      Author: jonathan
 */

#include "Board.h"
#include "Node.h"
#include "MoveGenerator.h"
#include "ComputerPlayer.h"
#include "PrintUtil.h"
#include "XBoardHandler.h"
#include "OpeningBook.h"
#include <sys/time.h>
#include <stdio.h>
#include <vector>

#define TIMETEST
//#define VS
//#define XBOARD

int main(){
#ifdef TIMETEST
	timeval start;
	timeval end;
	Board* b = new Board();
	MoveGenerator* mg = new MoveGenerator(b);
	ComputerPlayer* cp = new ComputerPlayer(7,20);
	gettimeofday(&start,NULL);
	MOVE m = cp->getMove(new Board(b));
	gettimeofday(&end,NULL);
	printMove(m);
	delete mg;
	delete cp;
	double time = end.tv_sec - start.tv_sec + (double)(end.tv_usec-start.tv_usec)/1000000;
	printf("%fs\n",time);
	printf("%d\n",(int)(Node::count/time));
#endif
#ifdef VS
	ComputerPlayer* cp1 = new ComputerPlayer(7,20,12);
	ComputerPlayer* cp2 = new ComputerPlayer(7,20,9);
	ComputerPlayer* cps[] = {cp1,cp2};
	Board* b = new Board();
	MoveGenerator* mg = new MoveGenerator(b);
	while(mg->gameOver() == -1){
		MOVE m = cps[b->getTurn()]->getMove(new Board(b));
		b->makeMove(m);
		display(b);
		printf("\n");
	}
	if(b->getTurn()){
		printf("BLACK WINS\n");
	} else {
		printf("WHITE WINS\n");
	}

#endif
#ifdef XBOARD
	XBoardHandler* xbh = new XBoardHandler();
	xbh->runXBoard();
#endif
}


