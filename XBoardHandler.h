/*
 * XBoardHandler.h
 *
 *  Created on: Nov 17, 2014
 *      Author: jonathan
 */

#ifndef XBOARDHANDLER_H_
#define XBOARDHANDLER_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/time.h>

#include "Board.h"
#include "MoveGenerator.h"
#include "ComputerPlayer.h"

#define LEVEL 7
#define TIME 20

#define STR_BUFF 32

enum time_control{
		CONV,ICS,NSPM
};

class XBoardHandler {
public:
	Board* b;
	MoveGenerator* mg;
	ComputerPlayer* cp;
	char input[STR_BUFF];
	char output[STR_BUFF];

	void runXBoard();
	XBoardHandler();
	virtual ~XBoardHandler();
private:
	int com_ismove(char* command);
	void handleXBOARD(char* input);
	void handlePROTOVER(char* input);
	void handleNEW(char* input);
	void handleLEVEL(char* input);
	void handleST(char* input);
	void handleNOPOST(char* input);
	void handleEASY(char* input);
	void handleTIME(char* input);
	void handleOTIM(char* input);
	void handleQUIT(char* input);
	void handlePING(char* input);
	void handleGO(char* input);
	void handleMOVE(char* input);
	void handleFORCE(char* input);
	void getMove();
	void checkForGameEnd();
	int totalTime;
	int movesPerTime;
	int base;
	int increment;
	int secsPerMove;
	bool forceMode;
	time_control timer;

	double* humanTimeUsed;
	double* computerTimeUsed;

	double whiteTimeUsed;
	double blackTimeUsed;
	timeval start;
	timeval end;
};

#endif /* XBOARDHANDLER_H_ */
