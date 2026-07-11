#include "board.h"
#include "gui.h"

int main() {
    initializeBoard();

    ChessGUI gui;
    gui.run();

    return 0;
}