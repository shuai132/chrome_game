#include "game/game_engine_port.h"
#include <Arduino.h>
#include "OLED.h"

#define PIX_PER_CHAR 6

struct Screen : public ge::Canvas {
    Screen() {
        display.begin();
    }

    void onClear() override {
        display.clearDisplay();
    }

    void onDraw() override {
        display.display();
    }

    void* native() override {
        return &display;
    }

    void drawBitmap(uint16_t x, uint16_t y, const uint8_t* bitmap, uint16_t w, uint16_t h, uint16_t color) override {
        display.drawBitmap(x, y, bitmap, w, h, color);
    }

    size_t drawBuffer(uint16_t x, uint16_t y, const char *buffer, size_t len) override {
        return display.write((const uint8_t*) buffer, len);
    }
};
