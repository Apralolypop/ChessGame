#ifndef EVALUATE_H
#define EVALUATE_H

#include <iostream>
#include <vector>

extern bool EngineIsWhite; // Global variable to track engine's color, true for white, false for black

int evaluateBoard(); // Returns the whole board position evaluation score
int evaluateBoardPiece(); // Function to evaluate the board position
int evaluatePieceSquare(int x, int y, char Type, bool isWhite); // Function to evaluate the piece-square table
int evaluateMobility(int x, int y);
int evaluatePawnStructure(bool isWhite);
int evaluateKingSafety(int x, int y, bool isWhite);


#endif // ENGINE_H