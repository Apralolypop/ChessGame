#include "board.h"
#include "gui.h"

#ifdef ENGINE_ENABLED
#include "engine/engine.h"
#endif

int main() {
    initializeBoard();

    ChessGUI gui;
    gui.run();

    return 0;
}