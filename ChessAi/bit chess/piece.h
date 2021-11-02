#ifndef PIECE_H_
#define PIECE_H_

struct Piece {
    //piece is denotes by 5 bits
    //  first 2 denote color, last 3 denote piece
    //    [cc][ppp] 

    // 00000 = 0 - none
    // xx001 = 1 - pawn
    // xx010 = 2 - knight
    // xx011 = 3 - bishop
    // xx100 = 4 - rook
    // xx101 = 5 - queen
    // xx110 = 6 - king
    // 01xxx = 8 - black
    // 10xxx = 16 - white
    private:
        const char none = 0b00000;
        const char pawn = 0b00001;
        const char knight = 0b00010;
        const char bishop = 0b00011;
        const char rook = 0b00100;
        const char queen = 0b00101;
        const char king = 0b00110;

        const char black = 0b01000;
        const char white = 0b10000;
    
    public:
        const char None();

        const char WhitePawn();
        const char WhiteKnight();
        const char WhiteBishop();
        const char WhiteRook();
        const char WhiteQueen();
        const char WhiteKing();

        const char BlackPawn();
        const char BlackKnight();
        const char BlackBishop();
        const char BlackRook();
        const char BlackQueen();
        const char BlackKing();

        const char getPieceValue(const char p);

        bool isWhite(const char);
        bool isBlack(const char);

        bool isKing(const char);
        bool isQueen(const char);
        bool isRook(const char);
        bool isBishop(const char);
        bool isKnight(const char);
        bool isPawn(const char);
        bool isNone(const char);

        bool isColor(const char, const char);
};

#endif PIECE_H_