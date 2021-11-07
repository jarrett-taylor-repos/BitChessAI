#include "move.h"
using namespace std;

Move::Move() : startSq(-1), targetSq(-1) {}
Move::Move(int a, int b) : startSq(a), targetSq(b) {}
Move::Move(const Move& other) : startSq(other.startSq), targetSq(other.targetSq){}
Move& Move::operator=(const Move&other) {
    startSq = other.startSq;
    targetSq = other.targetSq;
    return *this;
}
bool Move::operator==(const Move &other) const {
    bool equal = (startSq == other.startSq) && (targetSq == other.targetSq);
    return equal;
}

int Move::getStartSq() { return startSq; }
int Move::getTargetSq() { return targetSq; }