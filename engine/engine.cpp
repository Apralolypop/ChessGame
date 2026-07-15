#include <iostream>
#include <memory>
#include <vector>

#include "../board.h"
#include "../pieces.h"
#include "engine.h"

int evaluation = 0; // Initialize the evaluation score to 0
bool EngineIsWhite; // Global variable to track the current player's turn

#define EVALPERPIECE { 100, 320, 330, 500, 900, 20000 } // Pawn, Knight, Bishop, Rook, Queen, King

int evaluateBoard(){
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] && board[i][j]->isAlive) {
                if (board[i][j]->isWhite) {
                    if(VERBOSE) std::cout << "White piece at (" << i << ", " << j << "): " << board[i][j]->type << std::endl;
                    if(board[i][j]->type == 'P') {
                        evaluation += 100;
                    } else if (board[i][j]->type == 'N') {
                        evaluation += 320;
                    } else if (board[i][j]->type == 'B') {
                        evaluation += 330;
                    } else if (board[i][j]->type == 'R') {
                        evaluation += 500;
                    } else if (board[i][j]->type == 'Q') {
                        evaluation += 900;
                    } else if (board[i][j]->type == 'K') {
                        evaluation += 0;
                    }
                    } else {
                    if(VERBOSE) std::cout << "Black piece at (" << i << ", " << j << "): " << board[i][j]->type << std::endl;
                    if(board[i][j]->type == 'P') {
                        evaluation -= 100;
                    } else if (board[i][j]->type == 'N') {
                        evaluation -= 320;
                    } else if (board[i][j]->type == 'B') {
                        evaluation -= 330;
                    } else if (board[i][j]->type == 'R') {
                        evaluation -= 500;
                    } else if (board[i][j]->type == 'Q') {
                        evaluation -= 900;
                    } else if (board[i][j]->type == 'K') {
                        evaluation -= 0;
                    }
                }
            }
        }
    }
    std::cout<< "Evaluation score: " << evaluation << std::endl;
    return evaluation;
} // Function to evaluate the board position

