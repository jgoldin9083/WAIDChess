/*
 * Node.cpp
 *
 *  Created on: Nov 16, 2014
 *      Author: jonathan
 */

#include "Node.h"

int Node::count = 0;
void Node::resetCount(){
	count = 0;
}

Node::Node(Board* board, int d, MOVE move){
	mg = new MoveGenerator(board);
	depth = d;
	m = move;
	count++;
	bestMove = 0;
}
Node::Node(){
	mg = new MoveGenerator(new Board());
	m = 0;
	depth = 0;
	bestMove = 0;
}

Node::~Node() {
	delete mg;
}
void Node::setBoard(Board* b,int depth,MOVE m){
//	delete mg->b;
	mg->b->setBoard(b);
	mg->b->makeMove(m);
//	mg->b = b;
	this->depth = depth;
	this->m = m;
	count++;
}
