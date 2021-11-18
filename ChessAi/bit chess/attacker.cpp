#include "attacker.h"

Attacker::Attacker() : startSq(-1), targetSq(-1), piece(char(-1)){}
Attacker::Attacker(int s, int e, char p) : startSq(s), targetSq(e), piece(p) {}
Attacker::Attacker(const Attacker& other) 
: startSq(other.startSq), targetSq(other.targetSq), piece(other.piece) {}
Attacker& Attacker::operator=(const Attacker&other) {
    startSq = other.startSq;
    targetSq = other.targetSq;
    piece = other.piece;
    return *this;
}
int Attacker::getStartSq() { return startSq; }
int Attacker::getTargetSq(){ return targetSq; }
char Attacker::getPiece(){ return piece; }