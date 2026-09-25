#include "minimax.h"
#include "evaluate.h"

int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta){
    if(depth == 0){
        return evaluateBoard();
    }
    
    if(isMaximizingPlayer){  //forgot to add promotion to this, so it will not promote pawns when it is the maximizing player's turn
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
                            if(buffer.isPromotion) {
                                // Handle promotion logic here
                                board[buffer.newX][buffer.newY] = std::make_unique<Queen>(true);
                                int eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                                maxEval = std::max(maxEval, eval);
                                if(eval > alpha) {
                                    buffer.promotionChoice = 'Q'; // Store the promotion choice if it leads to a better evaluation
                                }
                                alpha = std::max(alpha, eval);

                                board[buffer.newX][buffer.newY] = std::make_unique<Rook>(true);
                                eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                                maxEval = std::max(maxEval, eval);
                                if(eval > alpha) {
                                    buffer.promotionChoice = 'R'; // Store the promotion choice if it leads to a better evaluation
                                }
                                alpha = std::max(alpha, eval);

                                board[buffer.newX][buffer.newY] = std::make_unique<Bishop>(true);
                                eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                                maxEval = std::max(maxEval, eval);
                                if(eval > alpha) {
                                    buffer.promotionChoice = 'B'; // Store the promotion choice if it leads to a better evaluation
                                }
                                alpha = std::max(alpha, eval);

                                board[buffer.newX][buffer.newY] = std::make_unique<Knight>(true);
                                eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                                maxEval = std::max(maxEval, eval);
                                if(eval > alpha) {
                                    buffer.promotionChoice = 'N'; // Store the promotion choice if it leads to a better evaluation
                                }
                                alpha = std::max(alpha, eval);
                                
                                board[buffer.newX][buffer.newY] = std::make_unique<Pawn>(true); // Remove the promoted piece after evaluation

                                undoMove(buffer, isMaximizingPlayer); // Undo the move after evaluating all promotion options
                            } else {
                                int eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                                maxEval = std::max(maxEval, eval);
                                if(eval > alpha) {
                                    buffer.promotionChoice = '\0'; // Reset the promotion choice
                                }
                                alpha = std::max(alpha, eval);
                                // Undo the move
                                undoMove(buffer, isMaximizingPlayer);
                            }


                            if(beta <= alpha){
                                return maxEval; // Beta cut-off
                            }
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
                            if(buffer.isPromotion) {
                                // Handle promotion logic here
                                board[buffer.newX][buffer.newY] = std::make_unique<Queen>(false);
                                int eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                                minEval = std::min(minEval, eval);
                                if(eval < beta) {
                                    buffer.promotionChoice = 'Q'; // Store the promotion choice if it leads to a better evaluation
                                }
                                beta = std::min(beta, eval);

                                board[buffer.newX][buffer.newY] = std::make_unique<Rook>(false);
                                eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                                minEval = std::min(minEval, eval);
                                if(eval < beta) {
                                    buffer.promotionChoice = 'R'; // Store the promotion choice if it leads to a better evaluation
                                }
                                beta = std::min(beta, eval);

                                board[buffer.newX][buffer.newY] = std::make_unique<Bishop>(false);
                                eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                                minEval = std::min(minEval, eval);
                                if(eval < beta) {
                                    buffer.promotionChoice = 'B'; // Store the promotion choice if it leads to a better evaluation
                                }
                                beta = std::min(beta, eval);

                                board[buffer.newX][buffer.newY] = std::make_unique<Knight>(false);
                                eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                                minEval = std::min(minEval, eval);
                                if(eval < beta) {
                                    buffer.promotionChoice = 'N'; // Store the promotion choice if it leads to a better evaluation
                                }
                                beta = std::min(beta, eval);

                                board[buffer.newX][buffer.newY] = std::make_unique<Pawn>(false); // Remove the promoted piece after evaluation

                                undoMove(buffer, isMaximizingPlayer); // Undo the move after evaluating all promotion options
                            } else {
                            int eval = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
                            minEval = std::min(minEval, eval);
                            beta = std::min(beta, eval);
                            // Undo the move
                            undoMove(buffer, isMaximizingPlayer);
                            }
                            if(beta <= alpha){
                                return minEval; // Alpha cut-off
                            }
                        }
                    }
                }
            }
        }
        return minEval;
    }
    
    return 0;
}
