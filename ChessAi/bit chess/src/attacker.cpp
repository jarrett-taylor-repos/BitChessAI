#include "attacker.h"

Attacker::Attacker() : startSq(-1), endSq(-1), piece(-1){}
Attacker::Attacker(int s, int e, char p) : startSq(s), endSq(e), piece(p) {}
Attacker::Attacker(const Attacker& other) 
: startSq(other.startSq), endSq(other.endSq), piece(other.piece) {}
Attacker& Attacker::operator=(const Attacker&other) {
    startSq = other.startSq;
    endSq = other.endSq;
    piece = other.piece;
    return *this;
}
int Attacker::getStartSq() { return startSq; }
int Attacker::getEndSq(){ return endSq; }
char Attacker::getPiece(){ return piece; }