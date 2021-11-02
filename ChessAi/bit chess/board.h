#ifndef BOARD_H_
#define BOARD_H_
#include <string>

class Board {
    private:
        int* squares;
        std::string fen;
        std::string castlingRights;
        int halfTurnNum;
        int fullTurnNum;
        std::string enPassantTarget;
        int moveColor;
        
        vector<Move> moves;
    public:
        Board(std::string);
        void loadFen(std::string);
        vector<Move> generateMoves();
};


#endif BOARD_H_