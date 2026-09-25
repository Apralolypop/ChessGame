#ifndef MOVEORDER_H
#define MOVEORDER_H

#include <iostream>
#include <vector>
#include "../board.h"

int reorderMoves(std::vector<std::vector<int>>& moves, bool isMaximizingPlayer); //returns the amount of useful moves, and reorders the vector to have the most useful moves first


#endif // MOVEORDER_H