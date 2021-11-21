#pragma once

#include "game_engine_port.h"

struct Screen : public ge::Canvas {
    Screen();
    void onClear() override;
    void onDraw() override;
    void drawBitmap(uint16_t x, uint16_t y, const uint8_t* bitmap, uint16_t width, uint16_t height, uint16_t color) override;
    size_t drawBuffer(uint16_t x, uint16_t y, const char *buffer, size_t len) override;
    ~Screen();

private:
    void* userData_;
};
