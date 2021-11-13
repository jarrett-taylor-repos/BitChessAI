#ifndef ATTACKER_H
#define ATTACKER_H

class Attacker {
    private: 
        int startSq;
        int targetSq;
        char piece;
    public:
        Attacker();
        Attacker(int, int, char);
        Attacker(const Attacker&);
        Attacker& operator=(const Attacker&);
        int getStartSq();
        int getTargetSq();
        char getPiece();
        Attacker operator()(int, int, char);
};

#endif