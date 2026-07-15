#include <iostream>
#include <memory>
#include <vector>

#include "../board.h"
#include "../pieces.h"
#include "engine.h"

//Engine to do list /*
/*
Finish all the steps
Add a Protected steps
Cache getLegal steps so that we dont recalculate eveerytime

*/

int evaluation = 0; // Initialize the evaluation score to 0
bool EngineIsWhite; // Global variable to track the current player's turn

#define EVALPERPIECE { 100, 320, 330, 500, 900, 20000 } // Pawn, Knight, Bishop, Rook, Queen, King

#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

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
    int mobility = board[x][y]->LegalMoves(x, y).size(); //This might create a bunch of vectors and make this slow;
}