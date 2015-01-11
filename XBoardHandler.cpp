/*
 * XBoardHandler.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: jonathan
 */

#include "XBoardHandler.h"

using namespace std;

XBoardHandler::XBoardHandler() {
}

void XBoardHandler::runXBoard() {

	setbuf (stdout, NULL);
	setbuf (stdin, NULL);

	for(;;){
		fill(input, input + STR_BUFF, 0);
      	fgets (input, STR_BUFF, stdin);
      	if(!strcmp(input,"xboard")){
			handleXBOARD(input);
		} else
		if(!strncmp(input,"protover",8)){
			handlePROTOVER(input);
		} else
		if(!strncmp(input,"new",3)){
			handleNEW(input);
		} else
		if(!strncmp(input,"level",5)){
			handleLEVEL(input);
		} else
		if(!strncmp(input,"st",2)){
			handleST(input);
		} else
		if(!strncmp(input,"nopost",6)){
			handleNOPOST(input);
		} else
		if(!strncmp(input,"easy",4)){
			handleEASY(input);
		} else
      	if(!strncmp(input,"time",4)){
      		handleTIME(input);
      	} else
      	if(!strncmp(input,"otim",4)){
      		handleOTIM(input);
      	} else
		if(!strncmp(input,"quit",4)){
			handleQUIT(input);
			return;
		} else
		if (!strncmp (input, "ping", 4)) {
			handlePING(input);
		} else
		if(!strncmp(input, "force", 5)){
			handleFORCE(input);
		} else
		if(!strncmp(input, "go",2)){
			handleGO(input);
		} else
		if(com_ismove(input)){
			handleMOVE(input);
		} else {
		}
	}
}

XBoardHandler::~XBoardHandler() {
	// TODO Auto-generated destructor stub
}

int XBoardHandler::com_ismove(char* command) {
	return (command[0] >= 'a' && command[0] <= 'h' &&
	            command[1] >= '1' && command[1] <= '8' &&
	            command[2] >= 'a' && command[2] <= 'h' &&
	            command[3] >= '1' && command[3] <= '8' &&
	            ( command[4] == ' ' || command[4] == '\n' || command[4] == 0 ||
	              command[4] == '-' ||
	              command[4] == 'q' || command[4] == 'r' || command[4] == 'b' || command[4] == 'n'));
}

void XBoardHandler::handleXBOARD(char* input) {
	printf("\n");
}

void XBoardHandler::handlePROTOVER(char* input) {
	printf("feature colors=0\n");
	printf("feature time=0\n");
	printf("feature sigterm=0\n");
	printf("feature sigint=0\n");
	printf("feature done=1\n");
}

void XBoardHandler::handleNEW(char* input) {
	whiteTimeUsed = 0;
	blackTimeUsed = 0;
	humanTimeUsed = &whiteTimeUsed;
	computerTimeUsed = &blackTimeUsed;
	b = new Board();
	mg = new MoveGenerator(b);
	cp = new ComputerPlayer(LEVEL,TIME);
}

void XBoardHandler::handleLEVEL(char* input) {

}

void XBoardHandler::handleST(char* input) {
	timer = NSPM;
	sscanf(input,"%d",&secsPerMove);
//	cp1->timeLimit = secsPerMove;
}

void XBoardHandler::handleNOPOST(char* input) {
}

void XBoardHandler::handleEASY(char* input) {
}

void XBoardHandler::handleTIME(char* input) {
	int centisec;
	sscanf(input+5,"%d",&centisec);
	*computerTimeUsed = centisec/100.0;
	printf("time used Computer: %f\n",*computerTimeUsed);
}

void XBoardHandler::handleOTIM(char* input) {
	int centisec;
	sscanf(input+5,"%d",&centisec);
	*humanTimeUsed = centisec/100.0;
	printf("time used Human: %f\n",*humanTimeUsed);
}

void XBoardHandler::handleQUIT(char* input) {
}

void XBoardHandler::handlePING(char* input) {
	int pingnum;
	sscanf (input+5, "%d", &pingnum);
	printf("pong %d\n", pingnum);
}

void XBoardHandler::handleGO(char* input) {
	forceMode = false;
	if(b->getTurn() == WHITE){
		computerTimeUsed = &whiteTimeUsed;
		humanTimeUsed = &blackTimeUsed;
	} else {
		computerTimeUsed = &blackTimeUsed;
		humanTimeUsed = &whiteTimeUsed;
	}
	getMove();
	checkForGameEnd();
	fill(output, output + STR_BUFF, 0);

}
void XBoardHandler::handleFORCE(char* input){
	forceMode = true;
}

void XBoardHandler::handleMOVE(char* input) {
	MOVE m = move(square((input[0]-'a'),(input[1]-'1')),square((input[2]-'a'),(input[3]-'1')),0);
	m = mg->getMove(m);
	if(m == 0){
		printf("Illegal move: %s\n",input);
	} else {
		Board* testB = new Board(b);
		MoveGenerator* mg = new MoveGenerator(testB);
		testB->makeMove(m);
		if(mg->inCheck(!mg->b->getTurn())){
			delete mg;
			printf("Illegal move: %s\n",moveString(m).c_str());
		} else {
			delete mg;
			b->makeMove(m);
			if(!forceMode){getMove();}
			checkForGameEnd();
		}
	}
}

void XBoardHandler::getMove() {
	MOVE m = cp->getMove(new Board(b));

		switch(getFlag(m)){
		case MFMPQ: case MFTPQ:
			output[4] = 'q';
			break;
		case MFMPR: case MFTPR:
			output[4] = 'r';
			break;
		case MFMPB: case MFTPB:
			output[4] = 'b';
			break;
		case MFMPN: case MFTPN:
			output[4] = 'n';
			break;
		}
		b->makeMove(m);
		output[0] = (col(from(m))) + 'a';
		output[1] = (row(from(m))) + '1';
		output[2] = (col(to(m))) + 'a';
		output[3] = (row(to(m))) + '1';
		printf("move %s\n",output);
		fill(output, output + STR_BUFF, 0);
		checkForGameEnd();
}

void XBoardHandler::checkForGameEnd() {

	int gameEnd = mg->gameOver();
	if(gameEnd == 0){
		printf("0-1 {black wins}\n");
	} else if(gameEnd == 1){
		printf("1/2-1/2 {draw}\n");
	} else if(gameEnd == 2){
		printf("1-0 {white wins}\n");
	}
}
