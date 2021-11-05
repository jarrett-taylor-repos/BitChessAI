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
        std::vector<precomputedAttackerData> attackers;

        int whiteKingSqaure;
        int blackKingSqaure;
        int* pinnedSquares;
        map<int,char> allPieces;
    public:
        Board(std::string);
        Board(const Board&);
        Board& operator=(const Board&);
        void loadFen(std::string);
        void clearSquare(int);
        void addSquare(int, char);
        bool makeMove(int, int);
        void setAttackers();
        void generateMoves();
        void generateSlidingMoves(int, const char);
        void generateKingMoves(int startSq, const char piece);
        void generatePawnMoves(int startSq, const char piece);
        void generateKnightMoves(int startSq, const char piece);
        void setAttackers(int startSq);
        void isPiecePinned(int attSq, int pinSq);
        vector<pair<int, int>> getChecks(const char);
        bool noPiecesBetween(int, int, int);
        int getKing(const char);
        vector<pair<int, precomputedAttackerData>> getPossibleAttacker(char color);
        void print();
        void setMoveColor();
        std::string pieceToString(const char);
        std::string intToStringNotation(int);
        int stringToIntSquare(std::string);
};


#endif