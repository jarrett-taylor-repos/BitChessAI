#include "precomputedAttackerData.h"

precomputedAttackerData::precomputedAttackerData(): targetSq(-1), piece(-1), possiblePin(-1), numSquaresAway(-1){};
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

int precomputedAttackerData::getTargetSq() { return targetSq; }
char precomputedAttackerData::getPiece() { return piece; }
bool precomputedAttackerData::getPossiblePin() { return possiblePin; }
int precomputedAttackerData::getNumSquaresAway() {return numSquaresAway; }