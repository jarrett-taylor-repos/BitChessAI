#include "board.cpp"

int main() {
    init();
    Board b;
    string test2 = "4k3/4r3/8/8/8/4K3/7R/8 w - - 0 1";
    b.loadFen(test2);
    b.print();
    
}