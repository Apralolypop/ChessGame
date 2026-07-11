#include <iostream>
#include <limits>
#include "board.h"

int fileToCol(char file) {
    return file - 'a';
}

std::vector<std::vector<char>> FastCastle(){
    std::vector<std::vector<char>> castleMoves;
    castleMoves.push_back({'e', '2', 'e', '4'}); // White kingside
    castleMoves.push_back({'e', '7', 'e', '5'}); // Black kingside
    castleMoves.push_back({'f', '1', 'c', '4'}); // White bishop
    castleMoves.push_back({'f', '8', 'c', '5'}); // Black bishop
    castleMoves.push_back({'g', '1', 'f', '3'}); // White knight
    castleMoves.push_back({'g', '8', 'f', '6'}); // Black knight
    return castleMoves;
}

std::vector<std::vector<char>> FastQueenCastle(){
    std::vector<std::vector<char>> castleMoves;
    castleMoves.push_back({'e', '2', 'e', '4'}); // White kingside
    castleMoves.push_back({'e', '7', 'e', '5'}); // Black kingside
    castleMoves.push_back({'f', '1', 'c', '4'}); // White bishop
    castleMoves.push_back({'f', '8', 'c', '5'}); // Black bishop
    castleMoves.push_back({'g', '1', 'f', '3'}); // White knight
    castleMoves.push_back({'g', '8', 'f', '6'}); // Black knight
    castleMoves.push_back({'d', '2', 'd', '4'}); // White queenside
    castleMoves.push_back({'d', '7', 'd', '5'}); // Black queenside
    castleMoves.push_back({'c', '1', 'd', '2'}); // White bishop
    castleMoves.push_back({'c', '8', 'd', '7'}); // Black bishop
    castleMoves.push_back({'b', '1', 'c', '3'}); // White knight
    castleMoves.push_back({'b', '8', 'c', '6'}); // Black knight
    castleMoves.push_back({'d', '1', 'e', '2'}); // White queen
    castleMoves.push_back({'d', '8', 'e', '7'}); // Black queen
    return castleMoves;
}

std::vector<std::vector<char>> FastPawnPromotionMoves(){
    std::vector<std::vector<char>> promotionMoves;
    promotionMoves.push_back({'h', '2', 'h', '4'}); // White pawn promotion
    promotionMoves.push_back({'a', '7', 'a', '5'}); // Black pawn promotion
    promotionMoves.push_back({'h', '4', 'h', '5'}); // White pawn promotion
    promotionMoves.push_back({'a', '5', 'a', '4'}); // Black pawn promotion
    promotionMoves.push_back({'h', '5', 'h', '6'}); // White pawn promotion
    promotionMoves.push_back({'a', '4', 'a', '3'}); // Black pawn promotion
    promotionMoves.push_back({'h', '6', 'g', '7'}); // White pawn promotion
    promotionMoves.push_back({'a', '3', 'b', '2'}); // Black pawn promotion
    promotionMoves.push_back({'g', '7', 'h', '8'}); // White pawn promotion
    promotionMoves.push_back({'b', '2', 'a', '1'}); // Black pawn promotion
    return promotionMoves;
}

int main(){
    VERBOSE = true; // console version wants the old debug prints back

    initializeBoard();

    bool gameRunning = true;
    bool WhitesTurn = true;
    

        /* ---------THIS IS FOR CASTLING TESTING ONLY *//*
        std::vector<std::vector<char>> castleMoves = FastQueenCastle();

        for(std::size_t i = 0; i < castleMoves.size(); i++){
            std::cout << "Castle Move: " << castleMoves[i][0] << castleMoves[i][1] << " to " << castleMoves[i][2] << castleMoves[i][3] << std::endl;
            movePiece(castleMoves[i][1] - '1', fileToCol(castleMoves[i][0]), castleMoves[i][3] - '1', fileToCol(castleMoves[i][2]), WhitesTurn);
            WhitesTurn = !WhitesTurn; // Toggle turn (for testing purposes)
            printBoard();
        }
        */

    while(gameRunning){
        
        printBoard();
      
        // Placeholder for game loop logic (e.g., handling player input, updating the board, checking for checkmate)
        //move generation and validation would go here (pass in WhitesTurn to determine which pieces can move)
        char c_y, c_newY;
        int x, y, newX, newY;
        std::cout << (WhitesTurn ? "White's turn" : "Black's turn") << std::endl;
        std::cout << "Enter move (y x newY newX): ";
        std::cin >> c_y >> x >> c_newY >> newX;
        std::cin.clear(); // Clear the input buffer to avoid issues with leftover characters



        y = fileToCol(c_y);

        newY = fileToCol(c_newY);


        std::cout << "Parsed move: (" << x << ", " << y << ") to (" << newX << ", " << newY << ")" << std::endl;

        if (!movePiece(x-1, y, newX-1, newY, WhitesTurn)) { 
            std::cout << "Invalid move!" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
            continue; // Skip the rest of the loop and prompt for input again
        }
    
        WhitesTurn = !WhitesTurn; // Toggle turn (for testing purposes)
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
    }

    return 0;
}