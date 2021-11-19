#include "board.cpp"

int main() {
    init();
    Board b;
    b.print();
    UciMove ucim("e2", "e4");
    b.makeMove(ucim);
    b.print();
}