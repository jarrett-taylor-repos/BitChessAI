#ifndef MOVE_H
#define MOVE_H

class Move {
    private: 
        int startSq;
        int targetSq;
    public:
        Move();
        Move(int, int);
        Move(const Move&);
        Move& operator=(const Move&);
        bool operator==(const Move &b) const;
        int getStartSq();
        int getTargetSq();
};

#endif