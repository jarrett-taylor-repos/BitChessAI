#ifndef MOVE_H_
#define MOVE_H_
#include <string>

struct Move {
    int startSq;
    int endSq;
    Move(int, int);
};

#endif MOVE_H_