/*
 * HashTable.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: jonathan
 */

#include "HashTable.h"

/*
 * HashTable.cpp
 *
 *  Created on: Mar 25, 2014
 *      Author: jonathan
 */

#include "HashTable.h"

uint64_t HashTable::hashBoard(Board* b) {
	uint64_t hash = 0;
	for(int i = 0; i < 64; i++){
		hash ^= zobristTable[i][b->pieceAt(i)];
	}
	hash^= zobristTable[b->metadata][13];
	hash^= zobristTable[b->epTarget][14];
	return hash;
}

void HashTable::insertEntry(Node* n, int level,long score, NODETYPE nt) {
	uint64_t key = hashBoard(n->mg->b);
	int index = key&mask;
	entries[index].key = key;
	entries[index].depth = level - n->depth;
	entries[index].bestMove = n->bestMove;
	entries[index].nodeType = nt;
	entries[index].value = score;
}

HashEntry* HashTable::retrieve(Board* b) {
	uint64_t key = hashBoard(b);
	int index = key&mask;
	if(entries[index].key == key){
		return &entries[index];
	}
	return NULL;
}

HashTable::HashTable(int size) {
	size = pow(2,(int)(log(size)/log(2))+1);
	entries = new HashEntry[size];
	this->size = size;
	mask = this->size-1;

	for(int i = 0; i < size; i++){
		entries[i].bestMove = 0;
		entries[i].depth = -1;
		entries[i].key = 0;
		entries[i].nodeType = 0;
		entries[i].value = -1;
	}

	for(int i = 0; i < 64; i++){
		for(int j = 0; j < 15; j++){
			zobristTable[i][j] = rand();
			for(int k = 0; k < 4; k++){
				zobristTable[i][j] <<= 15;
				zobristTable[i][j] ^= rand();
			}
		}
	}

}

HashTable::~HashTable() {
	delete[] entries;
}


