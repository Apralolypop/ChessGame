#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <memory>
#include <vector>

#include "pieces.h"



void printBoard();

void initializeBoard();

int checkLegal(int x, int y, int newX, int newY);

bool KingWillBeChecked(int x, int y, int newX, int newY);

bool movePiece(int x, int y, int newX, int newY, bool WhitesTurn);

bool isPieceAt(int x, int y);

std::vector<std::vector<int>> getLegalHorizontalMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) ;

std::vector<std::vector<int>> getLegalVerticalMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) ;

std::vector<std::vector<int>> getLegalDiagonalMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) ;

std::vector<std::vector<int>> getLegalKnightMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) ;

std::vector<std::vector<int>> getLegalKingMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) ;

std::vector<std::vector<int>> getLegalCastleMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) ;

std::vector<std::vector<int>> getLegalPawnMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) ;

bool isAttacked(int x, int y, bool isWhite);


#endif // BOARD_H