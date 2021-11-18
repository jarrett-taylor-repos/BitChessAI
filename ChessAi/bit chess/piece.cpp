#include "piece.h"
#include <algorithm>
#include <vector>
#include <map>
#include <string>
using namespace std;

int directionOffSets[8] = {8, -8, -1, 1, 7, -7, 9, -9};
int knightOffsets[8] = {6, 15, 17, 10, -6, -15, -17, -10}; //start at 2 left and up 1 then clockwise
int whitePawnOffsets[4] = {8, 16, 7, 9};
int blackPawnOffsets[4] = {-8, -16, -9, -7};
int whitePawnAttckingOffsets[2] = {7, 9}; //left, right
int blackPawnAttckingOffsets[2] = {-9, -7}; //left, right

int numSqauresToEdge[64][8];
int knightMoves[64][8];
int blackPawnMoves[64][4];
int whitePawnMoves[64][4];
int blackAttackingPawnMoves[64][2];
int whiteAttackingPawnMoves[64][2];
int kingMoves[64][8];
multimap<int, precomputedAttackerData> attackersOnWhite;
multimap<int, precomputedAttackerData> attackersOnBlack;
map<int, string> intToStringMap;
map<string, int> stringToIntMap;

//5 bits
//first 2 store color, last 3 store piece type
const char none = 0b000; //0
const char pawn = 0b001; //1
const char knight = 0b010; //2
const char bishop = 0b011; //3
const char rook = 0b100; //4 
const char queen = 0b101; //5
const char king = 0b110; //6

const char black = 0b01000; //8
const char white = 0b10000; //16

//NONE
const char None() { return none; }
//WHITE PIECES
const char WhitePawn() { return (white | pawn); }
const char WhiteKnight() { return (white | knight); }
const char WhiteBishop() { return (white | bishop); }
const char WhiteRook() { return (white | rook); }
const char WhiteQueen() { return (white | queen); }
const char WhiteKing() { return (white | king); }
//BLACK PIECES
const char BlackPawn() { return (black | pawn); }
const char BlackKnight() { return (black | knight); }
const char BlackBishop() { return (black | bishop); }
const char BlackRook() { return (black | rook);  }
const char BlackQueen() { return (black | queen); }
const char BlackKing() { return (black | king);  }
//TEST PIECE COLOR AND TYPE
bool isWhite(const char p) { return (p - white >= 0b0); }
bool isBlack(const char p) { return (p - black < 8 && p-black > 0); }
const char getPieceValue(const char p) { if(p < 8) { return p;} else { return isWhite(p) ? p - 16 : p - 8; } }
bool isKing(const char p) { return (getPieceValue(p) == 0b00110); }
bool isQueen(const char p) { return (getPieceValue(p) == 0b00101); }
bool isRook(const char p) { return (getPieceValue(p) == 0b00100); }
bool isBishop(const char p) { return (getPieceValue(p) == 0b00011); }
bool isKnight(const char p) { return (getPieceValue(p) == 0b00010); }
bool isPawn(const char p) { return (getPieceValue(p) == 0b00001); }
bool isNone(const char p) { return (p == 0b0); }
bool isColor(const char a, const char b) { return (a >> 3) == (b >> 3); }
bool isSlidingPiece(const char p) { return (isRook(p) || isBishop(p) || isQueen(p)); }
bool isPiece(const char a, const char b) { return (getPieceValue(a) == getPieceValue(b)); }


//calulate moves potentials
void precomputtedMoveData() {
    for(int file = 0; file < 8; file++) {
        for(int rank = 0; rank < 8; rank++) {
            //sliding pieces: queen, rook, bishop
            int numNorth = 7-rank;
            int numSouth = rank;
            int numWest = file;
            int numEast = 7-file;
            int sqIndex = rank * 8 + file;
            int directionArray[8] = {
                numNorth, 
                numSouth, 
                numWest, 
                numEast,
                min(numNorth, numWest),
                min(numSouth, numEast),
                min(numNorth, numEast),
                min(numSouth, numWest)
            };
            for(int i = 0; i < 8; i++) {
                numSqauresToEdge[sqIndex][i] = directionArray[i];
            }

            //knights
            bool possibleKnight[8] = {
                (numNorth >= 1) && (numWest >= 2),
                (numNorth >= 2) && (numWest >= 1),
                (numNorth >= 2) && (numEast >= 1),
                (numNorth >= 1) && (numEast >= 2),
                (numSouth >= 1) && (numEast >= 2),
                (numSouth >= 2) && (numEast >= 1),
                (numSouth >= 2) && (numWest >= 1),
                (numSouth >= 1) && (numWest >= 2),
            };
            for(int i=0; i < 8; i++) {
                int addValue = (knightOffsets[i]*possibleKnight[i]);
                knightMoves[sqIndex][i] = sqIndex + addValue;
            }

            //kings
            bool possibleKing[8] = {
                (numNorth >= 1),
                (numSouth >= 1),
                (numWest >= 1),
                (numEast >= 1),
                (numNorth >= 1) && (numWest >=1),
                (numSouth >= 1) && (numEast >= 1),
                (numNorth >= 1) && (numEast >= 1),
                (numSouth >= 1) && (numWest >= 1),
            };
            for(int i=0; i < 8; i++) {
                kingMoves[sqIndex][i] = sqIndex + (directionOffSets[i]*possibleKing[i]);
            }

            //white pawns
            bool possibleWhitePawn[4] = {
                (numNorth >= 1),
                (numNorth >= 2) && (sqIndex < 16) && (sqIndex > 7),
                (numNorth >= 1) && (numWest >= 1),
                (numNorth >= 1) && (numEast >= 1)
            };
            for(int i=0; i < 4; i++) {
                int addValue = (whitePawnOffsets[i]*possibleWhitePawn[i]);
                whitePawnMoves[sqIndex][i] = sqIndex + addValue;
                if (i > 1) {
                    whiteAttackingPawnMoves[sqIndex][i-2] = sqIndex + addValue;
                }
            }

            //black pawns
            bool possibleBlackPawn[4] = {
               (numSouth >= 1),
               (numSouth >= 2) && (sqIndex < 56) && (sqIndex > 47),
               (numSouth >= 1) && (numWest >= 1),
               (numSouth >= 1) && (numEast >= 1)
            };
            for(int i=0; i < 4; i++) {
                int addValue = (blackPawnOffsets[i]*possibleBlackPawn[i]);
                blackPawnMoves[sqIndex][i] = sqIndex + addValue;
                if (i > 1) {
                    blackAttackingPawnMoves[sqIndex][i-2] = sqIndex + addValue;
                }
            }
        }
    }
    return;
}

void precomputtedIntToString() {
    for(int file = 0; file < 8; file++) {
        for(int rank = 0; rank < 8; rank++) { 
            int startSq = file*8 + rank;
            string s = "";
            int remainder = startSq % 8;
            switch(remainder) {
                case 0: s+="a"; break;
                case 1: s+="b"; break;
                case 2: s+="c"; break;
                case 3: s+="d"; break;
                case 4: s+="e"; break;
                case 5: s+="f"; break;
                case 6: s+="g"; break;
                case 7: s+="h"; break;
                default: break;
            }
            int intRow = (startSq-remainder)/8 + 1;
            s += to_string(intRow);
            intToStringMap.insert(make_pair(startSq, s));
        }
    }
    return;
}

void precomputtedStringToInt() {
    for(int file = 0; file < 8; file++) {
        for(int rank = 0; rank < 8; rank++) { 
            int startSq = file*8 + rank;
            string s = "";
            int remainder = startSq % 8;
            switch(remainder) {
                case 0: s+="a"; break;
                case 1: s+="b"; break;
                case 2: s+="c"; break;
                case 3: s+="d"; break;
                case 4: s+="e"; break;
                case 5: s+="f"; break;
                case 6: s+="g"; break;
                case 7: s+="h"; break;
                default: break;
            }
            int intRow = (startSq-remainder)/8 + 1;
            s += to_string(intRow);
            stringToIntMap.insert(make_pair(s, startSq));
        }
    }
    return;
}

void init() {//load on start of program
    precomputtedMoveData();
    precomputtedIntToString();
    precomputtedStringToInt();
    cout << "INIT complete" << endl;
}
