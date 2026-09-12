#include <iostream>
#include <memory>
#include <vector>

#include "../board.h"
#include "../pieces.h"
#include "evaluate.h"

//Engine to do list /*
/*
Finish all the steps
Definitely redesign all the function in this so that they are more efficient and less redundant
*/

int evaluation = 0; // Initialize the evaluation score to 0
bool EngineIsWhite; // Global variable to track engine's color, true for white, false for black

#define EVALPERPIECE { 100, 320, 330, 500, 900, 200000 } // Pawn, Knight, Bishop, Rook, Queen, King

#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

void ColorScore(bool isWhite, int &score) {
    if(isWhite) {
        score *= 1; // White pieces have positive evaluation
    } else {
        score *= -1; // Black pieces have negative evaluation
    }
}

const int pieceSquareTable[6][8][8] = {
    // Pawn
    {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 5, 10, 10, -20, -20, 10, 10, 5 },
        { 5, -5, -10, 0, 0, -10, -5, 5 },
        { 0, 0, 0, 20, 20, 0, 0, 0 },
        { 5, 5, 10, 25, 25, 10, 5, 5 },
        {10,10,20,30,30,20,10,10},
        {50,50,50,50,50,50,50,50},
        {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 }
    },
    // Knight
    {
        {-50,-40,-30,-30,-30,-30,-40,-50},
        {-40,-20 ,0 ,5 ,5 ,0 ,-20,-40},
        {-30 ,5 ,10 ,15 ,15 ,10 ,5 ,-30},
        {-30 ,0 ,15 ,20 ,20 ,15 ,0 ,-30},
        {-30 ,5 ,15 ,20 ,20 ,15 ,5 ,-30},
        {-30 ,0 ,10 ,15 ,15 ,10 ,0 ,-30},
        {-40,-20 ,0 ,-5 ,-5 ,0 ,-20,-40},
        {-50,-40,-30,-30,-30,-30,-40,-50}
    },
    // Bishop
    {
        {-20,-10,-10,-10,-10,-10,-10,-20},
        {-10 ,0 ,0 ,0 ,0 ,0 ,0 ,-10},
        {-10 ,0 ,5 ,10 ,10 ,5 ,0 ,-10},
        {-10 ,5 ,5 ,10 ,10 ,5 ,5 ,-10},
        {-10 ,0 ,10 ,10 ,10 ,10 ,0 ,-10},
        {-10, 5, 5, 5, 5, 5, 5, -10},
        {-10, 0, 0, 0, 0, 0, 0, -10},
        {-20,-10,-10,-10,-10,-10,-10,-20}
    },
    // Rook
    {
        {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 },
        {5 ,10,10,10,10,10,10,5 },
        {-5, 0 ,0 ,0 ,0 ,0 ,0 ,-5},
        {-5, 0 ,0 ,0 ,0 ,0 ,0 ,-5},
        {-5, 0 ,0 ,0 ,0 ,0 ,0 ,-5},
        {-5, 0 ,0 ,0 ,0 ,0 ,0 ,-5},
        {-5, 0 ,0 ,0 ,0 ,0 ,0 ,-5},
        {  0,  5, 10, 10, 10, 10, 5,  0}
    },
    // Queen
    {
        {-20,-10,-10, -5, -5,-10,-10,-20},
        {-10 ,0 ,0 ,0 ,0 ,0 ,0 ,-10},
        {-10 ,0 ,5 ,5 ,5 ,5 ,0 ,-10},
        { -5 ,0 ,5 ,5 ,5 ,5 ,0 ,-5 },
        {  0 ,0 ,5 ,5 ,5 ,5 ,0 ,-5 },
        {-10, 5, 5, 5, 5, 5, 0 ,-10},
        {-10, 0, 0, 0, 0, 0, 0 ,-10},
        {-20,-10,-10,-10,-10,-10,-10,-20}
    },
    // King
    {
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-20,-30,-30,-40,-40,-30,-30,-20},
        {-10, 0 ,0 ,0 ,0 ,0 ,0 ,-10},
        { 20, 20, 0 ,0 ,0 ,0 ,20, 20},
        { 20, 30, 10, 0 ,0 ,10, 30, 20}
    }
};

int evaluateBoard(){
    int score = 0;
    score += evaluateBoardPiece();

    score += evaluatePawnStructure(true); // Evaluate white pawn structure
    score += evaluatePawnStructure(false); // Evaluate black pawn structure

    score += evaluateKingSafety(KingPositionWhite[0], KingPositionWhite[1], true); // Evaluate white king safety
    score += evaluateKingSafety(KingPositionBlack[0], KingPositionBlack[1], false); // Evaluate black king safety

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j] && board[i][j]->isAlive){
                score += evaluateMobility(i, j); // Evaluate mobility for each piece
                score += evaluatePieceSquare(i, j, board[i][j]->type, board[i][j]->isWhite);
            }
        }
    }
    return score;
}

int evaluateBoardPiece(){
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] && board[i][j]->isAlive) {
                if (board[i][j]->isWhite) {
                    if(VERBOSE) std::cout << "White piece at (" << i << ", " << j << "): " << board[i][j]->type << std::endl;
                    switch(board[i][j]->type) {
                        case 'P':
                            evaluation += 100;
                            break;
                        case 'N':
                            evaluation += 320;
                            break;
                        case 'B':
                            evaluation += 330;
                            break;
                        case 'R':
                            evaluation += 500;
                            break;
                        case 'Q':
                            evaluation += 900;
                            break;
                        case 'K':
                            evaluation += 0;
                            break;
                    }
                } else {
                    if(VERBOSE) std::cout << "Black piece at (" << i << ", " << j << "): " << board[i][j]->type << std::endl;
                    switch(board[i][j]->type) {
                        case 'P':
                            evaluation -= 100;
                            break;
                        case 'N':
                            evaluation -= 320;
                            break;
                        case 'B':
                            evaluation -= 330;
                            break;
                        case 'R':
                            evaluation -= 500;
                            break;
                        case 'Q':
                            evaluation -= 900;
                            break;
                        case 'K':
                            evaluation -= 0;
                            break;
                    }
                }

                if(board[i][j]->isWhite){
                    evaluation += evaluatePieceSquare(i, j, board[i][j]->type, board[i][j]->isWhite);
                }
                else {
                    evaluation -= evaluatePieceSquare(i, j, board[i][j]->type, board[i][j]->isWhite);
                }
            }
        }
    }
    std::cout<< "Evaluation score: " << evaluation << std::endl;
    return evaluation;
} // Function to evaluate the board position

int evaluatePieceSquare(int x, int y, char Type, bool isWhite){
    int TypeIndex;
    switch(Type){
        case 'P':
            TypeIndex = PAWN;
            break;
        case 'N':
            TypeIndex = KNIGHT;
            break;
        case 'B':
            TypeIndex = BISHOP;
            break;
        case 'R':
            TypeIndex = ROOK;
            break;
        case 'Q':
            TypeIndex = QUEEN;
            break;
        case 'K':
            TypeIndex = KING;
            break;
    }

    // Implementation for evaluating piece-square table
    if(isWhite){
        return pieceSquareTable[TypeIndex][x][y];
    }
    else{
        return pieceSquareTable[TypeIndex][x][7-y];
    }
    return 0;
}

int evaluateMobility(int x, int y){
    int mobility = board[x][y]->LegalMoves(x, y).size(); 

    ColorScore(board[x][y]->isWhite, mobility);

    return mobility*5; // Weight mobility by 5 points per legal move
}

int evaluatePawnStructure(bool isWhite) {
    int score = 0;

    // Check for doubled pawns
    for (int j = 0; j < 8; j++) {
        int pawnCount = 0;
        for (int i = 0; i < 8; i++) {
            if (board[i][j] && board[i][j]->type == 'P' && board[i][j]->isWhite == isWhite) {
                pawnCount++;
            }
        }
        if (pawnCount > 1) {
            score -= 20 * (pawnCount - 1); // Penalize for each additional pawn in the same file
        }
    }

    // Check for isolated pawns
    for (int j = 0; j < 8; j++) {
        bool hasAdjacentPawn = false;
        for (int i = 0; i < 8; i++) {
            if (board[i][j] && board[i][j]->type == 'P' && board[i][j]->isWhite == isWhite) {
                // Check adjacent files
                if ((j > 0 && board[i][j - 1] && board[i][j - 1]->type == 'P' && board[i][j - 1]->isWhite == isWhite) ||
                    (j < 7 && board[i][j + 1] && board[i][j + 1]->type == 'P' && board[i][j + 1]->isWhite == isWhite)) {
                    hasAdjacentPawn = true;
                    break;
                }
            }
        }
        if (!hasAdjacentPawn) {
            score -= 15; // Penalize for isolated pawns
        }
    }

    ColorScore(isWhite, score); 
    return score;
}

int evaluateKingSafety(int x, int y, bool isWhite) {
        int score = 0;

        // Check for castling rights
    if (board[x][y]->hasMoved) {
        score -= 20; // Penalize if the king has moved (lost castling rights)
    }

    // Check for pawn shield
    int direction = isWhite ? 1 : -1; // White pawns are below the king, black pawns are above
    for (int j = -1; j <= 1; j++) {
        int newX = x + direction;
        int newY = y + j;
        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            if (board[newX][newY] && board[newX][newY]->type == 'P' && board[newX][newY]->isWhite == isWhite) {
                score += 10; // Reward for having a pawn shield
            }
        }
    }

    ColorScore(isWhite, score);
    return score;
}