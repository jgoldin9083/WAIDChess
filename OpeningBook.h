/*
 * OpeningBook.h
 *
 *  Created on: Nov 25, 2014
 *      Author: jonathan
 */

#ifndef OPENINGBOOK_H_
#define OPENINGBOOK_H_

#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include "util.h"


class OpeningBook {
public:
	static void getMoves(std::string FEN, std::vector<MOVE>* moveList);
	OpeningBook();
	virtual ~OpeningBook();
};

#endif /* OPENINGBOOK_H_ */
