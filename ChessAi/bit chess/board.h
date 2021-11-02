#ifndef BOARD_H
#define BOARD_H
#include "move.cpp"
#include <string>
#include <vector>

class Board {
    private:
        char squares[64];
        std::string fen;
        std::string castlingRights;
        int halfTurnNum;
        int fullTurnNum;
        std::string enPassantTarget;
        int moveColor;
        
        std::vector<Move> moves;

        int whiteKingSqaure;
        int blackKingSqaure;
    public:
        Board(std::string);
        void loadFen(std::string);
        void generateMoves();
        void generateSlidingMoves(int, const char);

        void print();
        std::string pieceToString(const char);
};


#endif