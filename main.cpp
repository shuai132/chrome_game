#include "game/chrome_game.h"
#include "impl/game_engine_port_desktop.h"

int main() {
    ScreenConfig c{};
    c.SCREEN_WIDTH = 128;
    c.SCREEN_HEIGHT = 32;
    c.PER_CHAR_WIDTH = 1;
    c.canvas = new Screen;
    c.fps = 120;
    start_game(&c);
    return 0;
}
