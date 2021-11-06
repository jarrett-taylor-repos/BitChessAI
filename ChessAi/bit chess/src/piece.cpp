#include "precomputedAttackerData.cpp"
#include <algorithm>
#include <vector>
#include <map>
#include <string>
using namespace std;


vector<int> directionOffSets = {8, -8, -1, 1, 7, -7, 9, -9};
vector<int> knightOffsets = {6, 15, 17, 10, -6, -25, -17, -10};
vector<int> whitePawnOffsets = {8, 16, 7, 9};
vector<int> blackPawnOffsets = {-8, -16, -7, -9};
vector<int> whitePawnAttckingOffsets = {7, 9};
vector<int> blackPawnAttckingOffsets = {-9, -7};
int numSqauresToEdge[64][8];
int knightMoves[64][8];
int blackPawnMoves[64][4];
int whitePawnMoves[64][4];
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
            for(int i=0; i < 8; i++) {
                knightMoves[sqIndex][i] = sqIndex + knightOffsets[i];
            }

            //kings
            for(int i=0; i < 8; i++) {
                kingMoves[sqIndex][i] = sqIndex + directionOffSets[i];
            }

            //white pawns
            for(int i=0; i < 8; i++) {
                whitePawnMoves[sqIndex][i] = sqIndex + whitePawnOffsets[i];
            }

            //black pawns
            for(int i=0; i < 8; i++) {
                blackPawnMoves[sqIndex][i] = sqIndex + blackPawnOffsets[i];
            }
        }
    }
    return;
}

multimap<int, precomputedAttackerData> precomputtedPawnAttackers(const char color, int startSq, vector<int> offsets, int size, const char piece) {
    multimap<int, precomputedAttackerData> temp;
    if(isWhite(color)) {
        for(int whitePawnIndex = 0; whitePawnIndex < size; whitePawnIndex++) {
            int targetSQ = startSq + offsets[whitePawnIndex];
            if(targetSQ >= 0 && targetSQ < 64) {
                //add possible pawn attacker, add to black king
                precomputedAttackerData data(targetSQ, piece, false, 0);
                temp.insert(make_pair(startSq, data));
            }
        }
        //attackersOnWhite.insert(temp.begin(), temp.end());
    } else {
        for(int blackPawnIndex = 0; blackPawnIndex < size; blackPawnIndex++) {
            int targetSQ = startSq + offsets[blackPawnIndex];
            if(targetSQ >= 0 && targetSQ < 64) {
                //add possible pawn attacker, add to white king
                precomputedAttackerData data(targetSQ, piece, false, 0);
                temp.insert(make_pair(startSq, data));
            }
        }
        //attackersOnBlack.insert(temp.begin(), temp.end());
    }
    return temp;
}

multimap<int, precomputedAttackerData> precomputedAttackersHelper(int startSq, vector<int> offsets, int size, const char piece) {
    multimap<int, precomputedAttackerData> temp;
    for(int index = 0; index < size; index++) {
        int targetSQ = startSq + offsets[index];
        if(targetSQ >= 0 && targetSQ < 64) {
            precomputedAttackerData data(targetSQ, piece, false, 0);
            pair<int, precomputedAttackerData> addPair = make_pair(startSq, data);
            temp.insert(addPair);
        }
    }
    // attackersOnWhite.insert(temp.begin(), temp.end());
    // attackersOnBlack.insert(temp.begin(), temp.end());
    return temp;
}

multimap<int, precomputedAttackerData> precomputtedSlidingPieces(int startSq, const char piece) {
    int startDirIndex = isBishop(piece) ? 4 : 0;
    int endDirIndex = isRook(piece) ? 4 : 8; 

    multimap<int, precomputedAttackerData> temp;
    for(int directionIndex = startDirIndex; directionIndex < endDirIndex; directionIndex++) {
        for(int n = 0; n < numSqauresToEdge[startSq][directionIndex]; n++) {
            int targetSq = startSq + directionOffSets[directionIndex] * (n+1);
            if(abs(targetSq-startSq) < 10) {
                precomputedAttackerData data(targetSq, piece, false, n);
                temp.insert(make_pair(startSq, data));
            } else {
                precomputedAttackerData data(targetSq, piece, true, n);
                temp.insert(make_pair(startSq, data));
            }
        }
    }
    // attackersOnWhite.insert(temp.begin(), temp.end());
    // attackersOnBlack.insert(temp.begin(), temp.end());
    return temp;
}

multimap<int, precomputedAttackerData> precomputtedPossibleAttackers(const char color) {
    multimap<int, precomputedAttackerData> temp;
    multimap<int, precomputedAttackerData> returnMap;
    vector<int> whitePawnAttckingOffsets = {7, 9};
    vector<int> blackPawnAttckingOffsets = {-9, -7};

    for(int startSq = 0; startSq < 64; startSq++) {
        //need to get knights attacking squares 
        temp = precomputedAttackersHelper(startSq, knightOffsets, sizeof(knightOffsets)/sizeof(const int), knight);
        returnMap.insert(temp.begin(), temp.end());
        temp.clear();
        //need to get pawns attacking sqaures
        if(color == white) {
            temp = precomputtedPawnAttackers(white, startSq, whitePawnAttckingOffsets, sizeof(whitePawnAttckingOffsets)/sizeof(const int), pawn);
            returnMap.insert(temp.begin(), temp.end());
            temp.clear();
        } else {
            temp = precomputtedPawnAttackers(black, startSq, blackPawnAttckingOffsets, sizeof(blackPawnAttckingOffsets)/sizeof(const int), pawn);
            returnMap.insert(temp.begin(), temp.end());
            temp.clear();
        }
        
        //need to get king attacking sqaures
        temp = precomputedAttackersHelper(startSq, directionOffSets, sizeof(directionOffSets)/sizeof(const int), king);
        returnMap.insert(temp.begin(), temp.end());
        temp.clear();
        //need to get rook, bishop, queen attacking squares
        temp = precomputtedSlidingPieces(startSq, rook);
        returnMap.insert(temp.begin(), temp.end());
        temp.clear();

        temp = precomputtedSlidingPieces(startSq, bishop);
        returnMap.insert(temp.begin(), temp.end());
        temp.clear();

        temp = precomputtedSlidingPieces(startSq, queen);
        returnMap.insert(temp.begin(), temp.end());
        temp.clear();
    }
    return returnMap;
}

map<int, string> precomputtedIntToString() {
    map<int, string> temp;
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
            temp.insert(make_pair(startSq, s));
        }
    }
    return temp;
}

map<string, int> precomputtedStringToInt() {
    map<string, int> temp;
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
            temp.insert(make_pair(s, startSq));
        }
    }
    return temp;
}

void init() {//load on start of program
    precomputtedMoveData();
    attackersOnWhite = precomputtedPossibleAttackers(white);
    attackersOnBlack = precomputtedPossibleAttackers(black);
    intToStringMap = precomputtedIntToString();
    stringToIntMap = precomputtedStringToInt();
    cout << "INIT complete" << endl;
}
