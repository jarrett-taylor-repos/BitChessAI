#include "board.cpp"

int main() {
    init();
    Board b;
    b.print();
    multimap<int, precomputedAttackerData>::iterator itr;
    for(itr = attackersOnWhite.begin(); itr != attackersOnWhite.end(); ++itr) {
        cout << itr->first << ", " << itr->second;
    }
}