#include "board.h"
#include "piece.cpp"
#include "helper.cpp"
#include <map>
#include <iostream>

void Board::generateMoves() {
    vector<pair<int, int>> checks = getChecks(moveColor);
    vector<pair<int, precomputedAttackerData>> precomputtedPossibleAttackers = getPossibleAttacker(moveColor);
    for(int pieceIndex = 0; pieceIndex < allPieces.size(); pieceIndex++) {
        char pieceSq = allPieces[pieceIndex];
        for(int attackerIdx = 0; attackerIdx < precomputtedPossibleAttackers.size(); attackerIdx++) {
            pair<int, precomputedAttackerData> preAtt = precomputtedPossibleAttackers[attackerIdx];
            int attStartSq = preAtt.first;
            precomputedAttackerData attData = preAtt.second;
        } 
    }
    
    if(checks.size() == 2) {
        //return only king moves that are not check
    } else if (checks.size() == 1) {
        //return all moves that block, check or king move that is not check after checking pinned pieces
    } else {
        //return all moves after checking pinned pieces
    }
}

void Board::generateSlidingMoves(int startSq, const char piece) {
    //need to test if pin and if between attack and pin
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

void Board::generateKingMoves(int startSq, const char piece) {
    int size = sizeof(kingMoves[startSq])/sizeof(int);
    for(int i = 0; i < size; i++) {
        int targetSquare = kingMoves[startSq][i];
        if(true) {//if is not check and target square does not hold piece of same color
            moves.push_back(Move(startSq, targetSquare));
        }
    }
}
void Board::generatePawnMoves(int startSq, const char piece) {
    if(isWhite(piece)) {
        int size = sizeof(whitePawnMoves[startSq])/sizeof(int);
        for(int i = 0; i < size; i++) {
            int targetSquare = whitePawnMoves[startSq][i];
            if(true) {//if is not pinned or stays between pin, need enpassant and double pawn move and capture and promotion logic here
                moves.push_back(Move(startSq, targetSquare));
            }
        }
    } else {
        int size = sizeof(blackPawnMoves[startSq])/sizeof(int);
        for(int i = 0; i < size; i++) {
            int targetSquare = blackPawnMoves[startSq][i];
            if(true) {//if is not pinned or stays between pin, need enpassant and double pawn move and capture and promotion logic here
                moves.push_back(Move(startSq, targetSquare));
            }
        }
    }
}
void Board::generateKnightMoves(int startSq, const char piece) {
    int size = sizeof(knightMoves[startSq])/sizeof(int);
    for(int i = 0; i < size; i++) {
        int targetSquare = knightMoves[startSq][i];
        if(true) {//if is not pinned and moves to square that holds piece of different color
            moves.push_back(Move(startSq, targetSquare));
        }
    }
}

void Board::isPiecePinned(int attSq, int pinSq) {

}

vector<pair<int, int>> Board::getChecks(const char color) {
    int king = getKing(color);
    vector<pair<int, int>> checks;
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
                pair<int, int> sq_piece = make_pair(startSq, boardPiece);
                checks.push_back(sq_piece);
            }
        }
    }
    return checks;
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


bool Board::makeMove(int startSq, int targetSq) {

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
                allPieces.insert(make_pair(rank * 8 + file, piece));
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

void Board::clearSquare(int squareIdx) {
    allPieces.erase(squareIdx);
    squares[squareIdx] = none;
}
void Board::addSquare(int sqaureIdx, int piece) {
    pair<int, int> addPair = make_pair(sqaureIdx, piece);
    allPieces.insert(addPair);
    squares[sqaureIdx] = piece;
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

void Board::setMoveColor() {
    if(moveColor == white) {
        moveColor = black;
    } else {
        moveColor = white;
    }
    return;
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

string Board::intToStringNotation(int startSq) {
    map<int, string>::iterator it;
    it = intToStringMap.find(startSq);
    if(it != intToStringMap.end()) {
        return it->second;
    }
    return "none";
}
int Board::stringToIntSquare(string notation) {
    map<string, int>::iterator it;
    it = stringToIntMap.find(notation);
    if(it != stringToIntMap.end()) {
        return it->second;
    }
    return -1;
}