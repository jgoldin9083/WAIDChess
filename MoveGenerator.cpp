/*
 * MoveGenerator.cpp
 *
 *  Created on: Nov 16, 2014
 *      Author: jonathan
 */

#include "MoveGenerator.h"

using namespace std;

uint8_t MoveGenerator::Scount[64] =  {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7};
uint8_t MoveGenerator::Ncount[64] =  {7,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0};
uint8_t MoveGenerator::Wcount[64] =  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7};
uint8_t MoveGenerator::Ecount[64] =  {7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0};
uint8_t MoveGenerator::SWcount[64] = {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,2,2,2,2,2,2,0,1,2,3,3,3,3,3,0,1,2,3,4,4,4,4,0,1,2,3,4,5,5,5,0,1,2,3,4,5,6,6,0,1,2,3,4,5,6,7};
uint8_t MoveGenerator::SEcount[64] = {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,6,0,1,2,3,4,5,5,5,0,1,2,3,4,4,4,4,0,1,2,3,3,3,3,3,0,1,2,2,2,2,2,2,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0};
uint8_t MoveGenerator::NWcount[64] = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,2,2,2,2,2,2,1,0,3,3,3,3,3,2,1,0,4,4,4,4,3,2,1,0,5,5,5,4,3,2,1,0,6,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0};
uint8_t MoveGenerator::NEcount[64] = {7,6,5,4,3,2,1,0,6,6,5,4,3,2,1,0,5,5,5,4,3,2,1,0,4,4,4,4,3,2,1,0,3,3,3,3,3,2,1,0,2,2,2,2,2,2,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0};

MoveGenerator::MoveGenerator(Board* board) {
	b = board;
}

void MoveGenerator::generateMoves() {
	COLOR turn = b->getTurn();
	moves.clear();
	moves.reserve(40);
	for(int square = 0; square < 64; square++){
		if(b->pieceAt(square) != EMPTY && colorOf(b->pieceAt(square)) == turn){
			switch(b->pieceAt(square)){
			case WKING:case BKING:
				genKingAt(square,false);break;
			case WQUEEN:case BQUEEN:
				genQueenAt(square,false);break;
			case WBISHOP: case BBISHOP:
				genBishopAt(square,false);break;
			case WKNIGHT:case BKNIGHT:
				genKnightAt(square,false);break;
			case WROOK:case BROOK:
				genRookAt(square,false);break;
			case WPAWN:case BPAWN:
				genPawnAt(square,false);break;
			}
		}
	}
}

void MoveGenerator::generateListeners() {
	for(int i = 0; i < 64; i++){
		listenerCount[i] = 0;
	}
	for(int square = 0; square < 64; square++){
		if(b->pieceAt(square) != EMPTY){
			switch(b->pieceAt(square)){
			case WKING:case BKING:
				genKingAt(square,true);break;
			case WQUEEN:case BQUEEN:
				genQueenAt(square,true);break;
			case WBISHOP: case BBISHOP:
				genBishopAt(square,true);break;
			case WKNIGHT:case BKNIGHT:
				genKnightAt(square,true);break;
			case WROOK:case BROOK:
				genRookAt(square,true);break;
			case WPAWN:case BPAWN:
				genPawnAt(square,true);break;
			}
		}
	}
}

MoveGenerator::~MoveGenerator() {
	delete b;
}

void MoveGenerator::genKingAt(SQUARE square, bool list) {
	SQUARE pos[8];
	int total = 0;
	if(col(square) != 0){ //left
		pos[total++] = (square - 8);
		if(row(square) != 0){pos[total++] = (square - 9);} //down
		if(row(square) != 7){pos[total++] = (square - 7);} //up
	}
	if(col(square) != 7){ //right
		pos[total++] = (square + 8);
		if(row(square) != 0){pos[total++] = (square + 7);} //down
		if(row(square) != 7){pos[total++] = (square + 9);} //up
	}
	if(row(square) != 0){pos[total++] = (square - 1);}//down
	if(row(square) != 7){pos[total++] = (square + 1);}//up
	for(int i = 0; i < total; i++){
		if(!list){
			if(b->pieceAt(pos[i]) != EMPTY){
				if(colorOf(b->pieceAt(pos[i])) != colorOf(b->pieceAt(square))){
					addMove(move(square,pos[i],MFTAK));
				}
			} else {
				addMove(move(square,pos[i],MFMOV));
			}
		} else {
			addListener(square,pos[i]);
		}
	}
	if(!list){
		if(!inCheck(b->getTurn())){
			if(colorOf(b->pieceAt(square)) == WHITE){
				if((b->metadata >> CASTLEWK) & 1){
					SQUARE t = square + 16;
					SQUARE midS = square + 8;
					if(b->pieceAt(midS) == EMPTY && b->pieceAt(t) == EMPTY && !squareThreatenedBy(midS,!b->getTurn())){
						addMove(move(square,t,MFWKC));
					}
				}
				if((b->metadata >> CASTLEWQ) & 1){
					SQUARE t = square - 16;
					SQUARE midS = square - 8;
					SQUARE otherS = square - 24;
					if(b->pieceAt(t) == EMPTY && b->pieceAt(midS) == EMPTY && b->pieceAt(otherS) == EMPTY && !squareThreatenedBy(midS,!b->getTurn())){
						addMove(move(square,t,MFWQC));
					}
				}
			} else {
				if((b->metadata >> CASTLEBK) & 1){
					SQUARE t = square + 16;
					SQUARE midS = square + 8;
					if(b->pieceAt(midS) == EMPTY && b->pieceAt(t) == EMPTY && !squareThreatenedBy(midS,!b->getTurn())){
						addMove(move(square,t,MFBKC));
					}
				}
				if((b->metadata >> CASTLEBQ) & 1){
					SQUARE t = square - 16;
					SQUARE midS = square - 8;
					SQUARE otherS = square - 24;
					if(b->pieceAt(t) == EMPTY && b->pieceAt(midS) == EMPTY && b->pieceAt(otherS) == EMPTY && !squareThreatenedBy(midS,!b->getTurn())){
						addMove(move(square,t,MFBQC));
					}
				}
			}
		}
	}
}

void MoveGenerator::genQueenAt(SQUARE square, bool list) {
	genRookAt(square,list);
	genBishopAt(square,list);
}

void MoveGenerator::genBishopAt(SQUARE square, bool list) {

	uint8_t downLeft = SWcount[square];
	uint8_t upLeft = NWcount[square];
	uint8_t downRight = SEcount[square];
	uint8_t upRight = NEcount[square];

	SQUARE s = square;
	for(int i = 0; i < downLeft; i++){
		s = s - 9;
		if(!list){
			if(b->pieceAt(s) == EMPTY){
				addMove(move(square,s,MFMOV));
			} else if(colorOf(b->pieceAt(s)) != colorOf(b->pieceAt(square))){
				addMove(move(square,s,MFTAK));
			}
		} else {
			addListener(square,s);
		}
		if(b->pieceAt(s) != EMPTY){break;}
	}
	s = square;
	for(int i = 0; i < upLeft; i++){
		s = s - 7;
		if(!list){
			if(b->pieceAt(s) == EMPTY){
				addMove(move(square,s,MFMOV));
			} else if(colorOf(b->pieceAt(s)) != colorOf(b->pieceAt(square))){
				addMove(move(square,s,MFTAK));
			}
		} else {
			addListener(square,s);
		}
		if(b->pieceAt(s) != EMPTY){break;}
	}
	s = square;
	for(int i = 0; i < downRight; i++){
		s = s + 7;
		if(!list){
			if(b->pieceAt(s) == EMPTY){
				addMove(move(square,s,MFMOV));
			} else if(colorOf(b->pieceAt(s)) != colorOf(b->pieceAt(square))){
				addMove(move(square,s,MFTAK));
			}
		} else {
			addListener(square,s);
		}
		if(b->pieceAt(s) != EMPTY){break;}
	}
	s = square;
	for(int i = 0; i < upRight; i++){
		s = s + 9;
		if(!list){
			if(b->pieceAt(s) == EMPTY){
				addMove(move(square,s,MFMOV));
			} else if(colorOf(b->pieceAt(s)) != colorOf(b->pieceAt(square))){
				addMove(move(square,s,MFTAK));
			}
		} else {
			addListener(square,s);
		}
		if(b->pieceAt(s) != EMPTY){break;}
	}

}

void MoveGenerator::genKnightAt(SQUARE square, bool list) {
	SQUARE pos[8];
	int total = 0;
	uint8_t tmpCol = col(square);
	uint8_t tmpRow = row(square);

	if(tmpCol > 1 && tmpRow > 0){pos[total++] = (square - 17);}
	if(tmpCol > 1 && tmpRow < 7){pos[total++] = (square - 15);}
	if(tmpCol > 0 && tmpRow > 1){pos[total++] = (square - 10);}
	if(tmpCol > 0 && tmpRow < 6){pos[total++] = (square - 6);}
	if(tmpCol < 7 && tmpRow > 1){pos[total++] = (square + 6);}
	if(tmpCol < 7 && tmpRow < 6){pos[total++] = (square + 10);}
	if(tmpCol < 6 && tmpRow > 0){pos[total++] = (square + 15);}
	if(tmpCol < 6 && tmpRow < 7){pos[total++] = (square + 17);}
	for(int i = 0; i < total; i++){
		if(list){
			addListener(square,pos[i]);
		} else {
			if(b->pieceAt(pos[i]) == EMPTY){
				addMove(move(square,pos[i],MFMOV));
			} else if(colorOf(b->pieceAt(pos[i])) != colorOf(b->pieceAt(square))){
				addMove(move(square,pos[i],MFTAK));
			}
		}
	}

}

void MoveGenerator::genRookAt(SQUARE square, bool list) {
	uint8_t down = Scount[square];
	uint8_t up = Ncount[square];
	uint8_t left = Wcount[square];
	uint8_t right = Ecount[square];

	SQUARE s = square;
	for(int i = 0; i < down; i++){
		s = s - 1;
		if(!list){
			if(b->pieceAt(s) == EMPTY){
				addMove(move(square,s,MFMOV));
			} else if(colorOf(b->pieceAt(s)) != colorOf(b->pieceAt(square))){
				addMove(move(square,s,MFTAK));
			}
		} else {
			addListener(square,s);
		}		if(b->pieceAt(s) != EMPTY){break;}
	}
	s = square;
	for(int i = 0; i < up; i++){
		s = s + 1;
		if(!list){
			if(b->pieceAt(s) == EMPTY){
				addMove(move(square,s,MFMOV));
			} else if(colorOf(b->pieceAt(s)) != colorOf(b->pieceAt(square))){
				addMove(move(square,s,MFTAK));
			}
		} else {
			addListener(square,s);
		}		if(b->pieceAt(s) != EMPTY){break;}
	}
	s = square;
	for(int i = 0; i < left; i++){
		s = s - 8;
		if(!list){
			if(b->pieceAt(s) == EMPTY){
				addMove(move(square,s,MFMOV));
			} else if(colorOf(b->pieceAt(s)) != colorOf(b->pieceAt(square))){
				addMove(move(square,s,MFTAK));
			}
		} else {
			addListener(square,s);
		}		if(b->pieceAt(s) != EMPTY){break;}
	}
	s = square;
	for(int i = 0; i < right; i++){
		s = s + 8;
		if(!list){
			if(b->pieceAt(s) == EMPTY){
				addMove(move(square,s,MFMOV));
			} else if(colorOf(b->pieceAt(s)) != colorOf(b->pieceAt(square))){
				addMove(move(square,s,MFTAK));
			}
		} else {
			addListener(square,s);
		}		if(b->pieceAt(s) != EMPTY){break;}
	}
}

void MoveGenerator::genPawnAt(SQUARE square, bool list) {
	int inc = 0;
	if(b->pieceAt(square) == BPAWN){
		inc = -1;
	} else {
		inc = 1;
	}
	if(list){
		if(col(square) != 0){addListener(square,square - 8 + inc);}
		if(b->pieceAt(square+inc) == EMPTY){addListener(square,square + inc);}
		if(col(square) != 7){addListener(square,square + 8 + inc);}
		if(row(square) == 1 && inc ==  1 && b->pieceAt(square+inc) == EMPTY){addListener(square,square + 2*inc);}
		if(row(square) == 6 && inc == -1 && b->pieceAt(square+inc) == EMPTY){addListener(square,square + 2*inc);}
	} else {
		if((inc == 1 && row(square) == 6) || (inc == -1 && row(square) == 1)){
			if(b->pieceAt(square+inc) == EMPTY){
				addMove(move(square,(square+inc),MFMPQ));
				addMove(move(square,(square+inc),MFMPR));
				addMove(move(square,(square+inc),MFMPB));
				addMove(move(square,(square+inc),MFMPN));
			}
			if(col(square) != 0 && b->pieceAt(square-8+inc) != EMPTY && colorOf(b->pieceAt(square-8+inc)) != colorOf(b->pieceAt(square))){
				addMove(move(square,(square-8+inc),MFTPQ));
				addMove(move(square,(square-8+inc),MFTPR));
				addMove(move(square,(square-8+inc),MFTPB));
				addMove(move(square,(square-8+inc),MFTPN));
			}
			if(col(square) != 7 && b->pieceAt(square+8+inc) != EMPTY && colorOf(b->pieceAt(square+8+inc)) != colorOf(b->pieceAt(square))){
				addMove(move(square,(square+8+inc),MFTPQ));
				addMove(move(square,(square+8+inc),MFTPR));
				addMove(move(square,(square+8+inc),MFTPB));
				addMove(move(square,(square+8+inc),MFTPN));
			}
		} else {
			if(b->pieceAt(square+inc) == EMPTY){
				addMove(move(square,(square+inc),MFMOV));
			}
			if(col(square) != 0 && b->pieceAt(square-8+inc) != EMPTY && colorOf(b->pieceAt(square-8+inc)) != colorOf(b->pieceAt(square))){
				addMove(move(square,(square-8+inc),MFTAK));
			}
			if(col(square) != 7 && b->pieceAt(square+8+inc) != EMPTY && colorOf(b->pieceAt(square+8+inc)) != colorOf(b->pieceAt(square))){
				addMove(move(square,(square+8+inc),MFTAK));
			}
			if(row(square) == 1 && inc ==  1 && b->pieceAt(square+inc) == EMPTY && b->pieceAt(square+2*inc) == EMPTY){addMove(move(square,(square + 2*inc),MFMOV));}
			if(row(square) == 6 && inc == -1 && b->pieceAt(square+inc) == EMPTY && b->pieceAt(square+2*inc) == EMPTY){addMove(move(square,(square + 2*inc),MFMOV));}
			if(b->epTarget != 0 && (b->epTarget == square+8+inc || b->epTarget == square-8+inc)){
				addMove(move(square,b->epTarget,MFENP));
			}
		}
	}
}

void MoveGenerator::addMove(MOVE m) {
	moves.push_back(m);
}

MOVE MoveGenerator::getMove(MOVE m) {
	generateMoves();
	for(unsigned int i = 0; i < moves.size(); i++){
		if(from(moves[i]) == from(m) && to(moves[i]) == to(m)){
			return moves[i];
		}
	}
	return 0;
}

bool MoveGenerator::squareThreatenedBy(SQUARE square, COLOR c) {
	c = !c;
	uint8_t tmpCol = col(square);
	uint8_t tmpRow = row(square);
	int inc = 0;
	if(c == WHITE){
		inc = 1;
	} else {
		inc = -1;
	}
	if(tmpCol != 0 && tmpRow != 7*c){
		if(b->pieceAt(square-8+inc) == (WPAWN+c)){return true;}
	}
	if(tmpCol != 7 && tmpRow != 7*c){
		if(b->pieceAt(square+8+inc) == (WPAWN+c)){return true;}
	}

	if(tmpCol > 1 && tmpRow > 0){if(b->pieceAt(square - 17) == (WKNIGHT+c)){return true;}}
	if(tmpCol > 1 && tmpRow < 7){if(b->pieceAt(square - 15) == (WKNIGHT+c)){return true;}}
	if(tmpCol > 0 && tmpRow > 1){if(b->pieceAt(square - 10) == (WKNIGHT+c)){return true;}}
	if(tmpCol > 0 && tmpRow < 6){if(b->pieceAt(square - 6) == (WKNIGHT+c)){return true;}}
	if(tmpCol < 7 && tmpRow > 1){if(b->pieceAt(square + 6) == (WKNIGHT+c)){return true;}}
	if(tmpCol < 7 && tmpRow < 6){if(b->pieceAt(square + 10) == (WKNIGHT+c)){return true;}}
	if(tmpCol < 6 && tmpRow > 0){if(b->pieceAt(square + 15) == (WKNIGHT+c)){return true;}}
	if(tmpCol < 6 && tmpRow < 7){if(b->pieceAt(square + 17) == (WKNIGHT+c)){return true;}}

	if(col(square) != 0){ //left
		if(b->pieceAt(square - 8) == (WKING+c)){return true;}
		if(row(square) != 0){if(b->pieceAt(square - 9) == (WKING+c)){return true;}} //down
		if(row(square) != 7){if(b->pieceAt(square - 7) == (WKING+c)){return true;}} //up
	}
	if(col(square) != 7){ //right
		if(b->pieceAt(square + 8) == (WKING+c)){return true;}
		if(row(square) != 0){if(b->pieceAt(square + 7) == (WKING+c)){return true;}} //down
		if(row(square) != 7){if(b->pieceAt(square + 9) == (WKING+c)){return true;}} //up
	}
	if(row(square) != 0){if(b->pieceAt(square - 1) == (WKING+c)){return true;}}//down
	if(row(square) != 7){if(b->pieceAt(square + 1) == (WKING+c)){return true;}}//up

	uint8_t down = Scount[square];
	uint8_t up = Ncount[square];
	uint8_t left = Wcount[square];
	uint8_t right = Ecount[square];
	uint8_t downLeft = SWcount[square];
	uint8_t upLeft = NWcount[square];
	uint8_t downRight = SEcount[square];
	uint8_t upRight = NEcount[square];

	SQUARE s = square;
	for(int i = 0; i < down; i++){
		s = s - 1;
		if(b->pieceAt(s) != EMPTY){break;}
	}
	if(b->pieceAt(s) == (WQUEEN + c) || b->pieceAt(s) == WROOK + c){return true;}
	s = square;
	for(int i = 0; i < up; i++){
		s = s + 1;
		if(b->pieceAt(s) != EMPTY){break;}
	}
	if(b->pieceAt(s) == (WQUEEN + c) || b->pieceAt(s) == WROOK + c){return true;}
	s = square;
	for(int i = 0; i < left; i++){
		s = s - 8;
		if(b->pieceAt(s) != EMPTY){break;}
	}
	if(b->pieceAt(s) == (WQUEEN + c) || b->pieceAt(s) == WROOK + c){return true;}
	s = square;
	for(int i = 0; i < right; i++){
		s = s + 8;
		if(b->pieceAt(s) != EMPTY){break;}
	}
	if(b->pieceAt(s) == (WQUEEN + c) || b->pieceAt(s) == WROOK + c){return true;}
	s = square;
	for(int i = 0; i < downLeft; i++){
		s = s - 9;
		if(b->pieceAt(s) != EMPTY){break;}
	}
	if(b->pieceAt(s) == (WQUEEN + c) || b->pieceAt(s) == WBISHOP + c){return true;}
	s = square;
	for(int i = 0; i < upLeft; i++){
		s = s - 7;
		if(b->pieceAt(s) != EMPTY){break;}
	}
	if(b->pieceAt(s) == (WQUEEN + c) || b->pieceAt(s) == WBISHOP + c){return true;}
	s = square;
	for(int i = 0; i < downRight; i++){
		s = s + 7;
		if(b->pieceAt(s) != EMPTY){break;}
	}
	if(b->pieceAt(s) == (WQUEEN + c) || b->pieceAt(s) == WBISHOP + c){return true;}
	s = square;
	for(int i = 0; i < upRight; i++){
		s = s + 9;
		if(b->pieceAt(s) != EMPTY){break;}
	}
	if(b->pieceAt(s) == (WQUEEN + c) || b->pieceAt(s) == WBISHOP + c){return true;}
	return false;
}

int MoveGenerator::gameOver() {
	//returns -1 if game not over
	//returns 0 if white lost
	//returns 1 if draw
	//returns 2 if white win
	if(b->drawCount == 100){printf("DRAW COUNT\n");return 1;}
	if(b->checkForRepetitions()){printf("REPETITION\n");return 1;}
	if(!b->hasMatingMaterial()){printf("MATING MATERIAL\n");return 1;}
	generateMoves();
	for(unsigned int i = 0; i < moves.size(); i++){
		Board* testB = new Board(b);
		MoveGenerator* mg = new MoveGenerator(testB);
		testB->makeMove(moves[i]);
		if(!mg->inCheck(b->getTurn())){
			delete mg;
			return -1;
		}
		delete mg;
	}
	if(b->getTurn() == WHITE && inCheck(WHITE)){
		return 0;
	}
	if(b->getTurn() == BLACK && inCheck(BLACK)){
		return 2;
	}
	return 1;
}

bool MoveGenerator::inCheck(COLOR c){
	return squareThreatenedBy(b->kings[c],!c);
}

void MoveGenerator::addListener(SQUARE origin, SQUARE target) {
	(colorOf(b->pieceAt(origin))) ? listenerCount[target]++:listenerCount[target]--;
}
