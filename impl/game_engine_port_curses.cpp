#include "game_engine_port_desktop.h"

#include <curses.h>
#include <memory>
#include <thread>

namespace ge {

using namespace std::chrono;

unsigned long nowMs() {
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}
unsigned long nowUs() {
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}
void delayUs(unsigned int us) {
    std::this_thread::sleep_for(microseconds(us));
}

void delayMs(unsigned int us) {
    std::this_thread::sleep_for(milliseconds(us));
}

bool checkButton() {
    static volatile bool isClick = false;
    static auto t = std::thread([]{
        for(;;) {
            isClick = false;
            auto ch = getch();
            if (ch == ' ') {
                isClick = true;
            } else if (ch == 'q') {
                exit(0);
            }
            delayMs(300);
        }
    });
    return isClick;
}

}

Screen::Screen()
{
    initscr();
    raw();
    noecho();
    curs_set(0);
}

void Screen::onClear()
{
    clear();
}

void Screen::onDraw()
{
    refresh();
}

void Screen::drawBitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t width, uint16_t height, uint16_t color)
{
    const int bytesPerCol = (width-1)/8+1;
    for(int h = 0; h < height; h++) {
        for(int w = 0; w < width; w++) {
            auto pixel = bitmap[h*bytesPerCol + w/8] & (0b10000000 >> (w%8));
            mvprintw(y+h, x+w, pixel == 0 ? " " : "*");
        }
    }
}

size_t Screen::drawBuffer(uint16_t x, uint16_t y, const char *buffer, size_t len)
{
    return mvprintw(y, x, buffer, len);
}
