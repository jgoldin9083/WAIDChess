/*
 * OpeningBook.cpp
 *
 *  Created on: Nov 25, 2014
 *      Author: jonathan
 */

#include "OpeningBook.h"

using namespace std;

void OpeningBook::getMoves(std::string FEN, std::vector<MOVE>* moveList) {
	ifstream openingBook;
	openingBook.open("opening_book.txt");
	vector<string> lines;
	if(!openingBook.is_open()){
		openingBook.open("../opening_book.txt");
		if(!openingBook.is_open()){
			return;
		}
	}
	string curLine;
	while(getline(openingBook,curLine)){
		if(!strncmp(curLine.c_str(),FEN.c_str(),FEN.size())){
			curLine.erase(0,FEN.length()+1);
			int j;
			sscanf(curLine.c_str(),"%d",&j);
			bool duplicate = false;
			for(unsigned int i = 0; i < moveList->size(); i++){
				if((*moveList)[i] == (MOVE)j){duplicate = true; break;}
			}
			if(!duplicate){
				moveList->push_back((MOVE)j);
			}
		}
	}
	openingBook.close();
}

OpeningBook::OpeningBook() {
}

OpeningBook::~OpeningBook() {
	// TODO Auto-generated destructor stub
}

