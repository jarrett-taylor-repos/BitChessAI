#ifndef BOARD_H
#define BOARD_H
#include "move.cpp"
#include "attacker.cpp"
#include "piece.cpp"
#include "UciMove.cpp"
#include <string>
#include <map>
#include <vector>
using std::vector;
using std::map;
using std::multimap;
using std::string;
using std::pair;

class Board {
    private:
        char squares[64];
        string fen;
        string castlingRights;
        int halfTurnNum;
        int fullTurnNum;
        string enPassantTarget;
        char moveColor;
        
        vector<Move> moves;
        vector<UciMove> ucimoves;
        multimap<int, Attacker> attackers;
        vector<int> attackedSquares;

        int whiteKingSqaure;
        int blackKingSqaure;
        map<int,char> allPieces;
        map<string, int> fenMap;

        bool wasCapture;
        bool wasPromo;
        bool wasCastle;
        bool wasEnpassant;
        int turnNum;
    public:
        Board(string);
        Board(const Board&);
        Board& operator=(const Board&);
        void loadFen(string);


        void clearSquare(int);
        void addSquare(int, char);
        void addAttacker(Attacker, int);
        void clearAttackers();
        void clearSquares();
        bool isSquareAttacked(int);
        multimap<int, Attacker> getAttackers();

        bool isGameover();
        bool isCheckMate();
        bool isDraw();
        bool noForcedMateDraw();
        bool repitionDraw();

        bool makeMove(UciMove);

        void setAttackers();
        void slidingAttackers(int, char);
        void pieceAttacker(int, char);

        void generateMoves();
        void generateMoveHelper(int, const char, int);
        void generateSlidingMoves(int, const char, int);
        void generateKingMoves(int);
        void generatePawnMoves(int, int);
        void generateKnightMoves(int);
        vector<UciMove> getUciMoves();

        bool isPiecePinned(int, int);
        vector<pair<int, int>> getChecks();
        int getPinningPieceIndex(int);
        bool noPiecesBetween(int, int, int);
        bool noPiecesBetweenNotKing(int, int, int);
        int getKing(const char);

        char getMoveColor();
        void setMoveColor();

        string pieceToString(const char);
        char charToPiece(char);
        string intToStringNotation(int);
        int stringToIntSquare(string);

        char* getSquares();
        map<int, char> getAllPieces();
        void clearAllPieces();
        void clearMoves();
        void setInit();
        void print();
        void printAttackers();
        void setfenMap();
        void setFen();
        string getFen();
        
        UciMove moveToUci(Move m, string);
        Move uciToMove(UciMove ucim);
};


#endif