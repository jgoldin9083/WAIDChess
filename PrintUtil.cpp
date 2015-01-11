/*
 * PrintUtil.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: jonathan
 */

#include "PrintUtil.h"

using namespace std;

string toString(PIECE p) {
	switch(p){
		case WPAWN: return "wp"; break;
		case WROOK: return "wR"; break;
		case WKNIGHT: return "wN"; break;
		case WBISHOP: return "wB"; break;
		case WQUEEN: return "wQ"; break;
		case WKING: return "wK"; break;
		case BPAWN: return "bp"; break;
		case BROOK: return "bR"; break;
		case BKNIGHT: return "bN"; break;
		case BBISHOP: return "bB"; break;
		case BQUEEN: return "bQ"; break;
		case BKING: return "bK"; break;
		default:
			return "--";
		break;
	}
	return "--";
}

void display(Board* board) {
	printf("///////////////////////////////\n");
	printf("///////////////////////////////\n");
	for(int i = 7; i >= 0; i--){
		for(int j = 0; j < 8; j++){
			printf(" %s ",toString(board->pieceAt(j*8+i)).c_str());
		}
		printf("\n");
	}
	printf("///////////////////////////////\n");
	printf("///////////////////////////////\n");
}


