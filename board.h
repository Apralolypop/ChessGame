#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <memory>
#include <vector>
#include <functional>

#include "pieces.h"

// Set to true to print detailed move-search debug output to the console.
// The console version (main.cpp) can turn this on; the GUI leaves it off
// so it isn't flooded with "Legal move: (...)" spam every time you click
// a piece (getAllLegalMoves probes all 64 squares). or at leasts says claude lol. idk
extern bool VERBOSE;

// If this is set, movePiece() calls it instead of blocking on std::cin
// when a pawn reaches the back rank. It must return one of 'Q','R','B','N'.
// The GUI sets this to a lambda that pops up an on-screen promotion menu.
extern std::function<char(bool isWhite)> promotionChoiceProvider;

enum class GameState { ONGOING, CHECK, CHECKMATE, STALEMATE };

void printBoard();
void initializeBoard();
int checkLegal(int x, int y, int newX, int newY);
bool KingWillBeChecked(int x, int y, int newX, int newY);
bool movePiece(int x, int y, int newX, int newY, bool WhitesTurn);
bool isPieceAt(int x, int y);

std::vector<std::vector<int>> getLegalHorizontalMoves(int x, int y, std::vector<std::vector<int>>& legalMoves);
std::vector<std::vector<int>> getLegalVerticalMoves(int x, int y, std::vector<std::vector<int>>& legalMoves);
std::vector<std::vector<int>> getLegalDiagonalMoves(int x, int y, std::vector<std::vector<int>>& legalMoves);
std::vector<std::vector<int>> getLegalKnightMoves(int x, int y, std::vector<std::vector<int>>& legalMoves);
std::vector<std::vector<int>> getLegalKingMoves(int x, int y, std::vector<std::vector<int>>& legalMoves);
std::vector<std::vector<int>> getLegalCastleMoves(int x, int y, std::vector<std::vector<int>>& legalMoves);
std::vector<std::vector<int>> getLegalPawnMoves(int x, int y, std::vector<std::vector<int>>& legalMoves);
void PawnEnPassantMoves(int x, int y, std::vector<std::vector<int>>& legalMoves);

bool isAttacked(int x, int y, bool isWhite);

// ---- Helpers added for the GUI ----

// Returns every fully-legal destination square for the piece at (x, y),
// already filtered for "does this leave/put my own king in check" and
// including castling destinations. Empty if there's no movable piece of
// the right color at (x, y).
std::vector<std::vector<int>> getAllLegalMoves(int x, int y, bool WhitesTurn);

bool isKingInCheck(bool isWhite);
bool hasAnyLegalMove(bool isWhite);

// Combines the two checks above into the four states a GUI cares about.
GameState getGameState(bool WhitesTurn);

extern std::unique_ptr<Piece> board[8][8];
extern std::vector<int> KingPositionWhite;
extern std::vector<int> KingPositionBlack;

#endif // BOARD_H