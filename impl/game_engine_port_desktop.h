#include "game/game_engine_port.h"

#include <memory>
#include <curses.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define PIX_PER_CHAR 1

struct Screen : public ge::Canvas {
    WINDOW* window;

    Screen() {
        window = newwin(SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);
        initscr();
        raw();
        keypad(stdscr, TRUE);
        noecho();
        curs_set(0);
        box(window, SCREEN_HEIGHT, SCREEN_WIDTH);
    }

    void onClear() override {
        clear();
    }

    void onDraw() override {
        refresh();
    }

    void drawBitmap(uint16_t x, uint16_t y, const uint8_t* bitmap, uint16_t width, uint16_t height, uint16_t color) override {
        const int bytesPerCol = (width-1)/8+1;
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                auto pixel = bitmap[h*bytesPerCol + w/8] & (0b10000000 >> (w%8));
                mvprintw(y+h, x+w, pixel == 0 ? " " : "*");
            }
        }
    }

    size_t drawText(uint16_t x, uint16_t y, const char* format, ...) override {
        va_list arg;
        va_start(arg, format);
        char temp[64];
        char* buffer = temp;
        size_t len = vsnprintf(temp, sizeof(temp), format, arg);
        va_end(arg);
        if (len > sizeof(temp) - 1) {
            buffer = new (std::nothrow) char[len + 1];
            if (!buffer) {
                return 0;
            }
            va_start(arg, format);
            vsnprintf(buffer, len + 1, format, arg);
            va_end(arg);
        }
        mvprintw(y, x, buffer, len);
        if (buffer != temp) {
            delete[] buffer;
        }
        return len;
    }
};
