/*
 * HashTable.h
 *
 *  Created on: Nov 17, 2014
 *      Author: jonathan
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "Node.h"

#define NODETYPE uint8_t
#define LOWERBOUND 0
#define EXACT 1
#define UPPERBOUND 2

struct HashEntry{
	uint64_t key;
	long value;
	int depth;
	MOVE bestMove;
	NODETYPE nodeType;
};

class HashTable {
private:
	int size;
	int mask;
	uint64_t zobristTable[64][15];
	HashEntry* entries;
public:
	uint64_t hashBoard(Board* b);
	void insertEntry(Node* n,int level,long score,NODETYPE nt);
	HashEntry* retrieve(Board* b);
	HashTable(int size);
	virtual ~HashTable();
};
#endif /* HASHTABLE_H_ */
