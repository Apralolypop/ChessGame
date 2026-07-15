#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <vector>

extern bool EngineIsWhite; // Global variable to track the current player's turn

int evaluateBoard(); // Function to evaluate the board position
int evaluatePieceSquare(int x, int y, char Type, bool isWhite); // Function to evaluate the piece-square table
int evaluateMobility(int x, int y);


#endif // ENGINE_H