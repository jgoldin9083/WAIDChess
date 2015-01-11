/*
 * util.h
 *
 *  Created on: Nov 9, 2014
 *      Author: jonathan
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>
#include <stdio.h>
#include <string>

#define COLOR bool
#define WHITE 1
#define BLACK 0

//type defines

#define SQUARE uint8_t
//column = bits 0-2
//row = bits 3-5
#define MOVE uint16_t
//origin = bits 0-5
//target = bits 6-11
//flags = bits 12-15
#define PIECE uint8_t

//move flags defines
#define MFMOV 0
#define MFTAK 1
#define MFENP 2
#define MFLIS 15
#define MFMPQ 13
#define MFMPR 11
#define MFMPB 9
#define MFMPN 7
#define MFTPQ 14
#define MFTPR 12
#define MFTPB 10
#define MFTPN 8
#define MFWKC 3
#define MFWQC 4
#define MFBKC 5
#define MFBQC 6

//metadata flags
#define TURN 0
#define CASTLEWK 1
#define CASTLEWQ 2
#define CASTLEBK 3
#define CASTLEBQ 4

//piece defines
#define BKING   2
#define BQUEEN  4
#define BBISHOP 6
#define BKNIGHT 8
#define BROOK   10
#define BPAWN   12
// Black Piece & 1 == 0
#define WKING   1
#define WQUEEN  3
#define WBISHOP 5
#define WKNIGHT 7
#define WROOK   9
#define WPAWN   11
// White Piece & 1 == 1
#define EMPTY   0

#define colorOf(p) (p&1)
#define col(s) (s>>3)
#define row(s) (s&0b111)
#define from(m) (m&0b111111)
#define to(m) ((m>>6)&0b111111)
#define getFlag(m) (m>>12)
#define move(f,t,fl) (((MOVE)t<<6)+f+((MOVE)fl<<12))
#define removeFlag(m) (m&0b111111111111)
#define square(col,row) ((col<<3)+row)

void printMove(MOVE m);
std::string toString(PIECE p);
std::string moveString(MOVE m);

#endif /* DEFINES_H_ */
