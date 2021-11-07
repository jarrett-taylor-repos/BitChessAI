#include "precomputedAttackerData.h"
#include <string>
using namespace std;

precomputedAttackerData::precomputedAttackerData(): targetSq(-1), piece((char)-1), possiblePin(false), numSquaresAway(0){};
precomputedAttackerData::precomputedAttackerData(int s, const char p, bool b, int o) : targetSq(s), piece(p), possiblePin(b), numSquaresAway(o) {}
precomputedAttackerData::precomputedAttackerData(const precomputedAttackerData& other) 
: targetSq(other.targetSq), piece(other.piece), possiblePin(other.possiblePin), numSquaresAway(other.numSquaresAway){}

precomputedAttackerData& precomputedAttackerData::operator=(const precomputedAttackerData& other) {
    targetSq = other.targetSq;
    piece = other.piece;
    possiblePin = other.possiblePin;
    numSquaresAway = other.numSquaresAway;
    return *this;
}

bool precomputedAttackerData::operator<(const precomputedAttackerData& other) const {
    if(other.targetSq < this->targetSq) {
        return true;
    }
    return false;
}

precomputedAttackerData precomputedAttackerData::operator()(int t, char p, bool pin, int a) {
    precomputedAttackerData preAtt;
    preAtt.targetSq = t;
    preAtt.piece = p;
    preAtt.possiblePin = pin;
    preAtt.numSquaresAway = a;
    return preAtt;
}

int precomputedAttackerData::getTargetSq() { return targetSq; }
char precomputedAttackerData::getPiece() { return piece; }
bool precomputedAttackerData::getPossiblePin() { return possiblePin; }
int precomputedAttackerData::getNumSquaresAway() {return numSquaresAway; }


ostream& operator<<(ostream& os, const precomputedAttackerData& preAttData) {
    os << "Target Square: " << preAttData.targetSq << " ";
    os << "Piece: " << to_string((int)preAttData.piece) << " ";
    os << "Possible Pin: " << preAttData.possiblePin << " ";
    os << "Num Sqaures Away: " << preAttData.numSquaresAway << endl;
    return os;
}