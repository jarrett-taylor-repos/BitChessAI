#include "piece.h"


//NONE
const char Piece::None() {
    return none;
}

//WHITE PIECES
const char Piece::WhitePawn() {
    return (white | pawn); 
}
const char Piece::WhiteKnight() {
    return (white | knight); 
}
const char Piece::WhiteBishop() {
    return (white | bishop); 
}
const char Piece::WhiteRook() {
    return (white | rook); 
}
const char Piece::WhiteQueen() {
    return (white | queen); 
}
const char Piece::WhiteKing() {
    return (white | king); 
}

//BLACK PIECES
const char Piece::BlackPawn() {
    return (black | pawn); 
}
const char Piece::BlackKnight() {
    return (black | knight); 
}
const char Piece::BlackBishop() {
    return (black | bishop); 
}
const char Piece::BlackRook() {
    return (black | rook); 
}
const char Piece::BlackQueen() {
    return (black | queen); 
}
const char Piece::BlackKing() {
    return (black | king); 
}


//TEST PIECE COLOR AND TYPE
const char Piece::getPieceValue(const char p) {
    char value = isWhite(p) ? p - 16 : p - 8;
    return value;
}

bool Piece::isWhite(const char p) {
    return (p - white >= 0b0);
}
bool Piece::isBlack(const char p) {
    return !isWhite(p);
}

bool Piece::isKing(const char p) {
    return (getPieceValue(p) == 0b00110);
}
bool Piece::isQueen(const char p) {
    return (getPieceValue(p) == 0b00101);
}
bool Piece::isRook(const char p) {
    return (getPieceValue(p) == 0b00100);
}
bool Piece::isBishop(const char p) {
    return (getPieceValue(p) == 0b00011);
}
bool Piece::isKnight(const char p) {
    return (getPieceValue(p) == 0b00010);
}
bool Piece::isPawn(const char p) {
    return (getPieceValue(p) == 0b00001);
}
bool Piece::isNone(const char p) {
    return (p == 0b0);
}

bool Piece::isColor(const char a, const char b) {
    return (a >> 3) == (b >> 3);
}