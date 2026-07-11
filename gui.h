#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "board.h"

// A minimal click-to-move chess GUI built on top of the existing board.h /
// board.cpp move logic. No image assets required: pieces are drawn as
// colored circles with their letter (P/N/B/R/Q/K) on top, so it builds and
// runs with nothing more than SFML and a system font.
class ChessGUI {
public:
    ChessGUI();
    void run();

private:
    static const int SQUARE = 80;
    static const int BOARD_PIXELS = SQUARE * 8;
    static const int STATUS_HEIGHT = 50;

    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded = false;

    bool WhitesTurn = true;
    bool gameOver = false;
    std::string statusMessage;

    bool selected = false;
    int selX = -1, selY = -1;
    std::vector<std::vector<int>> legalDestinations;

    void loadFont();

    bool screenToBoard(int mouseX, int mouseY, int& x, int& y) const;
    sf::Vector2f boardToScreen(int x, int y) const;

    void handleClick(int mouseX, int mouseY);
    void updateStatus();

    void draw();
    void drawBoard();
    void drawHighlights();
    void drawPieces();
    void drawStatus();

    char askPromotion(bool isWhite);
};

#endif // GUI_H