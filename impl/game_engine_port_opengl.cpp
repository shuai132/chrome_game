#include "game_engine_port_desktop.h"

#include <cstdint>
#include <thread>
#include <vector>

#include <GLFW/glfw3.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#define POINT_SIZE 10
#define WINDOW_WIDTH (128)
#define WINDOW_HEIGHT (64)
#define WINDOW_WIDTH_PIX (WINDOW_WIDTH * POINT_SIZE)
#define WINDOW_HEIGHT_PIX (WINDOW_HEIGHT * POINT_SIZE)

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
    return false;
}

}

struct UserData {
    GLFWwindow* window;
    std::vector<uint8_t> frameBuffer;
    UserData() {
        frameBuffer.resize(WINDOW_WIDTH* WINDOW_HEIGHT);
    }
};

static GLFWwindow* initGLFW()
{
    // Setup window
    glfwSetErrorCallback([](int error, const char* description) {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    });
    if (!glfwInit())
        return nullptr;
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH_PIX, WINDOW_HEIGHT_PIX, "game", nullptr, nullptr);
    if (window == nullptr)
        return nullptr;

    glfwSetWindowSizeLimits(window, 0, 0, WINDOW_WIDTH_PIX, WINDOW_HEIGHT_PIX);

    return window;
}

Screen::Screen()
{
    auto window = initGLFW();
    if (window == nullptr) throw std::runtime_error("window init error");
    auto userData = new UserData;
    this->userData_ = userData;
    userData->window = window;
}
void Screen::onDraw()
{
    auto userData = static_cast<UserData*>(userData_);
    auto window = userData->window;
    // Main loop
    if (glfwWindowShouldClose(window)) {
        exit(0);
    }

    glfwMakeContextCurrent(window);
//    gluOrtho2D(-WINDOW_WIDTH, WINDOW_WIDTH, -WINDOW_HEIGHT, WINDOW_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(POINT_SIZE);
    glBegin(GL_POINTS);
    auto buf = userData->frameBuffer.data();
    for (int h = 0; h < WINDOW_HEIGHT; h++) {
        for (int w = 0; w < WINDOW_WIDTH; w++) {
            auto flag = buf[h * WINDOW_WIDTH + w];
            if (flag) {
                glVertex2f((float)w/WINDOW_WIDTH, -(float)h/WINDOW_HEIGHT);
            }
        }
    }
    glVertex2i(0, 0);
    glEnd();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

size_t Screen::drawBuffer(uint16_t x, uint16_t y, const char *buffer, size_t len)
{
//        return mvprintw(y, x, buffer, len);
    return 0;
}


void Screen::drawBitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t width, uint16_t height, uint16_t color)
{
    const int bytesPerCol = (width-1)/8+1;
    for(int h = 0; h < height; h++) {
        for(int w = 0; w < width; w++) {
            auto pixel = bitmap[h*bytesPerCol + w/8] & (0b10000000 >> (w%8));
            auto userData = static_cast<UserData*>(userData_);
            if (pixel) {
                userData->frameBuffer[WINDOW_WIDTH*(y+h) + x+w] = 1;
            }
        }
    }
}

void Screen::onClear()
{
    auto userData = static_cast<UserData*>(userData_);
    for (auto &item : userData->frameBuffer) {
        item = 0;
    }
}

Screen::~Screen()
{
    // Cleanup
    auto window = (GLFWwindow*)userData_;
    glfwDestroyWindow(window);
    glfwTerminate();
    delete static_cast<UserData*>(userData_);
}
