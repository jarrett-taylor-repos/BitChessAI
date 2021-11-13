#include "board.cpp"

int main() {
    init();
    Board b;
    string test2 = "4k3/4r3/8/8/8/4K3/7R/8 w - - 0 1";
    b.loadFen(test2);
    b.printAttackers();
    multimap<int,Attacker> atts = b.getAttackers();
    vector<pair<int, int>> checks = b.getChecks();
    char* boardSquares = b.getSquares();
    for(int i = 0; i < checks.size(); i++) {
        int start = checks[i].first;
        int end = checks[i].second;
        char startP = boardSquares[start];
        char endP = boardSquares[end];
        cout << b.pieceToString(startP) << " - " << start << " checks " <<  b.pieceToString(endP) << " - " << end << endl;
    }
}