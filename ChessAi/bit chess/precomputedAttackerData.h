#ifndef PRECOMPUTEDATTCKERDATA_H
#define PRECOMPUTEDATTCKERDATA_H

class precomputedAttackerData {
    private: 
        int targetSq;
        char piece;
        bool possiblePin;
        int numSquaresAway;
    public:
        precomputedAttackerData();
        precomputedAttackerData(int, char, bool, int);
        precomputedAttackerData(const precomputedAttackerData&);
        precomputedAttackerData& operator=(const precomputedAttackerData&);
        int getTargetSq();
        char getPiece();
        bool getPossiblePin();
        int getNumSquaresAway();
};

#endif