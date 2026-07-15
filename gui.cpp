#include "gui.h"
#include <iostream>
#include <optional>

#ifdef ENGINE_ENABLED
#include "engine/engine.h"
#endif

extern int enPassantX;
extern int enPassantY;

namespace {
    const sf::Color LIGHT_SQUARE(240, 217, 181);
    const sf::Color DARK_SQUARE(181, 136, 99);
    const sf::Color SELECT_COLOR(246, 246, 105, 180);
    const sf::Color MOVE_DOT_COLOR(20, 120, 20, 160);
    const sf::Color CAPTURE_RING_COLOR(200, 40, 40, 200);
    const sf::Color STATUS_BG(30, 30, 30);
}

ChessGUI::ChessGUI()
    : window(sf::VideoMode({static_cast<unsigned int>(BOARD_PIXELS),
                             static_cast<unsigned int>(BOARD_PIXELS + STATUS_HEIGHT)}),
             "Chess") {
    window.setFramerateLimit(60);
    loadFont();
    updateStatus();
}

void ChessGUI::loadFont() {
    std::vector<std::string> candidates = {
        "/mingw64/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "C:\\msys64\\mingw64\\share\\fonts\\truetype\\dejavu\\DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "C:\\Windows\\Fonts\\arialbd.ttf",
        "C:\\Windows\\Fonts\\arial.ttf",
        "assets/font.ttf",
        "font.ttf"
    };
    for (const auto& path : candidates) {
        if (font.openFromFile(path)) {
            fontLoaded = true;
            break;
        }
    }
    if (!fontLoaded) {
        std::cerr << "Warning: could not find a system font. Text (piece letters, "
                     "status bar) will not be drawn. Drop a .ttf file next to the "
                     "executable and name it font.ttf to fix this." << std::endl;
    }
}

sf::Vector2f ChessGUI::boardToScreen(int x, int y) const {
    int rowFromTop = 7 - x; // rank 8 at the top of the screen
    float px = static_cast<float>(y * SQUARE);
    float py = static_cast<float>(STATUS_HEIGHT + rowFromTop * SQUARE);
    return {px, py};
}

bool ChessGUI::screenToBoard(int mouseX, int mouseY, int& x, int& y) const {
    if (mouseY < STATUS_HEIGHT) return false;
    int col = mouseX / SQUARE;
    int rowFromTop = (mouseY - STATUS_HEIGHT) / SQUARE;
    if (col < 0 || col > 7 || rowFromTop < 0 || rowFromTop > 7) return false;
    y = col;
    x = 7 - rowFromTop;
    return true;
}

void ChessGUI::run() {
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    handleClick(mb->position.x, mb->position.y);
                }
            }
            #ifdef ENGINE_ENABLED
            evaluateboard();
            #endif
        }
        draw();
    }
}

void ChessGUI::handleClick(int mouseX, int mouseY) {
    if (gameOver) return;

    int x, y;
    if (!screenToBoard(mouseX, mouseY, x, y)) return;

    if (!selected) {
        if (board[x][y] && board[x][y]->isAlive && board[x][y]->isWhite == WhitesTurn) {
            selected = true;
            selX = x;
            selY = y;
            legalDestinations = getAllLegalMoves(x, y, WhitesTurn);
        }
        return;
    }

    // Something is already selected.
    if (x == selX && y == selY) {
        // Clicked the same square again: deselect.
        selected = false;
        legalDestinations.clear();
        return;
    }

    bool isLegalTarget = false;
    for (const auto& move : legalDestinations) {
        if (move[0] == x && move[1] == y) {
            isLegalTarget = true;
            break;
        }
    }

    if (isLegalTarget) {
        promotionChoiceProvider = [this](bool isWhite) { return askPromotion(isWhite); };
        bool moved = movePiece(selX, selY, x, y, WhitesTurn);
        promotionChoiceProvider = nullptr;

        selected = false;
        legalDestinations.clear();

        if (moved) {
            WhitesTurn = !WhitesTurn;
            updateStatus();
        }
        return;
    }

    // Not a legal target — maybe they're switching selection to another
    // one of their own pieces instead.
    if (board[x][y] && board[x][y]->isAlive && board[x][y]->isWhite == WhitesTurn) {
        selX = x;
        selY = y;
        legalDestinations = getAllLegalMoves(x, y, WhitesTurn);
    } else {
        selected = false;
        legalDestinations.clear();
    }
}

void ChessGUI::updateStatus() {
    if (gameOver) return;

    GameState state = getGameState(WhitesTurn);
    std::string turn = WhitesTurn ? "White" : "Black";

    switch (state) {
        case GameState::CHECKMATE:
            statusMessage = turn + " is checkmated. " + (WhitesTurn ? "Black" : "White") + " wins!";
            gameOver = true;
            break;
        case GameState::STALEMATE:
            statusMessage = "Stalemate. It's a draw.";
            gameOver = true;
            break;
        case GameState::CHECK:
            statusMessage = turn + "'s turn - Check!";
            break;
        case GameState::ONGOING:
        default:
            statusMessage = turn + "'s turn";
            break;
    }
}

void ChessGUI::drawBoard() {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            sf::RectangleShape square(sf::Vector2f(SQUARE, SQUARE));
            square.setPosition(boardToScreen(x, y));
            square.setFillColor(((x + y) % 2 == 0) ? DARK_SQUARE : LIGHT_SQUARE);
            window.draw(square);
        }
    }
}

void ChessGUI::drawHighlights() {
    if (selected) {
        sf::RectangleShape highlight(sf::Vector2f(SQUARE, SQUARE));
        highlight.setPosition(boardToScreen(selX, selY));
        highlight.setFillColor(SELECT_COLOR);
        window.draw(highlight);
    }

    bool selectedIsPawn = selected && board[selX][selY] && board[selX][selY]->type == 'P';

    for (const auto& move : legalDestinations) {
        sf::Vector2f pos = boardToScreen(move[0], move[1]);
        bool isCapture = board[move[0]][move[1]] && board[move[0]][move[1]]->isAlive;

        // En passant: destination is empty, but it's still a capture
        bool isEnPassant = selectedIsPawn && !isCapture &&
                            move[1] != selY &&
                            move[0] == enPassantX && move[1] == enPassantY;

        if (isCapture || isEnPassant) {
            sf::CircleShape ring(SQUARE / 2.f - 4.f);
            ring.setPosition({pos.x + 4, pos.y + 4});
            ring.setFillColor(sf::Color::Transparent);
            ring.setOutlineColor(CAPTURE_RING_COLOR);
            ring.setOutlineThickness(4);
            window.draw(ring);
        } else {
            sf::CircleShape dot(SQUARE / 6.f);
            dot.setPosition({pos.x + SQUARE / 2.f - SQUARE / 6.f, pos.y + SQUARE / 2.f - SQUARE / 6.f});
            dot.setFillColor(MOVE_DOT_COLOR);
            window.draw(dot);
        }
    }
}
void ChessGUI::drawPieces() {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (!board[x][y] || !board[x][y]->isAlive) continue;

            sf::Vector2f pos = boardToScreen(x, y);
            float cx = pos.x + SQUARE / 2.f;
            float cy = pos.y + SQUARE / 2.f;
            bool isWhite = board[x][y]->isWhite;

            sf::CircleShape piece(SQUARE / 2.f - 8.f);
            piece.setOrigin({piece.getRadius(), piece.getRadius()});
            piece.setPosition({cx, cy});
            piece.setFillColor(isWhite ? sf::Color(250, 250, 250) : sf::Color(40, 40, 40));
            piece.setOutlineColor(isWhite ? sf::Color(40, 40, 40) : sf::Color(230, 230, 230));
            piece.setOutlineThickness(3);
            window.draw(piece);

            if (fontLoaded) {
                sf::Text label(font, std::string(1, board[x][y]->type), 28);
                label.setStyle(sf::Text::Bold);
                label.setFillColor(isWhite ? sf::Color(40, 40, 40) : sf::Color(230, 230, 230));
                sf::FloatRect tb = label.getLocalBounds();
                label.setOrigin({tb.position.x + tb.size.x / 2.f, tb.position.y + tb.size.y / 2.f});
                label.setPosition({cx, cy});
                window.draw(label);
            }
        }
    }
}

void ChessGUI::drawStatus() {
    sf::RectangleShape bar(sf::Vector2f(BOARD_PIXELS, STATUS_HEIGHT));
    bar.setPosition({0, 0});
    bar.setFillColor(STATUS_BG);
    window.draw(bar);

    if (fontLoaded) {
        sf::Text text(font, statusMessage, 22);
        text.setFillColor(sf::Color::White);
        sf::FloatRect tb = text.getLocalBounds();
        text.setOrigin({tb.position.x, tb.position.y});
        text.setPosition({12.f, (STATUS_HEIGHT - tb.size.y) / 2.f - 6});
        window.draw(text);
    }
}

void ChessGUI::draw() {
    window.clear(sf::Color(20, 20, 20));
    drawBoard();
    drawHighlights();
    drawPieces();
    drawStatus();
    window.display();
}

char ChessGUI::askPromotion(bool isWhite) {
    const char options[4] = {'Q', 'R', 'B', 'N'};
    const float boxW = 65.f, boxH = 90.f, gap = 5.f;
    const float menuWidth = 4 * boxW + 3 * gap;
    const float menuX = (BOARD_PIXELS - menuWidth) / 2.f;
    const float menuY = STATUS_HEIGHT + (BOARD_PIXELS - boxH) / 2.f;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return 'Q';
            }
            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    float mx = static_cast<float>(mb->position.x);
                    float my = static_cast<float>(mb->position.y);
                    for (int i = 0; i < 4; i++) {
                        sf::FloatRect box({menuX + i * (boxW + gap), menuY}, {boxW, boxH});
                        if (box.contains({mx, my})) {
                            return options[i];
                        }
                    }
                }
            }
        }

        // Redraw the board underneath plus a dimmed overlay + menu so the
        // window stays responsive while we wait for a choice.
        window.clear(sf::Color(20, 20, 20));
        drawBoard();
        drawPieces();

        sf::RectangleShape dim(sf::Vector2f(BOARD_PIXELS, BOARD_PIXELS + STATUS_HEIGHT));
        dim.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(dim);

        for (int i = 0; i < 4; i++) {
            sf::RectangleShape box(sf::Vector2f(boxW, boxH));
            box.setPosition({menuX + i * (boxW + gap), menuY});
            box.setFillColor(isWhite ? sf::Color(250, 250, 250) : sf::Color(60, 60, 60));
            box.setOutlineColor(sf::Color(255, 215, 0));
            box.setOutlineThickness(2);
            window.draw(box);

            if (fontLoaded) {
                sf::Text label(font, std::string(1, options[i]), 30);
                label.setStyle(sf::Text::Bold);
                label.setFillColor(isWhite ? sf::Color::Black : sf::Color::White);
                sf::FloatRect tb = label.getLocalBounds();
                label.setOrigin({tb.position.x + tb.size.x / 2.f, tb.position.y + tb.size.y / 2.f});
                label.setPosition({menuX + i * (boxW + gap) + boxW / 2.f, menuY + boxH / 2.f});
                window.draw(label);
            }
        }

        window.display();
    }
    return 'Q';
}