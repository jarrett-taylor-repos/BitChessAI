#include "board.h"
#include "piece.cpp"
#include "helper.cpp"
#include <map>
#include <iostream>

void Board::generateMoves() {
    for(int startSq = 0; startSq < 64; startSq++) {
        const char piece = squares[startSq];
        if(isColor(piece, moveColor)) {
            if(isSlidingPiece(piece)) {
                generateSlidingMoves(startSq, piece);
            }
        }
    }
}

void Board::generateSlidingMoves(int startSq, const char piece) {
    int startDirIndex = isBishop(piece) ? 4 : 0;
    int endDirIndex = isRook(piece) ? 4 : 8; 

    for(int directionIndex = startDirIndex; directionIndex < endDirIndex; directionIndex++) {
        for(int n = 0; n < numSqauresToEdge[startSq][directionIndex]; n++) {
            int targetSq = startSq + directionOffSets[directionIndex] * (n+1);
            const char pieceOnTargetSq = squares[targetSq];

            if(isColor(piece, pieceOnTargetSq)) {
                break;
            }
            moves.push_back(Move(startSq, targetSq));
            if(!isColor(pieceOnTargetSq, moveColor)) {
                break;
            }
            
        }
    }
}




//in board
Board::Board(string s="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
    for(int i = 0; i < 64; i++) {
        squares[i] = none;
    }
    loadFen(s);
}

void Board::loadFen(string f="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
    map<char, const int> piece_map = {
        {'k', BlackKing()},
        {'q', BlackQueen()},
        {'r', BlackRook()},
        {'b', BlackBishop()},
        {'n', BlackKnight()},
        {'p', BlackPawn()},

        {'K', WhiteKing()},
        {'Q', WhiteQueen()},
        {'R', WhiteRook()},
        {'B', WhiteBishop()},
        {'N', WhiteKnight()},
        {'P', WhitePawn()}
    };

    vector<string> fen_info = split(f, " ");
    fen = fen_info[0];
    moveColor = fen_info[1] == "w" ? 0b10000 : 0b01000;
    castlingRights = fen_info[2];
    enPassantTarget = fen_info[3];
    halfTurnNum = stoi(fen_info[4]);
    fullTurnNum = stoi(fen_info[5]);

    int rank = 7, file = 0;
    for(auto ch : fen) {
        if(ch == '/') {
            file = 0;
            rank--;
        } else {
            if(isdigit(ch)) {
                file += (int) ch - '0';
            } else {
                const int piece = piece_map.find(ch)->second;
                squares[rank * 8 + file] = piece;
                file++;
            }
        }
    }
    return;
}

void Board::print() {
    for(int file = 7; file >= 0; file--) {
        for(int rank = 0; rank < 8; rank++) {
            int value = file*8 + rank;
            const char piece = squares[value];
            cout << pieceToString(piece) << " ";
        }
        cout << endl;
    }
}

string Board::pieceToString(const char p) {
    string s="";
    if(isNone(p)) {
        s = ".";
        return s;
    }
    if(isWhite(p)) {
        if(isPawn(p)) {
            s = "P";
        } else if (isQueen(p)) {
            s = "Q";
        } else if (isRook(p)) {
            s = "R";
        } else if (isBishop(p)) {
            s = "B";
        } else if (isKnight(p)) {
            s = "N";
        } else { //is king
            s = "K";
        }

    } else {
        if(isPawn(p)) {
            s = "p";
        } else if (isQueen(p)) {
            s = "q";
        } else if (isRook(p)) {
            s = "r";
        } else if (isBishop(p)) {
            s = "b";
        } else if (isKnight(p)) {
            s = "n";
        } else { //is king
            s = "k";
        }
    }
    return s;
}