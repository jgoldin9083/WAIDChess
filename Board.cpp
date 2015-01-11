/*
 * Board.cpp
 *
 *  Created on: Nov 16, 2014
 *      Author: jonathan
 */

#include "Board.h"

using namespace std;

Board::Board() {

	board[0]  = WROOK;
	board[8]  = WKNIGHT;
	board[16] = WBISHOP;
	board[24] = WQUEEN;
	board[32] = WKING;
	board[40] = WBISHOP;
	board[48] = WKNIGHT;
	board[56] = WROOK;

	board[7]  = BROOK;
	board[15] = BKNIGHT;
	board[23] = BBISHOP;
	board[31] = BQUEEN;
	board[39] = BKING;
	board[47] = BBISHOP;
	board[55] = BKNIGHT;
	board[63] = BROOK;

	for(int i = 0; i < 8; i++){
		for(int j  = 2; j < 6; j++){
			board[i*8+j] = EMPTY;
		}
		board[i*8+1] = WPAWN;
		board[i*8+6] = BPAWN;
	}
	epTarget = 0;
	drawCount = 0;
	metadata = 31;

	kings[WHITE] = 32;
	kings[BLACK] = 39;


/*
//	FOR MATE CHECKING
	board[1] = BKING;
	board[16] = WKING;
	board[15] = WROOK;
	kings[WHITE] = 16;
	kings[BLACK] = 1;
	metadata = 1;
*/
/*
FOR MATINGMATERIAL CHECKING
	board[0] = BQUEEN;
	board[1] = BKING;
	kings[BLACK] = 1;
	board[5] = WKING;
	kings[WHITE] = 5;
	board[10] = WKNIGHT;
	epTarget = 0;
	drawCount = 0;
	metadata = 1;
*/
/*
FOR DRAW CHECKING
	board[0] = WKING;
	kings[WHITE] = 0;
	board[63] = BKING;
	kings[BLACK] = 63;
	board[1] = WPAWN;
	board[2] = BPAWN;
	board[24] = WPAWN;
	board[25] = BPAWN;
	board[26] = BPAWN;
	board[37] = WPAWN;
	board[38] = WPAWN;
	board[39] = BPAWN;
	board[61] = WPAWN;
	board[62] = BPAWN;
*/
}
Board::Board(Board* b){
	for(int i = 0; i < 64; i++){
		board[i] = b->board[i];
	}
	epTarget = b->epTarget;
	drawCount = b->drawCount;
	metadata = b->metadata;
	kings[WHITE] = b->kings[WHITE];
	kings[BLACK] = b->kings[BLACK];
	moves = b->moves;
}

Board::~Board() {
	// TODO Auto-generated destructor stub
}

void Board::setBoard(Board* b){
	for(int i = 0; i < 64; i++){
		board[i] = b->board[i];
	}
	epTarget = b->epTarget;
	drawCount = b->drawCount;
	metadata = b->metadata;
	kings[WHITE] = b->kings[WHITE];
	kings[BLACK] = b->kings[BLACK];
	moves = b->moves;
}

PIECE Board::pieceAt(SQUARE s) {
	return board[s];
}

COLOR Board::getTurn(){
	return metadata & 1;
}

void Board::putPiece(SQUARE s, PIECE p) {
	board[s] = p;
}

void Board::makeMove(MOVE m) {
	SQUARE origin = from(m);
	SQUARE target = to(m);
	uint8_t flag = getFlag(m);

	if(flag != MFMOV || pieceAt(origin) == WPAWN || pieceAt(origin) == BPAWN){
		drawCount = 0;
		moves.clear();
	} else {
		drawCount++;
		moves.push_back(m);
	}
	putPiece(target,pieceAt(origin));
	putPiece(origin,EMPTY);
	if(pieceAt(target) == BPAWN){
		switch(flag){
		case MFMPQ: case MFTPQ:
			putPiece(target,BQUEEN);
			break;
		case MFMPR: case MFTPR:
			putPiece(target,BROOK);
			break;
		case MFMPB: case MFTPB:
			putPiece(target,BBISHOP);
			break;
		case MFMPN: case MFTPN:
			putPiece(target,BKNIGHT);
			break;
		}
	} else if(pieceAt(target) == WPAWN){
		switch(flag){
		case MFMPQ: case MFTPQ:
			putPiece(target,WQUEEN);
			break;
		case MFMPR: case MFTPR:
			putPiece(target,WROOK);
			break;
		case MFMPB: case MFTPB:
			putPiece(target,WBISHOP);
			break;
		case MFMPN: case MFTPN:
			putPiece(target,WKNIGHT);
			break;
		}
	}
	switch(flag){
	case MFWKC:
		putPiece(56,EMPTY);
		putPiece(40,WROOK);
		break;
	case MFWQC:
		putPiece(0,EMPTY);
		putPiece(24,WROOK);
		break;
	case MFBKC:
		putPiece(63,EMPTY);
		putPiece(47,BROOK);
		break;
	case MFBQC:
		putPiece(7,EMPTY);
		putPiece(31,BROOK);
		break;
	case MFENP:
		putPiece(square(col(target),row(origin)),EMPTY);
		break;
	}
	if(origin == 0 || origin == 32 || target == 0){
		metadata &= ~(1 << CASTLEWQ);
	}
	if(origin == 56 || origin == 32 || target == 56){
		metadata &= ~(1 << CASTLEWK);
	}
	if(origin == 7 || origin == 39 || target == 7){
		metadata &= ~(1 << CASTLEBQ);
	}
	if(origin == 63 || origin == 39 || target == 63){
		metadata &= ~(1 << CASTLEBK);
	}
	if(origin == kings[0]){
		kings[0] = target;
	}
	if(origin == kings[1]){
		kings[1] = target;
	}

	if((pieceAt(target) == BPAWN || pieceAt(target) == WPAWN) && abs(row(origin) - row(target)) == 2){
		epTarget = (origin+target)/2;
	} else {
		epTarget = 0;
	}
	metadata ^= 1; // flips turn bit
}
string Board::toFEN(){
	string FEN = "";
	for(int row = 7; row >= 0; row--){
		for(int col = 0; col < 8; col++){
			switch(pieceAt(col*8+row)){
			case WPAWN:
				FEN += "P";
				break;
			case BPAWN:
				FEN += "p";
				break;
			case WROOK:
				FEN += "R";
				break;
			case BROOK:
				FEN += "r";
				break;
			case WKNIGHT:
				FEN += "N";
				break;
			case BKNIGHT:
				FEN += "n";
				break;
			case WBISHOP:
				FEN += "B";
				break;
			case BBISHOP:
				FEN += "b";
				break;
			case WQUEEN:
				FEN += "Q";
				break;
			case BQUEEN:
				FEN += "q";
				break;
			case WKING:
				FEN += "K";
				break;
			case BKING:
				FEN += "k";
				break;
			case EMPTY:
				int total = 0;
				while(col < 8 && pieceAt(col*8+row) == EMPTY){
					total++;
					col++;
				}
				col--;
				stringstream out;
				out << total;
				FEN += out.str();
				break;
			}
		}
		if(row != 0){FEN += "/";}
	}
	FEN += " ";
	if(getTurn() == WHITE){
		FEN += "w";
	} else {
		FEN += "b";
	}
	FEN += " ";
	if(!metadata >> 1){
		FEN += "-";
	} else {
		if((metadata >> CASTLEWK)&1){
			FEN += "K";
		}
		if((metadata >> CASTLEWQ)&1){
				FEN += "Q";
		}
		if((metadata >> CASTLEBK)&1){
			FEN += "k";
		}
		if((metadata >> CASTLEBQ)&1){
			FEN += "q";
		}
	}
	return FEN;
}
bool Board::hasMatingMaterial(){
	uint8_t whiteMinorPieceCount = 0;
	uint8_t blackMinorPieceCount = 0;
	for(int i = 0; i < 64; i++){
		switch(pieceAt(i)){
		case WPAWN:case BPAWN:
		case WROOK:case BROOK:
		case WQUEEN:case BQUEEN:
			return true;
			break;
		case WKNIGHT:case WBISHOP:
			whiteMinorPieceCount++;
			if(whiteMinorPieceCount >= 2){
				return true;
			}
			break;
		case BKNIGHT: case BBISHOP:
			blackMinorPieceCount++;
			if(blackMinorPieceCount >= 2){
				return true;
			}
		}
	}
	return false;
}
bool Board::checkForRepetitions(){
	if(moves.size() < 12){return false;}
	int s = moves.size();
	return  (moves[s-1] == moves[s-5] &&
		moves[s-2] == moves[s-6] &&
		moves[s-3] == moves[s-7] &&
		moves[s-4] == moves[s-8] &&
		moves[s-5] == moves[s-9] &&
		moves[s-6] == moves[s-10] &&
		moves[s-7] == moves[s-11] &&
		moves[s-8] == moves[s-12]);
}
