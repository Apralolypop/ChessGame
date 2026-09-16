#include "engine/minimax.h"
#include "engine/evaluate.h"

int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta){
    if(depth == 0){
        return evaluateBoard();
    }
    
    if(isMaximizingPlayer){
        int maxEval = -1000000;
        std::vector<std::vector<int>> legalMoves = getAllLegalMovesForColor(isMaximizingPlayer); // Get all legal moves for white
        for(const auto& coord : legalMoves){
            move buffer;
            buffer.x = coord[0];
            buffer.y = coord[1];

            for(int newX = 0; newX < 8; newX++){
                for(int newY = 0; newY < 8; newY++){
                    if(checkLegal(buffer.x, buffer.y, newX, newY) != 0){
                        // Make the move
                        buffer.newX = newX;
                        buffer.newY = newY;
                        makeMove(buffer, isMaximizingPlayer);
                        if(buffer.isLegal){
                            int eval = minimax(depth - 1, false, alpha, beta);
                            maxEval = std::max(maxEval, eval);
                            alpha = std::max(alpha, eval);
                            // Undo the move
                            undoMove(buffer, isMaximizingPlayer);
                        }
                    }
                }
            }
        }
        return maxEval;
    } else {
        int minEval = 1000000;
        std::vector<std::vector<int>> legalMoves = getAllLegalMovesForColor(false); // Get all legal moves for black
        for(const auto& coord : legalMoves){
            int x = coord[0];
            int y = coord[1];
            move buffer;
            buffer.x = x;
            buffer.y = y;
            for(int newX = 0; newX < 8; newX++){
                for(int newY = 0; newY < 8; newY++){
                    if(checkLegal(x, y, newX, newY) != 0){
                        // Make the move

                        buffer.newX = newX;
                        buffer.newY = newY;
                        makeMove(buffer, isMaximizingPlayer);
                        if(buffer.isLegal){
                            int eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                            minEval = std::min(minEval, eval);
                            beta = std::min(beta, eval);
                            // Undo the move
                            undoMove(buffer, isMaximizingPlayer);
                        }
                    }
                }
            }
        }
        return minEval;
    }
    
    return 0;
}
