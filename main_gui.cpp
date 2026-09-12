#include "board.h"
#include "gui.h"

#ifdef ENGINE_ENABLED 
#include "engine/evaluate.h"
#include "engine/minimax.h"
#endif

int main() {
    initializeBoard();

    ChessGUI gui;
    gui.run();

    return 0;
}