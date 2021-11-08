#include "board.h"
#include "helper.cpp"

void Board::setAttackers() {
    multimap<int, precomputedAttackerData> precomputtedPossibleAttackers = getPossibleAttacker(moveColor);
    for(int pieceIndex = 0; pieceIndex < allPieces.size(); pieceIndex++) {
        int square = allPieces[pieceIndex];
        char piece = squares[square];
        if(!isColor(piece, moveColor)) {
            //set attackling squares
        
            
        }
    }
    //for all squares that have black pieces on startsq
    //get all precompuuted attackers for squares where the startSq and piece are the same
}

void Board::generateMoves() {
    vector<pair<int, int>> checks = getChecks(moveColor);
    
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


vector<pair<int, int>> Board::getChecks(const char color) {
    int king = getKing(color);
    vector<pair<int, int>> checks;
    return checks;
}

bool Board::noPiecesBetween(int startSq, int targetSq, int numSquaresAway) {
    if(abs(startSq-targetSq) < 10 || numSquaresAway < 2) {
        return true;
    }
    int total_diff = targetSq - startSq;
    int diffIndex;
    if (total_diff % 7 == 0) { diffIndex = 7; }
    else if (total_diff % 1 == 0) { diffIndex = 1; }
    else if (total_diff % 8 == 0) { diffIndex = 8; }
    else if (total_diff % 9 == 0) { diffIndex = 9; }
    else {
        diffIndex = abs(total_diff / numSquaresAway);
    }
    int start = (startSq > targetSq) ? targetSq : startSq;
    int end = (startSq == start) ? targetSq : startSq;
    for(int i = start + diffIndex; i < end; i += diffIndex) {
        if(!isNone(squares[i])) {
            return false;
        }
    }
    return true;

}

bool Board::isPiecePinned(int attSq, int pinSq) {
     //piece is pinned if king is behind piece and 
    char att = squares[attSq];
    char pin = squares[pinSq];
    int kingSq = getKing(moveColor);
    char king = squares[kingSq];
    bool nullPiece = isNone(att) || isNone(pin);
    bool oppositePiece = !isColor(att, pin) && isColor(pin, king) && !isColor(king, att); //test to see if pin and attacker are diff color, test if pin and sking are same color, test if king and attacker are opposite color
    bool possiblePinner = isSlidingPiece(att);
    if (nullPiece || !oppositePiece || !possiblePinner) {
        return false;
    }
    int total_diff = attSq - pinSq;
    int diff; 
    if (total_diff % 7 == 0) { diff = 7; }
    else if (total_diff % 1 == 0) { diff = 1; }
    else if (total_diff % 8 == 0) { diff = 8; }
    else if(total_diff % 9 == 0) { diff = 9; }
    else { return false; }

    if (total_diff > 0) { diff *= -1; }

    int testSq = pinSq + diff;
    bool kingBehindPin = false;
    int numAwayAtt = abs(total_diff/diff);
    int pintoKing = (pinSq-kingSq)/diff*-1;
    while (testSq >= 0 && testSq < 64) {
        bool kingTester = (testSq == kingSq);
        if (kingTester) {
            kingBehindPin = true;
            break;
        }
        testSq += diff;
    }
    if (!kingBehindPin) {
        return false;
    }
    bool noneBetween = noPiecesBetween(attSq, pinSq, numAwayAtt) && noPiecesBetween(pinSq, kingSq, pintoKing);
    if (noneBetween) {
        return true;
    }
    return false;

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

multimap<int, precomputedAttackerData> Board::getPossibleAttacker(char color) {
    if(color == white) {
        return attackersOnWhite;
    } else {
        return attackersOnBlack;
    }
}


bool Board::makeMove(string start, string target) {
    int startSq = stringToIntSquare(start);
    int targetSq = stringToIntSquare(target);
    if(startSq == -1 || targetSq == -1) {
        cerr << start << target << " -> Illegal move." << endl;
        return false;
    }

    bool moveInMoves = false;
    for(auto m : moves) {
        Move testMove = Move(startSq, targetSq);
        if(testMove == m) {
            moveInMoves = true;
            break;
        }
    }

    if(!moveInMoves) {
        cerr << start << target << " -> Illegal move." << endl;
        return false;
    }



}

//CONSTRUCTORS AND OPERATORS
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


Board::Board(const Board&) {

}
Board& Board::operator=(const Board&) {

    return *this;
}


void Board::clearSquare(int squareIdx) {
    allPieces.erase(squareIdx);
    squares[squareIdx] = none;
}
void Board::addSquare(int sqaureIdx, char piece) {
    pair<int, char> addPair = make_pair(sqaureIdx, piece);
    allPieces.insert(addPair);
    squares[sqaureIdx] = piece;
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

char Board::getMoveColor() {
    return moveColor;
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
    if(isNone(p) || (!isWhite(p) && !isBlack(p)) ) {
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

char* Board::getSquares() {
    return squares;
}

map<int, char> Board::getAllPieces() {
    return allPieces;
}