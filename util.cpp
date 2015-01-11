
#include "util.h"

using namespace std;

string moveString(MOVE m) {
	string let = "abcdefgh";
	string num = "12345678";
	string str = "";
	str += let[col(from(m))];
	str += num[row(from(m))];
	str += let[col(to(m))];
	str += num[row(to(m))];
	return str;
}
void printMove(MOVE m){
	string s = "abcdefgh";
	printf("%c%d%c%d %d\n",s[col(from(m))],row(from(m))+1,s[col(to(m))],row(to(m))+1,getFlag(m));
}
