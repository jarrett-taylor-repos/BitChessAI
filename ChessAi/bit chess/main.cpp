#include "board.cpp"

int main() {
    init();
    Board b;
    b.printAttackers();
    multimap<int,Attacker> atts = b.getAttackers();
    multimap<int, Attacker>::iterator itr;
    for (int i = 0; i < 64; i++) {
        int key = i;
        for (auto itr = atts.begin(); itr != atts.end(); itr++) {
            if (itr->first == key) {
                cout << itr->first << "  " << itr->second.getTargetSq() << endl;
            }
        }
    }
    cout << atts.size();
}