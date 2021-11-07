#ifndef PRECOMPUTEDATTCKERDATA_H
#define PRECOMPUTEDATTCKERDATA_H
#include <iostream>
using std::ostream;

class precomputedAttackerData {
    public: 
        int targetSq;
        char piece;
        bool possiblePin;
        int numSquaresAway;
    public:
        precomputedAttackerData();
        precomputedAttackerData(int, char, bool, int);
        precomputedAttackerData(const precomputedAttackerData&);
        precomputedAttackerData& operator=(const precomputedAttackerData&);
        bool operator< (const precomputedAttackerData&) const;
        precomputedAttackerData operator()(int, char, bool, int);
        int getTargetSq();
        char getPiece();
        bool getPossiblePin();
        int getNumSquaresAway();
        friend std::ostream& operator<<(std::ostream& os, const precomputedAttackerData& preAttData);
};

#endif