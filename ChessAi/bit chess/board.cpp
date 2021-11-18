#include "board.h"
#include "helper.cpp"
#include <algorithm>
#include <windows.h>
#include <stdlib.h>


void Board::setAttackers() {
    for(int pieceIndex = 0; pieceIndex < allPieces.size(); pieceIndex++) {
        int piece = allPieces[pieceIndex];
        if(!isColor(piece, moveColor) && !isNone(piece)) {
            if (isSlidingPiece(piece)) {
                slidingAttackers(pieceIndex, piece);
            }
            else {
                pieceAttacker(pieceIndex, piece);
            }
        }
    }
    return;
}
void Board::slidingAttackers(int startSq, char piece) {
    int startDirIndex = isBishop(piece) ? 4 : 0;
    int endDirIndex = isRook(piece) ? 4 : 8;

    for (int directionIndex = startDirIndex; directionIndex < endDirIndex; directionIndex++) {
        for (int n = 0; n < numSqauresToEdge[startSq][directionIndex]; n++) {
            int targetSq = startSq + directionOffSets[directionIndex] * (n + 1);
            const char pieceOnTargetSq = squares[targetSq];

            if (isColor(piece, pieceOnTargetSq)) {
                Attacker add(startSq, targetSq, piece);
                addAttacker(add, startSq);
                break;
            }
            Attacker add(startSq, targetSq, piece);
            addAttacker(add, startSq);
            if (!isColor(pieceOnTargetSq, moveColor) && !isKing(pieceOnTargetSq) && !isNone(pieceOnTargetSq)) {
                break;
            }
        }
    }
    return;
}
void Board::pieceAttacker(int startSq, char piece) {
    if (isKing(piece)) {
        int size = sizeof(kingMoves[startSq])/sizeof(int);
        for (int i = 0; i < size; i++) {
            int targetSquare = kingMoves[startSq][i];
            if (targetSquare != startSq) {//if is not check and target square does not hold piece of same color
                Attacker add(startSq, targetSquare, piece);
                addAttacker(add, startSq);
            }
        }
    }
    else if (isPawn(piece)) {
        if (!isWhite(moveColor)) {
            int size = sizeof(whiteAttackingPawnMoves[startSq]) / sizeof(int);
            for (int i = 0; i < size; i++) {
                int targetSquare = whiteAttackingPawnMoves[startSq][i];
                if (targetSquare != startSq) {//if is not pinned or stays between pin, need enpassant and double pawn move and capture and promotion logic here
                    Attacker add(startSq, targetSquare, piece);
                    addAttacker(add, startSq);
                }
            }
        }
        else {
            int size = sizeof(blackAttackingPawnMoves[startSq]) / sizeof(int);
            for (int i = 0; i < size; i++) {
                int targetSquare = blackAttackingPawnMoves[startSq][i];
                if (targetSquare != startSq) {//if is not pinned or stays between pin, need enpassant and double pawn move and capture and promotion logic here
                    Attacker add(startSq, targetSquare, piece);
                    addAttacker(add, startSq);
                }
            }
        }
    }
    else if (isKnight(piece)) {
        int size = sizeof(knightMoves[startSq]) / sizeof(int);
        for (int i = 0; i < size; i++) {
            int targetSquare = knightMoves[startSq][i];
            if (targetSquare != startSq) {//if is not check and target square does not hold piece of same color
                Attacker add(startSq, targetSquare, piece);
                addAttacker(add, startSq);
            }
        }
    }
    return;
}


vector<UciMove> Board::getUciMoves() {
    return ucimoves;
}

void Board::generateMoves() {
    moves.clear();
    ucimoves.clear();
    setAttackers();
    vector<int> removeMoveIndex;
    vector<pair<int, int>> checks = getChecks();
    if(checks.size() == 2) {
        //return only king moves that are not check
        generateKingMoves(getKing(moveColor));
    } else if (checks.size() == 1) {
        //return all moves that block, check or king move that is not check after checking pinned pieces
        int checkStartSq = checks[0].first;
        char checkingPiece = squares[checkStartSq];
        int checkTargetSq = checks[0].second;
        for (int i = 0; i < allPieces.size(); i++) {
            int pieceIndex = allPieces[i];
            const char piece = squares[pieceIndex];
            if (!isNone(piece)) {//piece is pinned moves
                int pinningPieceIdx = getPinningPieceIndex(pieceIndex);
                generateMoveHelper(pieceIndex, piece, pinningPieceIdx);
            }
        }
        //now need to see if the move is legal in terms of check
        for (int i = 0; i < ucimoves.size(); i++) {
            UciMove ucim = ucimoves[i];
            Move m = uciToMove(ucim);
            int mstartsq = m.getStartSq();
            char mstartpiece = squares[mstartsq];
            int mtargetsq = m.getTargetSq();
            //tets if blocks check(to block, but be more than a square away and attacker must be sliding piece)
            bool checkCanBeBlocked = (checkStartSq - checkTargetSq > 10) && isSlidingPiece(checkingPiece);
            bool capture = (mtargetsq == checkStartSq);
            bool isKingMove = isKing(mstartpiece);
            bool isBlockingMove = false;
            if (checkCanBeBlocked) {
                //if piece moves between check and king
                int diffOffsetKingToAtt = getDifferenceOffset(checkTargetSq, checkStartSq); //from king to attacker
                int diffOffsetTargetToAtt = getDifferenceOffset(mtargetsq, checkStartSq); //from target piece to attacker
                int diffOffsetKingToTarget = getDifferenceOffset(checkTargetSq, mtargetsq); //from king to target piece
                isBlockingMove = (diffOffsetKingToAtt == diffOffsetTargetToAtt) && (diffOffsetTargetToAtt == diffOffsetKingToTarget);
            }
            if (!(isBlockingMove || capture || isKingMove)) {//if piece captures, blocks, or king moves (IF NOT, store index)
                //store index
                removeMoveIndex.push_back(i);
            }
        }
        //remove moves of bad index
        for (int i = removeMoveIndex.size(); i >= 0; i--) { //remove all indexes, starting from back and all moves are noe generated
            ucimoves.erase(ucimoves.begin() + i);
        }
    } else {
        //return all moves after checking pinned pieces
        for (int i = 0; i < allPieces.size(); i++) {
            int pieceIndex = allPieces[i];
            const char piece = squares[pieceIndex];
            if (!isNone(piece)) {//piece is pinned moves
                int pinningPieceIdx = getPinningPieceIndex(pieceIndex);
                generateMoveHelper(pieceIndex, piece, pinningPieceIdx);
            }
        }
    }
    return;
}

int Board::getPinningPieceIndex(int startSq) {
    vector<pair<int, int>> attackersOnSq;
    int pinningPieceIdx = -1;
    for (auto it = attackers.begin(); it != attackers.end(); ++it) {
        Attacker currAtt = it->second;
        int startSq = it->first;
        int targetSq = currAtt.getTargetSq();
        if (targetSq == startSq) {
            pair<int, int> startToTargetSq = make_pair(startSq, targetSq);
            attackersOnSq.push_back(startToTargetSq);
        }
    }

    for (int j = 0; j < attackersOnSq.size(); j++) {
        int attackerIdx = attackersOnSq[j].first;
        if (isPiecePinned(attackerIdx, startSq)) {
            pinningPieceIdx = attackerIdx;
            break;
        }
    }

    return pinningPieceIdx;
}

void Board::generateMoveHelper(int startSq, const char piece, int pinningPieceIdx) {
    bool isPin = pinningPieceIdx != -1;
    if(isSlidingPiece(piece)) {
        generateSlidingMoves(startSq, piece, pinningPieceIdx);
    }
    else if (isKing(piece) && !isPin) {//king cant be pinned
        generateKingMoves(startSq);
    }
    else if (isKnight(piece) && !isPin) {//if pinned, unable to move
        generateKingMoves(startSq);
    }
    else if (isPawn(piece)) {
        generatePawnMoves(startSq, pinningPieceIdx);
    }
    return;
}
void Board::generateSlidingMoves(int startSq, const char piece, int pinningPieceIdx) {
    //need to test if pin and if between attack and pin
    bool isPin = pinningPieceIdx != -1;
    int kingIndex = getKing(moveColor);
    int diffOffsetKingToAtt = getDifferenceOffset(kingIndex, pinningPieceIdx);

    int startDirIndex = isBishop(piece) ? 4 : 0;
    int endDirIndex = isRook(piece) ? 4 : 8; 

    for(int directionIndex = startDirIndex; directionIndex < endDirIndex; directionIndex++) {
        for(int n = 0; n < numSqauresToEdge[startSq][directionIndex]; n++) {
            int targetSq = startSq + directionOffSets[directionIndex] * (n+1);
            const char pieceOnTargetSq = squares[targetSq];

            //test to see if the piece stays bewteen attacker and king

            if (isPin) {
                int diffOffsetTargetToAtt = getDifferenceOffset(targetSq, pinningPieceIdx);
                int diffOffsetKingToTarget = getDifferenceOffset(kingIndex, targetSq);
                bool legalPinnedMove = (diffOffsetTargetToAtt == diffOffsetKingToAtt) && (diffOffsetTargetToAtt == diffOffsetKingToTarget);
                if(!legalPinnedMove) {
                    break;
                }
            }

            if(isColor(piece, pieceOnTargetSq)) {
                break;
            }
            Move m(startSq, targetSq);
            moves.push_back(m);
            UciMove ucim = moveToUci(m, "");
            ucimoves.push_back(ucim);
            if(!isColor(pieceOnTargetSq, moveColor) && !isNone(pieceOnTargetSq)) {
                break;
            }
        }
    }
    return;
}
void Board::generateKingMoves(int startSq) { //need to add castling
    int size = sizeof(kingMoves[startSq])/sizeof(int);
    const char piece = squares[startSq];
    for(int i = 0; i < size; i++) {
        int targetSquare = kingMoves[startSq][i];
        const char targetPiece = squares[targetSquare];
        bool isAttacked = find(attackedSquares.begin(), attackedSquares.end(), targetSquare) != attackedSquares.end();
        bool emptyorOppositeColorSquare = isNone(targetPiece) || !isColor(piece, targetPiece);
        if(!isAttacked && emptyorOppositeColorSquare) {//if is not check and target square does not hold piece of same color
            Move m(startSq, targetSquare);
            moves.push_back(m);
            UciMove ucim = moveToUci(m, "");
            ucimoves.push_back(ucim);
        }
    }
    //need castling here
    if (isWhite(moveColor)) {
        const int whiteKing = 4;
        if (castlingRights.find('K') != std::string::npos) {
            const char sq5 = squares[5];
            const char sq6 = squares[6];            bool canCastleShort = 
                isNone(sq5) && isNone(sq6) &&
                !isSquareAttacked(whiteKing) && !isSquareAttacked(5) && !isSquareAttacked(6);
            if (canCastleShort) {
                Move m(4, 6);
                UciMove ucim = moveToUci(m, "");
                moves.push_back(m);
                ucimoves.push_back(ucim);
            }
        }
        if (castlingRights.find('Q') != std::string::npos) {
            const char sq1 = squares[1];
            const char sq2 = squares[2];
            const char sq3 = squares[3];
            bool canCastleLong =
                isNone(sq1) && isNone(sq2) && isNone(sq3);
                !isSquareAttacked(whiteKing) && !isSquareAttacked(2) && !isSquareAttacked(3);
            if (canCastleLong) {
                Move m(4, 2);
                UciMove ucim = moveToUci(m, "");
                moves.push_back(m);
                ucimoves.push_back(ucim);
            }
        }
    }
    else {
        const int blackKing = 60;
        if (castlingRights.find('k') != std::string::npos) {
            const char sq61 = squares[61];
            const char sq62 = squares[62];
            bool canCastleShort = 
                isNone(sq61) && isNone(sq62) &&
                !isSquareAttacked(blackKing) && !isSquareAttacked(61) && !isSquareAttacked(62);
            if (canCastleShort) {
                Move m(60, 62);
                UciMove ucim = moveToUci(m, "");
                moves.push_back(m);
                ucimoves.push_back(ucim);
            }
        }
        if (castlingRights.find('q') != std::string::npos) {
            const char sq59 = squares[59];
            const char sq58 = squares[58];
            const char sq57 = squares[57];
            bool canCastleLong = 
                isNone(sq59) && isNone(sq58) && isNone(sq57) &&
                !isSquareAttacked(blackKing) && !isSquareAttacked(59) && !isSquareAttacked(58);
            if (canCastleLong) {
                Move m(60, 58);
                UciMove ucim = moveToUci(m, "");
                moves.push_back(m);
                ucimoves.push_back(ucim);
            }
        }
    }
    return;
}
void Board::generatePawnMoves(int startSq, int pinningPieceIdx) {
    vector<bool> pawnMovesBool = { false, false, false, false }; //up1, up2, capleft, capright [0-3]
    vector<string> promos = {"Q", "R", "B", "N" };
    const char startSqPiece = squares[startSq];
    int kingIndex = getKing(moveColor);
    int diffOffsetKingToAtt = getDifferenceOffset(kingIndex, pinningPieceIdx);
    bool isPin = pinningPieceIdx != -1;
    int pawnMoves[64][4];
    if (isWhite(moveColor)) {
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 4; j++) {
                pawnMoves[i][j] = whitePawnMoves[i][j];
            }
        }
    }
    else {
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 4; j++) {
                pawnMoves[i][j] = blackPawnMoves[i][j];
            }
        }
    }

    int size = sizeof(pawnMoves[startSq]) / sizeof(int);
    for (int i = 0; i < size; i++) {
        int targetSquare = pawnMoves[startSq][i];
        const char targetSqPiece = squares[targetSquare];
        int diffOffsetTargetToAtt = getDifferenceOffset(targetSquare, pinningPieceIdx);
        int diffOffsetKingToTarget = getDifferenceOffset(kingIndex, targetSquare);
        bool legalPinnedMove = (diffOffsetTargetToAtt == diffOffsetKingToAtt) && (diffOffsetTargetToAtt == diffOffsetKingToTarget);
        bool canMoveWithPin = !isPin ? true : legalPinnedMove;
        bool pieceMoves = (targetSquare != startSq);
        if (canMoveWithPin && pieceMoves) {
            if (i == 0) {
                pawnMovesBool[i] = isNone(targetSqPiece);
            }
            else if (i == 1) {
                bool noPieceBetweenPawnStartAndTarget = noPiecesBetween(startSq, targetSquare, 2);
                pawnMovesBool[i] = isNone(targetSqPiece) && noPieceBetweenPawnStartAndTarget;
            }
            else if (i == 2) {
                bool canCapture = !isNone(targetSqPiece) && !isColor(targetSqPiece, startSqPiece);
                int enpassantTargetSq = stringToIntSquare(enPassantTarget);
                bool enpassant = enpassantTargetSq == targetSquare;
                pawnMovesBool[i] = (canCapture || enpassantTargetSq);
            }
            else if (i == 3) {
                bool canCapture = !isNone(targetSqPiece) && !isColor(targetSqPiece, startSqPiece);
                int enpassantTargetSq = stringToIntSquare(enPassantTarget);
                bool enpassant = enpassantTargetSq == targetSquare;
                pawnMovesBool[i] = (canCapture || enpassantTargetSq);
            }
        }
        //now add moves
        bool addMove = pawnMovesBool[i];
        if (addMove) {
            Move m(startSq, targetSquare);
            moves.push_back(m);
            bool promotion = isWhite(moveColor) ? targetSquare > 55 : targetSquare < 8;
            if (promotion) {
                for (int j = 0; j < promos.size(); j++) {
                    string promoStr = promos[j];
                    UciMove uciMoveAdd = moveToUci(m, promoStr);
                    ucimoves.push_back(uciMoveAdd);
                }
            }
            else {
                UciMove uciMoveAdd = moveToUci(m, "");
                ucimoves.push_back(uciMoveAdd);
            }
        }
    }
    return;
}
void Board::generateKnightMoves(int startSq) {
    int size = sizeof(knightMoves[startSq])/sizeof(int);
    for(int i = 0; i < size; i++) {
        int targetSquare = knightMoves[startSq][i];
        const char targetSqPiece = squares[targetSquare];
        const char piece = squares[startSq];
        bool validMove = targetSquare != startSq;
        bool emptyorOppositeColorSquare = isNone(targetSqPiece) || !isColor(piece, targetSqPiece);
        if(validMove && emptyorOppositeColorSquare) {//if is not pinned and moves to square that holds piece of different color
            Move m(startSq, targetSquare);
            moves.push_back(m);
            UciMove ucim = moveToUci(m, "");
            ucimoves.push_back(ucim);
        }
    }
    return;
}


vector<pair<int, int>> Board::getChecks() {
    int king = getKing(moveColor);
    vector<pair<int, int>> checks;
    for (auto it = attackers.begin(); it != attackers.end(); ++it) {
        Attacker currAtt = it->second;
        int startSq = it->first;
        int targetSq = currAtt.getTargetSq();
        if(targetSq == king) {
            pair<int, int> startToTargetSq = make_pair(startSq, targetSq);
            checks.push_back(startToTargetSq);
        }
    }
    return checks;
}

bool Board::isSquareAttacked(int sq) {
    for (auto it = attackers.begin(); it != attackers.end(); ++it) {
        Attacker currAtt = it->second;
        int targetSq = currAtt.getTargetSq();
        if (targetSq == sq) {
            return true;
        }
    }
    return false;
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
bool Board::noPiecesBetweenNotKing(int startSq, int targetSq, int numSquaresAway) {
    if (abs(startSq - targetSq) < 10 || numSquaresAway < 2) {
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
    for (int i = start + diffIndex; i < end; i += diffIndex) {
        char oppColor = (moveColor == white) ? black : white;
        int testIdx = getKing(oppColor);
        if (!isNone(squares[i])&&testIdx!=i) {
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

bool Board::isGameover() {
    return (isCheckMate() || isDraw());
}
bool Board::isCheckMate() {
    int king = getKing(moveColor);
    bool checkmate = ucimoves.size() == 0 && isSquareAttacked(king);
    return checkmate;
}
bool Board::isDraw() {
    bool repdraw = repitionDraw();
    bool noForcedMate = noForcedMateDraw();
    bool _50moveRule = halfTurnNum == 100;
    bool stalemate = ucimoves.size() == 0 && !isSquareAttacked(king);
    if (repdraw || noForcedMate || _50moveRule || stalemate) {
        return true;
    }
    return false;
}
bool Board::noForcedMateDraw() {
    if (allPieces.size() == 2) {
        return true;
    }
    if (allPieces.size() == 3) {
        auto it = allPieces.begin();
        char p1 = it->second;
        std::advance(it, 1);
        char p2 = it->second;
        std::advance(it, 1);
        char p3 = it->second;
        bool isKnightPiece = isKnight(p1) || isKnight(p2) || isKnight(p3);
        bool isBishopPiece = isBishop(p1) || isBishop(p2) || isBishop(p3);
        if (isKnightPiece || isBishopPiece) {
            return true;
        }
    }
    return false;
}
bool Board::repitionDraw() {
    map<string, int>::iterator it = fenMap.find(fen);
    if (it != fenMap.end()) {
        if (it->second == 3) {
            return true;
        }
    }
    return false;
}


bool Board::makeMove(UciMove ucim) {
    string start = ucim.getStart();
    string target = ucim.getTarget();
    Move m = uciToMove(ucim);
    int startSq = m.getStartSq();
    int targetSq = m.getTargetSq();
    const char startSqPiece = squares[startSq];
    const char targetSqPiece = squares[targetSq];
    if(startSq == -1 || targetSq == -1) {
        cerr << start << target << " -> Illegal move." << endl;
        return false;
    }

    bool moveInMoves = false;
    for(auto ucimtest : ucimoves) {
        if(ucimtest == ucim) {
            moveInMoves = true;
            break;
        }
    }

    if(!moveInMoves) {
        cerr << start << target << " -> Illegal move." << endl;
        return false;
    }

    bool gameOver = isGameover();//need to check here in future
    if (gameOver) {
        return false;
    }

    whiteKingSqaure = isWhite(startSqPiece) && isKing(startSqPiece) ? targetSq : whiteKingSqaure;
    blackKingSqaure = isBlack(startSqPiece) && isKing(startSqPiece) ? targetSq : blackKingSqaure;

    bool pawnmove = isPawn(startSqPiece);
    bool promotionHelper = isWhite(moveColor) ? targetSq > 55 : targetSq < 8;
    bool promotion = isPawn(startSqPiece) && promotionHelper;
    bool castleMove = isKing(startSqPiece) && abs(targetSq - startSq) == 2;
    bool pawnDoubleMove = isPawn(startSqPiece) && abs(targetSq - startSq) == 16;
    int enpassantTargetSq = stringToIntSquare(enPassantTarget);
    bool enpassant = isPawn(startSqPiece) && enpassantTargetSq == targetSq;
    bool capture = !isNone(targetSqPiece) || enpassant;

    if (castleMove) {
        int blackOffsetRook = isWhite(moveColor) ? 0 : 56;
        int rookTarget = targetSq - startSq == 2 ? targetSq - 1: targetSq + 1;
        int rookBegin = targetSq - startSq == 2 ? 7 + blackOffsetRook : blackOffsetRook;
        char rookPiece = squares[rookBegin];
        addSquare(targetSq, startSqPiece);
        addSquare(rookTarget, rookPiece);
        clearSquare(startSq);
        clearSquare(rookBegin);
    } else if(enpassant) {
        int removeEnpassantPawnCaptured = isWhite(moveColor) ? enpassantTargetSq - 8 : enpassantTargetSq + 8;
        clearSquare(removeEnpassantPawnCaptured);
        addSquare(targetSq, startSqPiece);
        clearSquare(startSq);
    } else if (promotion) {
        char promoStr = target[2];
        promoStr = isWhite(moveColor) ? toupper(promoStr) : tolower(promoStr);
        char promoPiece = charToPiece(promoStr);
        clearSquare(targetSq);
        addSquare(targetSq, promoPiece);
        clearSquare(startSq);
    } else {
        if (capture) {
            clearSquare(targetSq);
        }
        addSquare(targetSq, targetSqPiece);
        clearSquare(startSq);
    }

    
    if (pawnDoubleMove) {
        isWhite(moveColor) ? enPassantTarget = intToStringNotation(startSq + 8) : enPassantTarget = intToStringNotation(startSq - 8);
    }
    else {
        enPassantTarget = "-";
    }
    //castling
    if ((start == "e1" || target == "e1") || (start == "h1" || target == "h1")) { castlingRights.erase(remove(castlingRights.begin(), castlingRights.end(), 'K'), castlingRights.end()); }//no castle short white
    if ((start == "e1" || target == "e1") || (start == "a1" || target == "a1")) { castlingRights.erase(remove(castlingRights.begin(), castlingRights.end(), 'Q'), castlingRights.end()); }//no castle long white
    if ((start == "e8" || target == "e8") || (start == "h8" || target == "h8")) { castlingRights.erase(remove(castlingRights.begin(), castlingRights.end(), 'k'), castlingRights.end()); }//no castle short black
    if ((start == "e8" || target == "e8") || (start == "a8" || target == "a8")) { castlingRights.erase(remove(castlingRights.begin(), castlingRights.end(), 'q'), castlingRights.end()); }//no castle long black

    wasCapture = capture;
    wasPromo = promotion;
    wasCastle = castleMove;
    wasEnpassant = enpassant;

    pawnmove || capture ? halfTurnNum = 0 : halfTurnNum++;
    if(isBlack(moveColor)) fullTurnNum++;
    setMoveColor();
    turnNum++;
    setfenMap();
    generateMoves();
    //update pgn
    return true;
}


//CONSTRUCTORS AND OPERATORS
Board::Board(string s="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
    for (int i = 0; i < 64; i++) {
        squares[i] = none;
    }
    loadFen(s);
}

void Board::loadFen(string f) {
    clearSquares();
    clearAttackers();
    clearAllPieces();
    clearMoves();
    int turnNum = 0;
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
    generateMoves();
    return;
}


Board::Board(const Board&) {

}
Board& Board::operator=(const Board&) {

    return *this;
}

void Board::setFen() {
    fen = "";
    int counter = 0;
    for (int file = 7; file >= 0; file--) {
        for (int rank = 0; rank < 8; rank++) {
            int index = file * 8 + rank;
            char piece = squares[index];
            if (isNone(piece)) {
                counter++;
            }
            else {
                if (counter) {
                    fen += to_string(counter);
                    counter = 0;
                }
                string pieceStr = pieceToString(piece);
                fen += pieceStr;
            }
        }
        if (counter) {
            fen.append(to_string(counter));
            counter = 0;
        }
        if (file != 0) {
            fen += "/";
        }
    }

    if (isWhite(moveColor)) { fen += " w "; }
    else if (!isWhite(moveColor)) { fen += " b "; }
    else { fen += " - "; }

    fen += castlingRights;
    fen += " ";
    fen += enPassantTarget;
    fen += " " + to_string(halfTurnNum) + " " + to_string(fullTurnNum);

    return;
}

string Board::getFen() {
    return fen;
}

void Board::setfenMap() {
    setFen();
    string fen_info = split(fen, " ")[0];
    map<string, int>::iterator it = fenMap.find(fen_info);
    if (it != fenMap.end()) {
        it->second++;
    }
    else {
        fenMap.insert(make_pair(fen_info, 1));
    }
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
void Board::addAttacker(Attacker a, int startSq) {
    attackedSquares.push_back(a.getTargetSq());
    attackers.insert(make_pair(startSq, a));
}
void Board::clearAttackers() {
    attackers.clear();
    attackedSquares.clear();
}
multimap<int, Attacker> Board::getAttackers() {
    return attackers;
}
char* Board::getSquares() {
    return squares;
}
void Board::clearSquares() {
    for(int i = 0; i < 64; i++) {
        squares[i] = none;
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


char Board::charToPiece(char c) {
    map<char, char> charToPiece = {
        make_pair('n', BlackKnight()),
        make_pair('b', BlackBishop()),
        make_pair('r', BlackRook()),
        make_pair('q', BlackQueen()),
        make_pair('N', WhiteKnight()),
        make_pair('B', WhiteBishop()),
        make_pair('R', WhiteRook()),
        make_pair('Q', WhiteQueen())
    };
    map<char, char>::iterator it;
    it = charToPiece.find(c);
    if (it != charToPiece.end()) {
        return it->second;
    }
    return char(-1);
}
string Board::pieceToString(const char p) {
    map<char, string> charToString = {
        make_pair(none, "."),
        make_pair(BlackPawn(), "p"),
        make_pair(BlackKnight(), "n"),
        make_pair(BlackBishop(), "b"),
        make_pair(BlackRook(), "r"),
        make_pair(BlackQueen(), "q"),
        make_pair(BlackKing(), "k"),
        make_pair(WhitePawn(), "P"),
        make_pair(WhiteKnight(), "N"),
        make_pair(WhiteBishop(), "B"),
        make_pair(WhiteRook(), "R"),
        make_pair(WhiteQueen(), "Q"),
        make_pair(WhiteKing(), "K")
    };
    map<char, string>::iterator it;
    it = charToString.find(p);
    if (it != charToString.end()) {
        return it->second;
    }
    return ".";
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

map<int, char> Board::getAllPieces() {
    return allPieces;
}
void Board::clearAllPieces() {
    allPieces.clear();
}
void Board::clearMoves() {
    moves.clear();
    ucimoves.clear();
}
void Board::setInit() {
    init();
}
void Board::print() {
    for (int file = 7; file >= 0; file--) {
        for (int rank = 0; rank < 8; rank++) {
            int value = file * 8 + rank;
            const char piece = squares[value];
            cout << pieceToString(piece) << " ";
        }
        cout << endl;
    }
}
void Board::printAttackers() {
    vector<int> attackedSquares;
    for (int file = 7; file >= 0; file--) {
        for (int rank = 0; rank < 8; rank++) {
            int sqIndex = file * 8 + rank;
            const char piece = squares[sqIndex];

            for (auto itr = attackers.begin(); itr != attackers.end(); itr++) {
                if (itr->first == sqIndex) {
                    int attackSq = itr->second.getTargetSq();
                    attackedSquares.push_back(attackSq);
                }
            }
        }
    }
    //cout << "attackers size " << attackedSquares.size() << endl;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int file = 7; file >= 0; file--) {
        for (int rank = 0; rank < 8; rank++) {
            int sqIndex = file * 8 + rank;
            const char piece = squares[sqIndex];
            if (count(attackedSquares.begin(), attackedSquares.end(), sqIndex)) {
                SetConsoleTextAttribute(hConsole, 12);
                cout << pieceToString(piece) << " ";
            }
            else {
                SetConsoleTextAttribute(hConsole, 15);
                cout << pieceToString(piece) << " ";
            }
        }
        SetConsoleTextAttribute(hConsole, 15);
        cout << endl;
    }
    SetConsoleTextAttribute(hConsole, 15);
    return;
}

UciMove Board::moveToUci(Move m, string promotion=string()) {//promotion will be Q, R, B, or N
    int startSq = m.getStartSq();
    int targetSq = m.getTargetSq();
    string start = intToStringNotation(startSq);
    string end = intToStringNotation(targetSq);
    if (!promotion.empty()) { end += promotion; }
    UciMove uci(start, end);
    return uci;
}

Move Board::uciToMove(UciMove ucim) {
    string start = ucim.getStart();
    string target = ucim.getTarget();
    if (target.length() == 3) {
        target.erase(target.begin() + 3);
    }
    int startSq = stringToIntSquare(start);
    int targetSq = stringToIntSquare(target);
    Move m(startSq, targetSq);
    return m;
}