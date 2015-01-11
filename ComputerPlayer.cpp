/*
 * ComputerPlayer.cpp
 *
 *  Created on: Nov 16, 2014
 *      Author: jonathan
 */

#include "ComputerPlayer.h"

using namespace std;

struct timeStruct{
	ComputerPlayer* cp;
	int seconds;
};
struct Comparator {
    bool operator()(const MoveWrapper& struct1, const MoveWrapper& struct2) const {
		if(struct1.key == struct2.key){
			return (getFlag(struct1.m) > getFlag(struct2.m));
		}
		return (struct1.key > struct2.key);
    }
};

void* timerThread(void* params){
	timeStruct* t = (timeStruct*)params;
	int usec = t->seconds * 1000000;
	usleep(usec);
	t->cp->timesup = true;
	pthread_exit(NULL);
}

ComputerPlayer::ComputerPlayer(int minLevel,int seconds) {
	inBook = false;
	difficulty = min(minLevel,MAX_LEVELS);
	timeLimit = seconds;
	endValue = 0;
	currentDepth = 0;
	timesup = false;
	hitMinL = false;
	table = new HashTable(1000000);
}

ComputerPlayer::~ComputerPlayer() {
	delete table;
}
MOVE ComputerPlayer::getMove(Board* b){
	fill(pv,pv+MAX_LEVELS,0);
	Node::resetCount();
	clearHash();
	timesup = false;
	hitMinL = false;
	timeStruct* t = new timeStruct();
	t->cp = this;
	t->seconds = timeLimit;
	pthread_t timer;
	pthread_create(&timer,NULL,timerThread,(void*)t);
	pthread_detach(timer);
	currentDepth = 0;
	timeval lastFinish;
	timeval curFinish;
	gettimeofday(&lastFinish,NULL);
	MOVE bestMove = 0;
	curFinish = lastFinish;
	double curTime = 0;
	double totTime = 0;
	Node* root = new Node(b,0,0);
	long lastVal = evaluate(root->mg,-MATE,MATE);
	long tmp = lastVal;
	long window[] = {INIT_WINDOW,-INIT_WINDOW};
	while(!(hitMinL && timesup) && currentDepth < MAX_LEVELS){
		if(tmp >= lastVal+window[0]){
//			printf("failed High\n");
			window[0] *= 2;
		} else if(tmp <= lastVal+window[1]){
//			printf("failed Low\n");
			window[1] *= 2;
		} else {
			currentDepth++;
			lastVal = tmp;
			window[0] = INIT_WINDOW;
			window[1] = -INIT_WINDOW;
		}
		root->bestMove = 0;
		MOVE line[currentDepth];
		tmp = alphaBeta(root,lastVal+window[1],lastVal+window[0],line);
		lastFinish = curFinish;
		gettimeofday(&curFinish,NULL);
		curTime = curFinish.tv_sec - lastFinish.tv_sec + (double)(curFinish.tv_usec-lastFinish.tv_usec)/1000000;
		totTime += curTime;
		if(timeLimit-totTime < curTime){
			timesup = true;
		}
		if(tmp < lastVal+window[0] && tmp > lastVal+window[1]){
			for(int i = 0; i < currentDepth; i++){
				pv[i] = line[i];
			}
			if(!(hitMinL && timesup)){
				bestMove = root->bestMove;
				killHash[0][removeFlag(bestMove)]++;
				if(currentDepth == difficulty){hitMinL = true;}
//				printf("%d %ld %d %d %s\n",currentDepth,tmp-TEMPO,(int)(100*totTime),Node::count,getLine(pv,currentDepth).c_str());
				printf("depth %d: %s value: %ld\n",currentDepth,moveString(bestMove).c_str(),tmp);
				printf("line: %s\n",getLine(pv,currentDepth).c_str());
				printf("time %f: Nodes: %d; nps: %d\n\n",totTime,Node::count,(int)(Node::count/totTime));
				if(tmp <= -MATE || tmp >= MATE){
					printf("Mate in %d\n",currentDepth/2);
					break;
				}
			}
			endValue = tmp;
		}
	}
	if(timesup){currentDepth--;}
	delete root;
	delete t;
	pthread_cancel(timer);
	return bestMove;
}


int val[] = {1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,1,1,2,4,4,4,4,2,1,1,2,4,8,8,4,2,1,1,2,4,8,8,4,2,1,1,2,4,4,4,4,2,1,1,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1};


long ComputerPlayer::evaluate(MoveGenerator* mg,long alpha, long beta) {
	long pieces[] = {0,0};
	int pawncount[][2] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
//	int piececount[][2]  = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
	PIECE pieceofsquare;
	COLOR colorofpiece;
	COLOR colorofowner;
	COLOR turn = mg->b->getTurn();
	for(int i = 0; i < 64; i++){
		pieceofsquare = mg->b->pieceAt(i);
		colorofpiece = colorOf(pieceofsquare);
		if(pieceofsquare == BPAWN-colorofpiece){
			pawncount[col(i)][colorofpiece]++;
		}
//		piececount[pieceofsquare/2+pieceofsquare%2-1][colorofpiece]++;
		if(pieceofsquare!=EMPTY){
			pieces[colorofpiece] += valueOf(pieceofsquare,i);
		}
	}

	pieces[turn]+=TEMPO;
	if((pieces[turn]-pieces[!turn]) < alpha-LAZY_EPSILON || (pieces[turn]-pieces[!turn] > beta+LAZY_EPSILON)){
		return pieces[turn]-pieces[!turn];
	}
//doubled pawns
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 8; j++){
			if(pawncount[j][i] > 1){
				pieces[i]-=50;
			}
		}
	}
////////////////
//isolated pawns
	for(int i = 0; i < 2; i++){
		if(pawncount[0][i] == 1 && !pawncount[1][i]){
			pieces[i]-=50;
		}
		for(int j = 1; j < 7; j++){
			if(pawncount[j][i] == 1 && (!pawncount[j-1][i] && !pawncount[j+1][i])){
				pieces[i]-=50;
			}
		}
		if(pawncount[7][i] == 1 && !pawncount[6][i]){
			pieces[i]-=50;
		}
	}
/////////////////
	if((pieces[turn]-pieces[!turn]) < alpha-LAZY_EPSILON || (pieces[turn]-pieces[!turn] > beta+LAZY_EPSILON)){
		return pieces[turn]-pieces[!turn];
	}

	mg->generateListeners();
	long territory[] = {0,0};
	for(int i = 0; i < 64; i++){
		pieceofsquare = mg->b->pieceAt(i);
		colorofpiece = colorOf(pieceofsquare);
		colorofowner = (mg->listenerCount[i] > 0);
		if(mg->listenerCount[i] != 0){
			territory[colorofowner] += val[i];
			if(colorofpiece != colorofowner && colorofowner == turn){
				pieces[colorofpiece] -= (valueOf(pieceofsquare,i)>>1);
			}
		}
	}
	return ((territory[turn]-territory[!turn])*(WEIGHT))+(pieces[turn]-pieces[!turn]);
}
int pawnValue[]   = {  0,  5,  5,  0,  5, 10, 50,  0,  0, 10, -5,  0,  5, 10, 50,  0,  0, 10,-10,  0, 10, 20, 50,  0,  0,-20,  0, 20, 25, 30, 50,  0,  0,-20,  0, 20, 25, 30, 50,  0,  0, 10,-10,  0, 10, 20, 50,  0,  0, 10, -5,  0,  5, 10, 50,  0,  0,  5,  5,  0,  5, 10, 50,  0};
int rookValue[]   = {  0, -5, -5, -5, -5, -5,  5,  0,  0,  0,  0,  0,  0,  0, 10,  0,  0,  0,  0,  0,  0,  0, 10,  0,  5,  0,  0,  0,  0,  0, 10,  0,  5,  0,  0,  0,  0,  0, 10,  0,  0,  0,  0,  0,  0,  0, 10,  0,  0,  0,  0,  0,  0,  0, 10,  0,  0, -5, -5, -5, -5, -5,  5,  0};
int queenValue[]  = {-20,-10,-10,  0, -5,-10,-10,-20,-10,  0,  5,  0,  0,  0,  0,-10,-10,  5,  5,  5,  5,  5,  0,-10, -5,  0,  5,  5,  5,  5,  0, -5, -5,  0,  5,  5,  5,  5,  0, -5,-10,  0,  5,  5,  5,  5,  0,-10,-10,  0,  0,  0,  0,  0,  0,-10,-20,-10,-10, -5, -5,-10,-10,-20};
int knightValue[] = {-50,-40,-30,-30,-30,-30,-40,-50,-40,-20,  5,  0,  5,  0,-20,-40,-30,  0, 10, 15, 15, 10,  0,-30,-30,  5, 15, 20, 20, 15,  0,-30,-30,  5, 15, 20, 20, 15,  0,-30,-30,  0, 10, 15, 15, 10,  0,-30,-40,-20,  5,  0,  5,  0,-20,-40,-50,-40,-30,-30,-30,-30,-40,-50};
int bishopValue[] = {-20,-10,-10,-10,-10,-10,-10,-20,-10,  5, 10,  0,  5,  0,  0,-10,-10,  0, 10, 10,  5,  5,  0,-10,-10,  0, 10, 10, 10, 10,  0,-10,-10,  0, 10, 10, 10, 10,  0,-10,-10,  0, 10, 10,  5,  5,  0,-10,-10,  5, 10,  0,  5,  0,  0,-10,-20,-10,-10,-10,-10,-10,-10,-20};
int kingValue[] =   { 20, 20,-10,-20,-30,-30,-30,-30, 30, 20,-20,-30,-40,-40,-40,-40, 10,  0,-20,-30,-40,-40,-40,-40,  0,  0,-20,-40,-50,-50,-50,-50,  0,  0,-20,-40,-50,-50,-50,-50, 10,  0,-20,-30,-40,-40,-40,-40, 30, 20,-20,-30,-40,-40,-40,-40, 20, 20,-10,-20,-30,-30,-30,-30};

unsigned int ComputerPlayer::valueOf(PIECE p, SQUARE s) {
	switch(p){
	case WKING: return (100+kingValue[s]);
	case BKING: return (100+kingValue[square(col(s),7-row(s))]);
	case WQUEEN: return (900+queenValue[s]);
	case BQUEEN: return (900+queenValue[square(col(s),(7-row(s)))]);
	case WBISHOP: return (330+bishopValue[s]);
	case BBISHOP: return (330+bishopValue[square(col(s),(7-row(s)))]);
	case WKNIGHT: return (320+knightValue[s]);
	case BKNIGHT: return (320+knightValue[square(col(s),(7-row(s)))]);
	case WROOK: return (500+rookValue[s]);
	case BROOK: return (500+rookValue[square(col(s),(7-row(s)))]);
	case WPAWN: return (100+pawnValue[s]);
	case BPAWN: return (100+pawnValue[square(col(s),(7-row(s)))]);
	}
	return 0;
}

long ComputerPlayer::alphaBeta(Node* n, long alpha, long beta,MOVE* l) {
	if(timesup && hitMinL){
		return 0;
	}
	if(!n->mg->b->hasMatingMaterial()){return DRAW;}
	if(n->mg->b->checkForRepetitions()){return DRAW;}
	HashEntry* he = NULL;
//	if(n->depth != 0){
		he = table->retrieve(n->mg->b);
		if(he != NULL){
			if(he->depth >= currentDepth - n->depth){
				switch(he->nodeType){
				case EXACT:
					n->bestMove = he->bestMove;
					l[n->depth] = n->bestMove;
					l[n->depth+1] = 0;
					return he->value;
					break;
				case LOWERBOUND:
					alpha = max(alpha,he->value);
					break;
				case UPPERBOUND:
					beta = min(beta,he->value);
					break;
				}
				if(beta <= alpha){
					n->bestMove = he->bestMove;
					l[n->depth] = n->bestMove;
					l[n->depth+1] = 0;
					return he->value;
				}
			}
		}
//	}
	if(n->depth == currentDepth){
		return evaluate(n->mg,alpha,beta);
//		return quiesce(n,alpha,beta);
	}
	n->mg->generateMoves();
/*
	if(inBook && n->depth == 0){
		vector<MOVE> moves;
		OpeningBook::getMoves(n->mg->b->toFEN(),&moves);
		if(moves.size() == 0){
			inBook = false;
		} else {
			for(unsigned int i = 0; i < moves.size(); i++){
				moves[i] = n->mg->getMove(moves[i]);
			}
			n->mg->moves.clear();
			for(unsigned int i = 0; i < moves.size(); i++){
				n->mg->moves.push_back(moves[i]);
			}
		}
	}
	if(n->mg->moves.size() == 1){
		n->bestMove = n->mg->moves[0];
		return evaluate(n->mg);
	}
*/


	int moveCount = n->mg->moves.size();
	vector<MoveWrapper> allMoves;
	allMoves.reserve(moveCount);
	for(int i = 0; i < moveCount; i++){
		MoveWrapper mw;
		mw.m = n->mg->moves[i];
		if(n->mg->moves[i] == pv[n->depth]){
			mw.key = INT_MAX-1;
		} else if(he != NULL && mw.m == he->bestMove){
			mw.key = INT_MAX-2;
		} else {
			mw.key = killHash[n->depth][removeFlag(mw.m)];
		}
		allMoves.push_back(mw);
	}
	sort(allMoves.begin(),allMoves.end(),Comparator());
	double best = -MATE-1;
	Node* newNode = &nodesArray[n->depth];
	NODETYPE nt = UPPERBOUND;
	bool hasMove = false;
	MOVE line[currentDepth];
	for(int i = 0; i < moveCount; i++){
		newNode->setBoard(n->mg->b,n->depth+1,allMoves[i].m);
		if(!newNode->mg->inCheck(n->mg->b->getTurn())){
			hasMove = true;
			long tmp = -alphaBeta(newNode,-beta,-alpha,line);
			if(tmp > best){
				n->bestMove = newNode->m;
				best = tmp;
				l[n->depth] = newNode->m;
				for(int pv = currentDepth-1; pv > n->depth; pv--){
					l[pv] = line[pv];
				}
			}
			if(tmp >= beta){
				killHash[n->depth][removeFlag(newNode->m)]++;
				table->insertEntry(n,currentDepth,beta,LOWERBOUND);
				return beta;
			}
			if(tmp > alpha){
				nt = EXACT;
				alpha = tmp;
			}
		}
	}
	if(!hasMove){
		if(!n->mg->inCheck(n->mg->b->getTurn())){alpha = DRAW;}
		else {alpha = -MATE;}
	}
	table->insertEntry(n,currentDepth,alpha,nt);
	return alpha;
}

void ComputerPlayer::clearHash() {
	for(int i = 0; i < 4096; i++){
		for(int j = 0; j < MAX_LEVELS; j++){
			killHash[j][i] = 0;
		}
	}
}
MOVE ComputerPlayer::getMoveFromBook(Node* n) {
	vector<MOVE> moves;
	OpeningBook::getMoves(n->mg->b->toFEN(),&moves);
	if(moves.empty()){return 0;}
	srand (time(NULL));
	n->mg->generateMoves();
	for(unsigned int i = 0; i < moves.size(); i++){
		moves[i] = n->mg->getMove(moves[i]);
	}
	return moves[rand()%moves.size()];
}

string ComputerPlayer::getLine(MOVE* line,int moveCount) {
	string s = "";
	for(int i = 0; i < moveCount; i++){
		if(line[i] == 0){
			s += "HT";
			break;
		}
		s += moveString(line[i]);
		s += " ";
	}
	return s;
}

long ComputerPlayer::quiesce(Node* n, long alpha, long beta) {
	long stand_pat = evaluate(n->mg,alpha,beta);
	if(stand_pat >= beta){return beta;}
	if(alpha < stand_pat){alpha = stand_pat;}
	if(n->depth >= MAX_LEVELS){return stand_pat;}

	n->mg->generateMoves();
	Node* newNode = &nodesArray[n->depth];

	for(unsigned int i = 0; i < n->mg->moves.size(); i++){
		int flag = getFlag(n->mg->moves[i]);
		if(flag == MFTAK || flag == MFTPQ || flag == MFTPR || flag == MFTPB || flag == MFTPN){
			newNode->setBoard(n->mg->b,n->depth+1,n->mg->moves[i]);
			if(!newNode->mg->inCheck(n->mg->b->getTurn())){
//				long score = -quiesce(newNode,-beta,-alpha);
				long score = -evaluate(newNode->mg,alpha,beta);
				if(score >= beta){
					return beta;
				}
				if(score > alpha){
					alpha = score;
				}
			}
		}
	}
	return alpha;
}
