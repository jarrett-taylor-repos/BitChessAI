#ifndef MOVE_H
#define MOVE_H

class Move {
    private: 
        int startSq;
        int endSq;
    public:
        Move();
        Move(int, int);
        Move(const Move&);
        Move& operator=(const Move&);
        bool operator==(const Move &b) const;
};

#endif