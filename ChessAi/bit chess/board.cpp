#include "board.h"
#include "piece.cpp"
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

//not in board
vector<string> split(string str, string token){
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}


void precomputedMoveData() {
    const int directionOffSets[8] = {8, -8, -1, 1, 7, -7, 9, -9};
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
}

struct Move {
    std::string startSq;
    std::string endSq;
};

vector<Move> moves;

vector<Move> Board::generateMoves() {
    for(int startSq = 0; startSq < 64; startSq++) {
        const int piece = squares[startSq];
        if(Piece.isColor(piece, moveColor))
    }
}




//in board
Board::Board(string s="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
    squares = new int[64];
    loadFen(s);
}

void Board::loadFen(string f="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
    Piece p;
    map<char, const int> piece_map = {
        {'k', p.BlackKing()},
        {'q', p.BlackQueen()},
        {'r', p.BlackRook()},
        {'b', p.BlackBishop()},
        {'n', p.BlackKnight()},
        {'p', p.BlackPawn()},

        {'K', p.WhiteKing()},
        {'Q', p.WhiteQueen()},
        {'R', p.WhiteRook()},
        {'B', p.WhiteBishop()},
        {'N', p.WhiteKnight()},
        {'P', p.WhitePawn()}
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