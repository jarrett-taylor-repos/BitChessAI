#include "board.h"
#include "piece.cpp"
#include "helper.cpp"
#include <map>
#include <iostream>

void Board::generateMoves() {
    precomputedAttackerData kingAttackers;
    vector<pair<int, precomputedAttackerData>> attackersOnSquare;
    vector<pair<int, precomputedAttackerData>> allAttackersOnSquare = getPossibleAttacker(moveColor);
    for(int startSq = 0; startSq < 64; startSq++) {
        const char piece = squares[startSq];
        for(int i = 0; i < allAttackersOnSquare.size(); i++) {
            if(allAttackersOnSquare[i].first == startSq) {
                attackersOnSquare.push_back(allAttackersOnSquare[i]); 
            }
        }
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

void Board::getChecks(const char color) {
    int king = getKing(color);
    vector<pair<int, precomputedAttackerData>> possibleAttackersOnSquare = getPossibleAttacker(moveColor);
    for(int i = 0; i < possibleAttackersOnSquare.size(); i++) {
        precomputedAttackerData data = possibleAttackersOnSquare[i].second;
        int startSq = possibleAttackersOnSquare[i].first;
        int targetSq = data.getTargetSq();
        char dataPiece = data.getPiece();
        char boardPiece = squares[startSq];
        bool possiblePin = data.getPossiblePin();
        if(targetSq == king && dataPiece == boardPiece && possiblePin) {
            int numSquaresAway = data.getNumSquaresAway();
            bool noBlockers = noPiecesBetween(startSq, targetSq, numSquaresAway);
            if(noBlockers) {
                //check
            }
        }
        
    }
}

bool Board::noPiecesBetween(int startSq, int targetSq, int numSquaresAway) {
    if(abs(startSq-targetSq) < 10 || !isSlidingPiece(squares[startSq]) || numSquaresAway == 0) { //too close to have pieces between OR piece cant pin
        return true;
    }
    bool noneBetween = true;
    int diffIndex = (startSq-targetSq)/numSquaresAway;
    for(int i = startSq + diffIndex; i < targetSq; i += diffIndex) {
        if(!isNone(squares[i])) {
            noneBetween = false;
        }
    }
    return noneBetween;

}

int Board::getKing(const char color) {
    if(color == white) {
        return whiteKingSqaure;
    } 
    if(color == black) {
        return blackKingSqaure;
    }
    return -1;
}


vector<pair<int, precomputedAttackerData>> Board::getPossibleAttacker(char color) {
    if(color == white) {
        return attackersOnWhite;
    } else {
        return attackersOnBlack;
    }
}

Board::Board(string s="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
    for(int i = 0; i < 64; i++) {
        squares[i] = none;
    }
    loadFen(s);
}

void Board::loadFen(string f="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
    map<char, const char> piece_map = {
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
                const char piece = piece_map.find(ch)->second;
                squares[rank * 8 + file] = piece;
                if(ch == 'k') {
                    blackKingSqaure = rank * 8 + file;
                }
                if(ch == 'K') {
                    whiteKingSqaure = rank * 8 + file;
                }
                file++;
            }
        }
    }
    return;
}

Board::Board(const Board&) {

}
Board& Board::operator=(const Board&) {

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