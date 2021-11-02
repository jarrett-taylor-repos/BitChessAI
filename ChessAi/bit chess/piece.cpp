const char none = 0b00000;
const char pawn = 0b00001;
const char knight = 0b00010;
const char bishop = 0b00011;
const char rook = 0b00100;
const char queen = 0b00101;
const char king = 0b00110;

const char black = 0b01000;
const char white = 0b10000;

//NONE
const char None() {
    return none;
}

//WHITE PIECES
const char WhitePawn() {
    return (white | pawn); 
}
const char WhiteKnight() {
    return (white | knight); 
}
const char WhiteBishop() {
    return (white | bishop); 
}
const char WhiteRook() {
    return (white | rook); 
}
const char WhiteQueen() {
    return (white | queen); 
}
const char WhiteKing() {
    return (white | king); 
}

//BLACK PIECES
const char BlackPawn() {
    return (black | pawn); 
}
const char BlackKnight() {
    return (black | knight); 
}
const char BlackBishop() {
    return (black | bishop); 
}
const char BlackRook() {
    return (black | rook); 
}
const char BlackQueen() {
    return (black | queen); 
}
const char BlackKing() {
    return (black | king); 
}


//TEST PIECE COLOR AND TYPE
const char getPieceValue(const char p) {
    char value = isWhite(p) ? p - 16 : p - 8;
    return value;
}

bool isWhite(const char p) {
    return (p - white >= 0b0);
}
bool isBlack(const char p) {
    return !isWhite(p);
}

bool isKing(const char p) {
    return (getPieceValue(p) == 0b00110);
}
bool isQueen(const char p) {
    return (getPieceValue(p) == 0b00101);
}
bool isRook(const char p) {
    return (getPieceValue(p) == 0b00100);
}
bool isBishop(const char p) {
    return (getPieceValue(p) == 0b00011);
}
bool isKnight(const char p) {
    return (getPieceValue(p) == 0b00010);
}
bool isPawn(const char p) {
    return (getPieceValue(p) == 0b00001);
}
bool isNone(const char p) {
    return (p == 0b0);
}

bool isColor(const char a, const char b) {
    return (a >> 3) == (b >> 3);
}

bool isSlidingPiece(const char p) {
    return (isRook(p) || isBishop(p) || isQueen(p));
}


//calulate moves potentials
const int directionOffSets[8] = {8, -8, -1, 1, 7, -7, 9, -9};
int** precomputedMoveData() {
    int* numSqauresToEdge[64];

    for(int file = 0; file < 8; file++) {
        for(int rank = 0; rank < 8; rank++) {
            int numNorth = 7-rank;
            int numSouth = rank;
            int numWest = file;
            int numEast = 7-file;
            int sqIndex = rank * 8 + file;
            int directionArray[8] = {
                numNorth, 
                numSouth, 
                numWest, 
                numEast,
                min(numNorth, numWest),
                min(numSouth, numEast),
                min(numNorth, numEast),
                min(numSouth, numWest)
            };
            numSqauresToEdge[sqIndex] = directionArray;
        }
    }
    return numSqauresToEdge;
}

int** numSqauresToEdge = precomputedMoveData();