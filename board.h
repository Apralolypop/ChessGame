#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <utility>

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

struct move {
    int x, y;
    int newX, newY;
    bool isLegal = false; // true if the move is legal, false otherwise
    char promotionChoice = '0'; // 'Q', 'R', 'B', or 'N' / '0' for invalid
    bool kingSideCastle = false; // true if the move is a kingside castle
    bool queenSideCastle = false; // true if the move is a queenside castle
    int possibleEnPassantX = -1; // The x-coordinate of a pawn that can be captured en passant, or -1 if none
    int possibleEnPassantY = -1; // The y-coordinate of a pawn that can be captured en passant, or -1 if none
    bool isEnPassant = false; // true if the move is an en passant capture
    bool kingHasMoved = false; // true if the king has moved, false otherwise
    bool rookHasMoved = false; // true if the rook has moved, false otherwise
    bool pawnHasMoved = false; // true if the pawn has moved, false otherwise
    
    std::unique_ptr<Piece> capturedPiece = nullptr; // Pointer to the captured piece, if any
    move() = default; // Default constructor
    move(int x, int y, int newX, int newY, char promotionChoice = '0', bool kingSideCastle = false, bool queenSideCastle = false)
        : x(x), y(y), newX(newX), newY(newY), promotionChoice(promotionChoice), kingSideCastle(kingSideCastle), queenSideCastle(queenSideCastle),
        isLegal(false), isEnPassant(false), kingHasMoved(false), rookHasMoved(false){}
   
    move(move&& other) noexcept
        : x(other.x), y(other.y), newX(other.newX), newY(other.newY), promotionChoice(other.promotionChoice),
          kingSideCastle(other.kingSideCastle), queenSideCastle(other.queenSideCastle),
          possibleEnPassantX(other.possibleEnPassantX), possibleEnPassantY(other.possibleEnPassantY),
          capturedPiece(std::move(other.capturedPiece)), isLegal(other.isLegal), isEnPassant(other.isEnPassant), kingHasMoved(other.kingHasMoved), rookHasMoved(other.rookHasMoved) {
        // Reset the moved-from object
        other.x = 0;
        other.y = 0;
        other.newX = 0;
            
        other.newY = 0;
        other.promotionChoice = '0';
        other.kingSideCastle = false;
        other.queenSideCastle = false;
        other.possibleEnPassantX = -1;
        other.possibleEnPassantY = -1;
        other.capturedPiece = nullptr;
        other.kingHasMoved = false;
        other.rookHasMoved = false;
        other.isEnPassant = false;
    }
    
    move& operator=(move&& other) noexcept{
        if (this != &other) {
            x = other.x;
            y = other.y;
            newX = other.newX;
            newY = other.newY;
            promotionChoice = other.promotionChoice;
            kingSideCastle = other.kingSideCastle;
            queenSideCastle = other.queenSideCastle;
            possibleEnPassantX = other.possibleEnPassantX;
            possibleEnPassantY = other.possibleEnPassantY;
            capturedPiece = std::move(other.capturedPiece);
            isLegal = other.isLegal;
            isEnPassant = other.isEnPassant;
            kingHasMoved = other.kingHasMoved;
            rookHasMoved = other.rookHasMoved;

            // Reset the moved-from object
            other.x = 0;
            other.y = 0;
            other.newX = 0;
            other.newY = 0;
            other.promotionChoice = '0';
            other.kingSideCastle = false;
            other.queenSideCastle = false;
            other.possibleEnPassantX = -1;
            other.possibleEnPassantY = -1;
            other.capturedPiece = nullptr;
            other.isLegal = false;
            other.isEnPassant = false;
            other.kingHasMoved = false;
            other.rookHasMoved = false;
        }
        return *this;
    }

    ~move() = default; // Default destructor
};

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
std::vector<std::vector<int>> getAllLegalMovesForColor(bool isWhite);

bool isKingInCheck(bool isWhite);
bool hasAnyLegalMove(bool isWhite);

void makeMove(move &m, bool WhitesTurn); 
void undoMove(move &m, bool WhitesTurn);

// Combines the two checks above into the four states a GUI cares about.
GameState getGameState(bool WhitesTurn);

extern std::unique_ptr<Piece> board[8][8];
extern std::vector<int> KingPositionWhite;
extern std::vector<int> KingPositionBlack;

#endif // BOARD_H