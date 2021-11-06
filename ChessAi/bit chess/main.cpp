#include "board.cpp"

extern int numSqauresToEdge[64][8];
extern int knightMoves[64][8];
extern int blackPawnMoves[64][4];
extern int whitePawnMoves[64][4];
extern int kingMoves[64][8];
extern multimap<int, precomputedAttackerData> attackersOnWhite;
extern multimap<int, precomputedAttackerData> attackersOnBlack;
extern map<int, string> intToStringMap;
extern map<string, int> stringToIntMap;

int main() {
    init();
    Board b;
    b.print();
    multimap<int, precomputedAttackerData>::iterator itr;
    for(itr = attackersOnWhite.begin(); itr != attackersOnWhite.end(); ++itr) {
        cout << itr->first << ", " << itr->second;
    }
}