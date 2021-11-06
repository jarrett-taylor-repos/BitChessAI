#include "move.h"
using namespace std;

Move::Move() : startSq(-1), endSq(-1) {}
Move::Move(int a, int b) : startSq(a), endSq(b) {}
Move::Move(const Move& other) 
    : startSq(other.startSq), endSq(other.endSq){}
Move& Move::operator=(const Move&other) {
    startSq = other.startSq;
    endSq = other.endSq;
    return *this;
}
bool Move::operator==(const Move &other) const {
    bool equal = (startSq == other.startSq) && (endSq == other.endSq);
    return equal;
}