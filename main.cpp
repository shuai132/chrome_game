#include "game/chrome_game.h"
#include "impl/game_engine_port_desktop.h"
#include "game_engine.hpp"

int main() {
    ScreenConfig c{};
    c.SCREEN_WIDTH = 128;
    c.SCREEN_HEIGHT = 32;
    c.PER_CHAR_WIDTH = 1;
    c.canvas = new Screen;
    auto game = static_cast<ge::Director *>(chrome_game_init(&c));
    game->start(120);
    return 0;
}
