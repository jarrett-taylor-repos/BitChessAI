#include "UciMove.h"
using namespace std;

UciMove::UciMove() : start(""), target("") {}
UciMove::UciMove(string s, string e) : start(s), target(e) {}
UciMove::UciMove(const UciMove& o) : start(o.start), target(o.target) {}
UciMove& UciMove::operator=(const UciMove& o) {
    start = o.start;
    target = o.target;
    return *this;
}
bool UciMove::operator==(const UciMove &o) const {
    bool equal = (start == o.start) && (target == o.target);
    return equal;
}
string UciMove::getStart() { return start; }
string UciMove::getTarget() { return target; }