#pragma once

#include <cstdint>
#include "game_engine_port.h"

struct ScreenConfig {
    uint16_t SCREEN_HEIGHT;
    uint16_t SCREEN_WIDTH;
    uint16_t PER_CHAR_WIDTH;
    ge::Canvas* canvas;
    uint16_t fps;
};
