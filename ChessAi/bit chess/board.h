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
        char moveColor;
        
        std::vector<Move> moves;

        int whiteKingSqaure;
        int blackKingSqaure;
    public:
        Board(std::string);
        Board(const Board&);
        Board& operator=(const Board&);
        void loadFen(std::string);
        void generateMoves();
        void generateSlidingMoves(int, const char);
        void getChecks(const char);
        bool noPiecesBetween(int, int, int);
        int getKing(const char);
        vector<pair<int, precomputedAttackerData>> getPossibleAttacker(char color);
        void print();
        std::string pieceToString(const char);
};


#endif