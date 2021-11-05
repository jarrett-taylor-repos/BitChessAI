#include "precomputedAttackerData.h"
#include <algorithm>
using namespace std;

//5 bits
//first 2 store color, last 3 store piece type
const char none = 0b00000; //0
const char pawn = 0b00001; //1
const char knight = 0b00010; //2
const char bishop = 0b00011; //3
const char rook = 0b00100; //4 
const char queen = 0b00101; //5
const char king = 0b00110; //6

const char black = 0b01000; //8
const char white = 0b10000; //16

//NONE
const char None() {
    return none;
}

//WHITE PIECES
const char WhitePawn() {
    return (white | pawn); 
}
const char WhiteKnight() {
    return (white | knight); 
}
const char WhiteBishop() {
    return (white | bishop); 
}
const char WhiteRook() {
    return (white | rook); 
}
const char WhiteQueen() {
    return (white | queen); 
}
const char WhiteKing() {
    return (white | king); 
}

//BLACK PIECES
const char BlackPawn() {
    return (black | pawn); 
}
const char BlackKnight() {
    return (black | knight); 
}
const char BlackBishop() {
    return (black | bishop); 
}
const char BlackRook() {
    return (black | rook); 
}
const char BlackQueen() {
    return (black | queen); 
}
const char BlackKing() {
    return (black | king); 
}


//TEST PIECE COLOR AND TYPE

bool isWhite(const char p) {
    return (p - white >= 0b0);
}
bool isBlack(const char p) {
    return (p - black < 8 && p-black > 0);
}

const char getPieceValue(const char p) {
    if(p < 8) {
        return p;
    } else {
        char value = isWhite(p) ? p - 16 : p - 8;
        return value;
    }
}

bool isKing(const char p) {
    return (getPieceValue(p) == 0b00110);
}
bool isQueen(const char p) {
    return (getPieceValue(p) == 0b00101);
}
bool isRook(const char p) {
    return (getPieceValue(p) == 0b00100);
}
bool isBishop(const char p) {
    return (getPieceValue(p) == 0b00011);
}
bool isKnight(const char p) {
    return (getPieceValue(p) == 0b00010);
}
bool isPawn(const char p) {
    return (getPieceValue(p) == 0b00001);
}
bool isNone(const char p) {
    return (p == 0b0);
}


bool isColor(const char a, const char b) {
    return (a >> 3) == (b >> 3);
}

bool isSlidingPiece(const char p) {
    return (isRook(p) || isBishop(p) || isQueen(p));
}


//calulate moves potentials
const int directionOffSets[8] = {8, -8, -1, 1, 7, -7, 9, -9};
const int knightOffsets[8] = {6, 15, 17, 10, -6, -25, -17, -10};
const int whitePawnOffsets[4] = {8, 16, 7, 9};
const int blackPawnOffsets[4] = {-8, -16, -7, -9};
const int whitePawnAttckingOffsets[2] = {7, 9};
const int blackPawnAttckingOffsets[2] = {-9, -7};
int** numSqauresToEdge;
int** knightMoves;
int** blackPawnMoves;
int** whitePawnMoves;
int** kingMoves;
vector<pair<int, precomputedAttackerData>> attackersOnWhite;
vector<pair<int, precomputedAttackerData>> attackersOnBlack;
map<int, string> intToStringMap;
map<int, string> stringToIntMap;

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
            numSqauresToEdge[sqIndex] = directionArray;

            //knights
            int knightarray[8];
            for(int i=0; i < 8; i++) {
                knightarray[i] = sqIndex + knightOffsets[i];
            }
            knightMoves[sqIndex] = knightarray;

            //kings
            int kingarray[8];
            for(int i=0; i < 8; i++) {
                kingarray[i] = sqIndex + directionOffSets[i];
            }
            kingMoves[sqIndex] = kingarray;

            //white pawns
            int whitepawnarray[4];
            for(int i=0; i < 8; i++) {
                whitepawnarray[i] = sqIndex + whitePawnOffsets[i];
            }
            whitePawnMoves[sqIndex] = whitepawnarray;

            //black pawns
            int blackpawnarray[4];
            for(int i=0; i < 8; i++) {
                blackpawnarray[i] = sqIndex + blackPawnOffsets[i];
            }
            blackPawnMoves[sqIndex] = blackpawnarray;
        }
    }
    return;
}

void precomputtedPawnAttackers(const char color, int startSq, const int* offsets, int size, const char piece) {
    if(isWhite(color)) {
        for(int whitePawnIndex = 0; whitePawnIndex < size; whitePawnIndex++) {
            int targetSQ = startSq + offsets[whitePawnIndex];
            if(targetSQ >= 0 && targetSQ < 64) {
                //add possible pawn attacker, add to black king
                precomputedAttackerData data(targetSQ, piece, false, 0);
                attackersOnBlack.push_back(make_pair(startSq, data));
            }
        }
    } else {
        for(int blackPawnIndex = 0; blackPawnIndex < size; blackPawnIndex++) {
            int targetSQ = startSq + offsets[blackPawnIndex];
            if(targetSQ >= 0 && targetSQ < 64) {
                //add possible pawn attacker, add to white king
                precomputedAttackerData data(targetSQ, piece, false, 0);
                attackersOnWhite.push_back(make_pair(startSq, data));
            }
        }
    }
    return;
}

void precomputedAttackersHelper(int startSq, const int* offsets, int size, const char piece) {
    for(int index = 0; index < size; index++) {
        int targetSQ = startSq + offsets[index];
        if(targetSQ >= 0 && targetSQ < 64) {
            //add possible knight attacker
            precomputedAttackerData data(targetSQ, piece, false, 0);
            attackersOnWhite.push_back(make_pair(startSq, data));
            attackersOnBlack.push_back(make_pair(startSq, data));
        }
    }
    return;
}

void precomputtedSlidingPieces(int startSq, const char piece) {
    int startDirIndex = isBishop(piece) ? 4 : 0;
    int endDirIndex = isRook(piece) ? 4 : 8; 

    for(int directionIndex = 0; directionIndex < 4; directionIndex++) {
        for(int n = 0; n < numSqauresToEdge[startSq][directionIndex]; n++) {
            int targetSq = startSq + directionOffSets[directionIndex] * (n+1);
            if(abs(targetSq-startSq) < 10) {
                precomputedAttackerData data(targetSq, rook, false, n);
                attackersOnWhite.push_back(make_pair(startSq, data));
                attackersOnBlack.push_back(make_pair(startSq, data));
            } else {
                precomputedAttackerData data(targetSq, rook, true, n);
                attackersOnWhite.push_back(make_pair(startSq, data));
                attackersOnBlack.push_back(make_pair(startSq, data));
            }
        }
    }
    return;
}

void precomputtedPossibleAttackers() {

    const int knightOffsets[8] = {6, 15, 17, 10, -6, -25, -17, -10};
    const int whitePawnAttckingOffsets[2] = {7, 9};
    const int blackPawnAttckingOffsets[2] = {-9, -7};

    for(int startSq = 0; startSq < 64; startSq++) {
        //need to get knights attacking squares 
        precomputedAttackersHelper(startSq, knightOffsets, sizeof(knightOffsets)/sizeof(const int), knight);
        //need to get pawns attacking sqaures
        precomputtedPawnAttackers(white, startSq, whitePawnAttckingOffsets, sizeof(whitePawnAttckingOffsets)/sizeof(const int), pawn);
        precomputtedPawnAttackers(black, startSq, blackPawnAttckingOffsets, sizeof(blackPawnAttckingOffsets)/sizeof(const int), pawn);
        //need to get king attacking sqaures
        precomputedAttackersHelper(startSq, directionOffSets, sizeof(directionOffSets)/sizeof(const int), king);
        //need to get rook, bishop, queen attacking squares
        precomputtedSlidingPieces(startSq, rook);
        precomputtedSlidingPieces(startSq, bishop);
        precomputtedSlidingPieces(startSq, queen);
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
            stringToIntMap.insert(make_pair(s, startSq));
        }
    }
}

void init() {
    precomputtedMoveData();
    precomputtedPossibleAttackers();
    precomputtedIntToString();
}
