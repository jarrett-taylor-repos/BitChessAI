#ifndef PIECE_H
#define PIECE_H
#include "precomputedAttackerData.cpp"
#include <string>
#include <map>
#include <vector>
using std::vector;
using std::map;
using std::multimap;
using std::string;

extern int numSqauresToEdge[64][8];
extern int knightMoves[64][8];
extern int blackPawnMoves[64][4];
extern int whitePawnMoves[64][4];
extern int kingMoves[64][8];
extern int blackAttackingPawnMoves[64][2];
extern int whiteAttackingPawnMoves[64][2];
extern multimap<int, precomputedAttackerData> attackersOnWhite;
extern multimap<int, precomputedAttackerData> attackersOnBlack;
extern map<int, string> intToStringMap;
extern map<string, int> stringToIntMap;

#endif