#ifndef BOARD_H
#define BOARD_H
#include "move.cpp"
#include "attacker.cpp"
#include "piece.cpp"
#include <string>
#include <map>
#include <vector>
using std::vector;
using std::map;
using std::multimap;
using std::string;

class Board {
    private:
        char squares[64];
        std::string fen;
        std::string castlingRights;
        int halfTurnNum;
        int fullTurnNum;
        std::string enPassantTarget;
        char moveColor;
        
        vector<Move> moves;
        std::multimap<int, Attacker> attackers;

        int whiteKingSqaure;
        int blackKingSqaure;
        map<int,char> pinnedSquares;
        map<int,char> allPieces;
    public:
        Board(std::string);
        Board(const Board&);
        Board& operator=(const Board&);
        void loadFen(std::string);
        void clearSquare(int);
        void addSquare(int, char);
        void addAttacker(Attacker, int);
        void clearAttackers();
        void clearSquares();
        multimap<int, Attacker> getAttackers();
        bool makeMove(std::string, std::string);
        void setAttackers();
        void slidingAttackers(int, char);
        void pieceAttacker(int startSq, char piece);
        void generateMoves();
        void generateSlidingMoves(int, const char);
        void generateKingMoves(int, const char);
        void generatePawnMoves(int, const char);
        void generateKnightMoves(int, const char);
        bool isPiecePinned(int, int);
        vector<std::pair<int, int>> getChecks();
        bool noPiecesBetween(int, int, int);
        bool noPiecesBetweenNotKing(int, int, int);
        int getKing(const char);
        char getMoveColor();
        void setMoveColor();
        std::string pieceToString(const char);
        std::string intToStringNotation(int);
        int stringToIntSquare(std::string);
        char* getSquares();
        map<int, char> getAllPieces();
        void clearAllPieces();
        void setInit();
        void print();
        void printAttackers();
};


#endif