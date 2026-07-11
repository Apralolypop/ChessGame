#include <iostream>
#include <memory>
#include <vector>

#include "board.h"
#include "pieces.h"

std::unique_ptr<Piece> board[8][8];

std::vector<int> KingPositionWhite = {0, 4}; // Initial position of the white king
std::vector<int> KingPositionBlack = {7, 4}; // Initial position of the black king

bool VERBOSE = false;
std::function<char(bool)> promotionChoiceProvider = nullptr;

Piece::Piece() : type(' '), isWhite(true), isAlive(true), hasMoved(false) {} // Default constructor

Piece::Piece(char t, bool white) : type(t), isWhite(white), isAlive(true), hasMoved(false) {}

void printBoard(){
    std::cout << "  --------BLACK SIDE---------" << std::endl;
    std::cout << "Y:a b c d e f g h" << std::endl<<std::endl;

    for(int i = 0; i < 8; i++){
        std::cout << 8 - i << " ";
        for(int j = 0; j < 8; j++){
            if (board[7-i][j]) {
                std::cout << board[7-i][j]->type << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "  --------WHITE SIDE---------" << std::endl;
}

void initializeBoard(){
    // Initialize pawns
    for(int i = 0; i < 8; i++){
        board[1][i] = std::make_unique<Pawn>(true); // White pawns
        board[6][i] = std::make_unique<Pawn>(false); // Black pawns
    }
    // Initialize other pieces
    board[0][0] = std::make_unique<Rook>(true);
    board[0][7] = std::make_unique<Rook>(true);
    board[7][0] = std::make_unique<Rook>(false);
    board[7][7] = std::make_unique<Rook>(false);
    board[0][1] = std::make_unique<Knight>(true);
    board[0][6] = std::make_unique<Knight>(true);
    board[7][1] = std::make_unique<Knight>(false);
    board[7][6] = std::make_unique<Knight>(false);
    board[0][2] = std::make_unique<Bishop>(true);
    board[0][5] = std::make_unique<Bishop>(true);
    board[7][2] = std::make_unique<Bishop>(false);
    board[7][5] = std::make_unique<Bishop>(false);
    board[0][3] = std::make_unique<Queen>(true);
    board[0][4] = std::make_unique<King>(true);
    board[7][3] = std::make_unique<Queen>(false);
    board[7][4] = std::make_unique<King>(false);
}

bool KingWillBeChecked(int x, int y, int newX, int newY){
    // Store the original piece at the destination
    std::unique_ptr<Piece> originalPiece = std::move(board[newX][newY]);

    //Move the piece temporarily to the new position
    board[newX][newY] = std::move(board[x][y]);

    // Check if the king is in check at the new position
    if(board[newX][newY]->type == 'K') {
        if(isAttacked(newX, newY, board[newX][newY]->isWhite)) {
            // Restore the original state of the board
            board[x][y] = std::move(board[newX][newY]);
            board[newX][newY] = std::move(originalPiece);
            return true; // King is in check
        }
        else {
            // Restore the original state of the board
            board[x][y] = std::move(board[newX][newY]);
            board[newX][newY] = std::move(originalPiece);
            return false; // King is not in check
        }
    }

    bool isInCheck;

    if(board[newX][newY]->isWhite){
        isInCheck = isAttacked(KingPositionWhite[0], KingPositionWhite[1], true);
    }else{
        isInCheck = isAttacked(KingPositionBlack[0], KingPositionBlack[1], false);
    }


    // Restore the original state of the board
    board[x][y] = std::move(board[newX][newY]);
    board[newX][newY] = std::move(originalPiece);

    return isInCheck; // Return true if in check, false otherwise
}

int checkLegal(int x, int y, int newX, int newY){
    if (board[x][y] && board[x][y]->isAlive) {
        if(KingWillBeChecked(x, y, newX, newY)) {
            if (VERBOSE) std::cout << "Move would put or leave the king in check!" << std::endl;
            return 0; // Move is illegal because it puts or leaves the king in check
        }
        std::vector<std::vector<int>> legalMoves = board[x][y]->LegalMoves(x, y);

        for (const auto& move : legalMoves) {
            if (move[0] == newX && move[1] == newY) {
                return 1; // Move is legal
            }
        }
        if (VERBOSE) {
            for(const auto& move : legalMoves) {
                std::cout << "Legal move: (" << move[0] << ", " << move[1] << ")" << std::endl;
            }
        }

        //Check for castling moves
        if(board[x][y]->type == 'K') {
            if (VERBOSE) std::cout << "Checking for castling moves..." << std::endl;
            std::vector<std::vector<int>> castleMoves = getLegalCastleMoves(x, y, legalMoves);
            for (const auto& move : castleMoves) {
                if (move[0] == newX && move[1] == newY) {
                    return 2; // Castling move is legal
                }
            }
        }
    }
    return 0; // Move is not legal
}

bool movePiece(int x, int y, int newX, int newY, bool WhitesTurn){
    if (VERBOSE) std::cout<<"Attempting to move piece from (" << x << ", " << y << ") to (" << newX << ", " << newY << ")" << std::endl;
    if (board[x][y] && board[x][y]->isAlive && board[x][y]->isWhite == WhitesTurn) {
        // Placeholder for move validation logic      
        int returnValue = checkLegal(x, y, newX, newY);
        if (returnValue == 0) {
            if (VERBOSE) std::cout << "Move is not legal!" << std::endl;
            return false;
        }

        if(returnValue == 1) {
            if(board[newX][newY] && board[newX][newY]->isAlive) {
                if(board[newX][newY]->isWhite != board[x][y]->isWhite) {
                    board[newX][newY]->isEaten(); // Capture the piece
                } else {
                    if (VERBOSE) std::cout << "Cannot move to a square occupied by your own piece!" << std::endl;
                    return false;
                }
            }
        }

        if(returnValue == 2) {
            if (VERBOSE) std::cout << "Performing castling move..." << std::endl;
            if(newY == 6) { // Kingside castling
                board[x][5] = std::move(board[x][7]); // Move rook to f-file
                board[x][7] = nullptr; // Clear original rook position
                board[x][5]->hasMoved = true; // Mark the rook as having moved
            } else if(newY == 2) { // Queenside castling
                board[x][3] = std::move(board[x][0]); // Move rook to d-file
                board[x][0] = nullptr; // Clear original rook position
                board[x][3]->hasMoved = true; // Mark the rook as having moved
            }
        }

        if(board[x][y]->type == 'K') {
            if(board[x][y]->isWhite) {
                KingPositionWhite = {newX, newY};
            } else {
                KingPositionBlack = {newX, newY};
            }
        }

        board[newX][newY] = std::move(board[x][y]);
        board[x][y] = nullptr;

        if(board[newX][newY]->type == 'K' || board[newX][newY]->type == 'R') {
            board[newX][newY]->hasMoved = true;
        }

        if((board[newX][newY]->type == 'P') && board[newX][newY]->isWhite && newX == 7) {
            char promotionChoice = 'Q';
            if (promotionChoiceProvider) {
                promotionChoice = promotionChoiceProvider(true);
            } else {
                std::cout << "White pawn promoted!" << std::endl;
                std::cout << "Choose a piece for promotion (Q, R, B, N): ";
                std::cin >> promotionChoice;
            }

            // Promote to the chosen piece
            if (promotionChoice == 'Q') {
                board[newX][newY] = std::make_unique<Queen>(true);
            } else if (promotionChoice == 'R') {
                board[newX][newY] = std::make_unique<Rook>(true);
            } else if (promotionChoice == 'B') {
                board[newX][newY] = std::make_unique<Bishop>(true);
            } else if (promotionChoice == 'N') {
                board[newX][newY] = std::make_unique<Knight>(true);
            } else {
                if (VERBOSE) std::cout << "Invalid choice! Promoting to Queen by default." << std::endl;
                board[newX][newY] = std::make_unique<Queen>(true);
            }
        } else if((board[newX][newY]->type == 'P') && !board[newX][newY]->isWhite && newX == 0) {
            char promotionChoice = 'Q';
            if (promotionChoiceProvider) {
                promotionChoice = promotionChoiceProvider(false);
            } else {
                std::cout << "Black pawn promoted!" << std::endl;
                std::cout << "Choose a piece for promotion (Q, R, B, N): ";
                std::cin >> promotionChoice;
            }

            // Promote to the chosen piece
            if (promotionChoice == 'Q') {
                board[newX][newY] = std::make_unique<Queen>(false);
            } else if (promotionChoice == 'R') {
                board[newX][newY] = std::make_unique<Rook>(false);
            } else if (promotionChoice == 'B') {
                board[newX][newY] = std::make_unique<Bishop>(false);
            } else if (promotionChoice == 'N') {
                board[newX][newY] = std::make_unique<Knight>(false);
            } else {
                if (VERBOSE) std::cout << "Invalid choice! Promoting to Queen by default." << std::endl;
                board[newX][newY] = std::make_unique<Queen>(false);
            }
        }


        return true;
    }
    else{
        if (VERBOSE) std::cout << "No piece at the source position! Or wrong color!" << std::endl;
        return false;
    }
}

bool isPieceAt(int x, int y){
    return board[x][y] != nullptr && board[x][y]->isAlive;
}

std::vector<std::vector<int>> getLegalHorizontalMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) {
    // Check left
    for (int j = y - 1; j >= 0; j--) {
        if (isPieceAt(x, j)) {
            if (board[x][j]->isWhite != board[x][y]->isWhite) {
                legalMoves.push_back({x, j}); // Can capture opponent's piece
            }
            break; // Stop at the first piece encountered
        }
        legalMoves.push_back({x, j}); // Empty square
    }
    // Check right
    for (int j = y + 1; j < 8; j++) {
        if (isPieceAt(x, j)) {
            if (board[x][j]->isWhite != board[x][y]->isWhite) {
                legalMoves.push_back({x, j}); // Can capture opponent's piece
            }
            break; // Stop at the first piece encountered
        }
        legalMoves.push_back({x, j}); // Empty square
    }
    return legalMoves;
}

std::vector<std::vector<int>> getLegalVerticalMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) {
    // Check up
    for (int i = x - 1; i >= 0; i--) {
        if (isPieceAt(i, y)) {
            if (board[i][y]->isWhite != board[x][y]->isWhite) {
                legalMoves.push_back({i, y}); // Can capture opponent's piece
            }
            break; // Stop at the first piece encountered
        }
        legalMoves.push_back({i, y}); // Empty square
    }
    // Check down
    for (int i = x + 1; i < 8; i++) {
        if (isPieceAt(i, y)) {
            if (board[i][y]->isWhite != board[x][y]->isWhite) {
                legalMoves.push_back({i, y}); // Can capture opponent's piece
            }
            break; // Stop at the first piece encountered
        }
        legalMoves.push_back({i, y}); // Empty square
    }
    return legalMoves;
}

std::vector<std::vector<int>> getLegalDiagonalMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) {
    // Check top-left
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
        if (isPieceAt(i, j)) {
            if (board[i][j]->isWhite != board[x][y]->isWhite) {
                legalMoves.push_back({i, j}); // Can capture opponent's piece
            }
            break; // Stop at the first piece encountered
        }
        legalMoves.push_back({i, j}); // Empty square
    }
    // Check top-right
    for (int i = x - 1, j = y + 1; i >= 0 && j < 8; i--, j++) {
        if (isPieceAt(i, j)) {
            if (board[i][j]->isWhite != board[x][y]->isWhite) {
                legalMoves.push_back({i, j}); // Can capture opponent's piece
            }
            break; // Stop at the first piece encountered
        }
        legalMoves.push_back({i, j}); // Empty square
    }
    // Check bottom-left
    for (int i = x + 1, j = y - 1; i < 8 && j >= 0; i++, j--) {
        if (isPieceAt(i, j)) {
            if (board[i][j]->isWhite != board[x][y]->isWhite) {
                legalMoves.push_back({i, j}); // Can capture opponent's piece
            }
            break; // Stop at the first piece encountered
        }
        legalMoves.push_back({i, j}); // Empty square
    }
    // Check bottom-right
    for (int i = x + 1, j = y + 1; i < 8 && j < 8; i++, j++) {
        if (isPieceAt(i, j)) {
            if (board[i][j]->isWhite != board[x][y]->isWhite) {
                legalMoves.push_back({i, j}); // Can capture opponent's piece
            }
            break; // Stop at the first piece encountered
        }
        legalMoves.push_back({i, j}); // Empty square
    }
    return legalMoves;
}

std::vector<std::vector<int>> getLegalKnightMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) {
    std::vector<std::pair<int, int>> knightMoves = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };
    for (const auto& move : knightMoves) {
        int newX = x + move.first;
        int newY = y + move.second;
        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            if (!isPieceAt(newX, newY) || board[newX][newY]->isWhite != board[x][y]->isWhite) {
                legalMoves.push_back({newX, newY}); // Can move to empty square or capture opponent's piece
            }
        }
    }
    return legalMoves;
}

std::vector<std::vector<int>> getLegalKingMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) {
    std::vector<std::pair<int, int>> kingMoves = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    for (const auto& move : kingMoves) {
        int newX = x + move.first;
        int newY = y + move.second;
        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            if (!isPieceAt(newX, newY) || board[newX][newY]->isWhite != board[x][y]->isWhite) {
                legalMoves.push_back({newX, newY}); // Can move to empty square or capture opponent's piece
            }
        }
    }

    return legalMoves;
}

std::vector<std::vector<int>> getLegalCastleMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) {
    if (board[x][y]->hasMoved) {
        return legalMoves; // King has moved, cannot castle
    }

    // Check kingside castling
    if (isPieceAt(x, 7) && board[x][7]->type == 'R' && !board[x][7]->hasMoved && !isPieceAt(x, 5) && !isPieceAt(x, 6)) {
        if(!isAttacked(x, 4, board[x][y]->isWhite) && !isAttacked(x, 5, board[x][y]->isWhite) && !isAttacked(x, 6, board[x][y]->isWhite)) {
            legalMoves.push_back({x, 6}); // Kingside castling move
        }     
    }

    // Check queenside castling
    if (isPieceAt(x, 0) && board[x][0]->type == 'R' && !board[x][0]->hasMoved && !isPieceAt(x, 1) && !isPieceAt(x, 2) && !isPieceAt(x, 3)) {
        if(!isAttacked(x, 4, board[x][y]->isWhite) && !isAttacked(x, 3, board[x][y]->isWhite) && !isAttacked(x, 2, board[x][y]->isWhite)) {
            legalMoves.push_back({x, 2}); // Queenside castling move
        }
    }

    return legalMoves;
}

std::vector<std::vector<int>> getLegalPawnMoves(int x, int y, std::vector<std::vector<int>>& legalMoves) {
    int direction = board[x][y]->isWhite ? 1 : -1; // White pawns move up (positive direction), black pawns move down (negative direction)
    // Move forward
    if (!isPieceAt(x + direction, y)) {
        legalMoves.push_back({x + direction, y});
        // Move two squares forward from starting position
        if ((board[x][y]->hasMoved == false) && (x == 1 || x == 6)) {
            if (!isPieceAt(x + 2 * direction, y)) {
                legalMoves.push_back({x + 2 * direction, y});
            }
        }
    }
    // Capture diagonally
    // NOTE: fixed from the original — it previously bounds-checked
    // (x + direction) *after* already indexing board[x+direction][newY]
    // (a potential out-of-bounds read), and then returned immediately
    // instead of pushing the capture, so pawns could never actually
    // capture. Both are fixed below.
    for (int dy = -1; dy <= 1; dy += 2) {
        int newY = y + dy;
        int newX = x + direction;
        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8 &&
            isPieceAt(newX, newY) && board[newX][newY]->isWhite != board[x][y]->isWhite) {
            legalMoves.push_back({newX, newY});
        }
    }
    return legalMoves;
}

std::vector<std::vector<int>> Pawn::LegalMoves(int x, int y) {
    std::vector<std::vector<int>> legalMoves;
    return getLegalPawnMoves(x, y, legalMoves);
}

std::vector<std::vector<int>> Knight::LegalMoves(int x, int y) {
    std::vector<std::vector<int>> legalMoves;
    return getLegalKnightMoves(x, y, legalMoves);
}

std::vector<std::vector<int>> Bishop::LegalMoves(int x, int y) {
    std::vector<std::vector<int>> legalMoves;
    return getLegalDiagonalMoves(x, y, legalMoves);
}

std::vector<std::vector<int>> Rook::LegalMoves(int x, int y) {
    std::vector<std::vector<int>> legalMoves;
    getLegalHorizontalMoves(x, y, legalMoves);
    getLegalVerticalMoves(x, y, legalMoves);
    return legalMoves;
}

std::vector<std::vector<int>> Queen::LegalMoves(int x, int y) {
    std::vector<std::vector<int>> legalMoves;
    getLegalHorizontalMoves(x, y, legalMoves);
    getLegalVerticalMoves(x, y, legalMoves);
    getLegalDiagonalMoves(x, y, legalMoves);
    return legalMoves;
}

std::vector<std::vector<int>> King::LegalMoves(int x, int y) {
    std::vector<std::vector<int>> legalMoves;
    return getLegalKingMoves(x, y, legalMoves);
}

bool isAttacked(int x, int y, bool isWhite) {
    // Check for attacks from all pieces
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] && board[i][j]->isAlive && board[i][j]->isWhite != isWhite) {
                std::vector<std::vector<int>> opponentMoves = board[i][j]->LegalMoves(i, j);
                for (const auto& move : opponentMoves) {
                    if (move[0] == x && move[1] == y) {
                        return true; // The square is attacked
                    }
                }
            }
        }
    }
    return false; // The square is not attacked
}

// ---- New helpers for the GUI ----

std::vector<std::vector<int>> getAllLegalMoves(int x, int y, bool WhitesTurn) {
    std::vector<std::vector<int>> results;
    if (!board[x][y] || !board[x][y]->isAlive || board[x][y]->isWhite != WhitesTurn) {
        return results;
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i == x && j == y) continue;
            if (checkLegal(x, y, i, j) != 0) {
                results.push_back({i, j});
            }
        }
    }
    return results;
}

bool isKingInCheck(bool isWhite) {
    if (isWhite) {
        return isAttacked(KingPositionWhite[0], KingPositionWhite[1], true);
    }
    return isAttacked(KingPositionBlack[0], KingPositionBlack[1], false);
}

bool hasAnyLegalMove(bool isWhite) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] && board[i][j]->isAlive && board[i][j]->isWhite == isWhite) {
                if (!getAllLegalMoves(i, j, isWhite).empty()) {
                    return true;
                }
            }
        }
    }
    return false;
}

GameState getGameState(bool WhitesTurn) {
    bool inCheck = isKingInCheck(WhitesTurn);
    bool anyMoves = hasAnyLegalMove(WhitesTurn);
    if (!anyMoves) {
        return inCheck ? GameState::CHECKMATE : GameState::STALEMATE;
    }
    return inCheck ? GameState::CHECK : GameState::ONGOING;
}