#ifndef PIECES_H
#define PIECES_H

#include <iostream>
#include <vector>

class DeadPiece; // Forward declaration
class Piece;    
class Pawn;
class Rook;
class Knight;
class Bishop;
class Queen;
class King;

// Piece is dependent on DeadPiece, so DEadpiece must be defined first

struct DeadPiece {
private:
public:
    char type; 
    bool isWhite; 

    DeadPiece(char t, bool white);
    DeadPiece() = delete; // Default constructor
};


class Piece {
public:
    char type; // 'P' for pawn, 'R' for rook, etc.
    bool isWhite; // true for white pieces, false for black pieces
    bool isAlive; // To track if the piece is still on the board
    bool hasMoved; // To track if the piece has moved (for castling and pawn's first move)

    Piece(); // Default constructor

    DeadPiece PieceEaten(); // Returns a vector indicating which piece has been captured //IsEaten API, this runs whenever a piece is captured

    void isEaten(){
        PieceEaten(); // Call the PieceEaten function to update the piece's status
        isAlive = false;
    }

    virtual ~Piece() = default; // Virtual destructor for proper cleanup of derived classes

    virtual std::vector<std::vector<int>> LegalMoves(int x, int y) = 0; // Placeholder for legal move generation logic

    Piece(char t, bool white);
};

class Pawn : public Piece {
    private:

public:
    
    bool Moved2; // To track if the pawn has moved (for en passant + 2 square move)

    std::vector<std::vector<int>> LegalMoves(int x, int y) override;

    Pawn() : Piece() {} // Default constructor
    Pawn(bool white) : Piece('P', white), Moved2(false) {}  
};

class Rook : public Piece {
    private:

public:
    std::vector<std::vector<int>> LegalMoves(int x, int y) override;
    bool hasMoved; // To track if the rook has moved (for castling)
    Rook(bool white) : Piece('R', white), hasMoved(false) {}
};

class Knight : public Piece {
public:
    std::vector<std::vector<int>> LegalMoves(int x, int y) override;
    Knight(bool white) : Piece('N', white) {}
};  

class Bishop : public Piece {
public:
    std::vector<std::vector<int>> LegalMoves(int x, int y) override;    
    Bishop(bool white) : Piece('B', white) {}
}; 

class Queen : public Piece {
public:
    std::vector<std::vector<int>> LegalMoves(int x, int y) override;
    Queen(bool white) : Piece('Q', white) {}
};

class King : public Piece {
private: 
public:
    std::vector<std::vector<int>> LegalMoves(int x, int y) override;
    bool hasMoved; // To track if the king has moved (for castling)
    King(bool white) : Piece('K', white), hasMoved(false) {}
};




#endif // PIECES_H