#include "engine/minimax.h"
#include "engine/evaluate.h"

/*
Values of each piece type and their moves:
take pieces captures
Promotions
Checks
*/


int reorderMoves(std::vector<std::vector<int>>& moves, bool isMaximizingPlayer) { 
    int moveCount = 0; 
  
    for(auto a = moves.begin(); a != moves.end(); a++) {
        // check what type of moves it is
        int x = (*a)[0];
        int y = (*a)[1];
        

    }

    return moveCount;
}