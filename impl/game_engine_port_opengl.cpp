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

#include "Adafruit_GFX.h"

#define POINT_SIZE 10
#define WINDOW_WIDTH (128)
#define WINDOW_HEIGHT (64)
#define WINDOW_WIDTH_PIX (WINDOW_WIDTH * POINT_SIZE)
#define WINDOW_HEIGHT_PIX (WINDOW_HEIGHT * POINT_SIZE)

static bool s_pressed = false;

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
    return s_pressed;
}

}

struct Adafruit_GFX_OpenGL: public Adafruit_GFX {
    std::vector<uint8_t> frameBuffer;

    Adafruit_GFX_OpenGL(int16_t w, int16_t h): Adafruit_GFX(w, h) {
        frameBuffer.resize(WINDOW_WIDTH* WINDOW_HEIGHT);
    }

    void drawPixel(int16_t x, int16_t y, uint16_t color) override {
        frameBuffer[y*WINDOW_WIDTH + x] = color ? 0xff : 0;
    }
};

struct UserData {
    UserData() {
        oled = std::unique_ptr<Adafruit_GFX_OpenGL>(new Adafruit_GFX_OpenGL(WINDOW_WIDTH, WINDOW_HEIGHT));
    }
    GLFWwindow* window{};
    std::unique_ptr<Adafruit_GFX_OpenGL> oled;
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

    glfwMakeContextCurrent(window);
    const auto scale = 1.f;
    glScaled(scale*2/WINDOW_WIDTH,-scale*2/WINDOW_HEIGHT,1);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
            return;
        }
        if (key == GLFW_KEY_SPACE) {
            switch (action) {
                case GLFW_PRESS:
                    s_pressed = true;
                    break;
                case GLFW_RELEASE:
                    s_pressed = false;
                    break;
                default:
                    break;
            }
        }
    });
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

//    gluOrtho2D(-WINDOW_WIDTH, WINDOW_WIDTH, -WINDOW_HEIGHT, WINDOW_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(POINT_SIZE);
    glBegin(GL_POINTS);
    auto buf = userData->oled->frameBuffer.data();
//    glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_LUMINANCE, GL_UNSIGNED_BYTE, buf);
    for (int h = 0; h < WINDOW_HEIGHT; h++) {
        for (int w = 0; w < WINDOW_WIDTH; w++) {
            auto flag = buf[h * WINDOW_WIDTH + w];
            if (flag) {
                glVertex2f(w - WINDOW_WIDTH/2, h - (WINDOW_HEIGHT-0.9)/2);
            }
        }
    }
    glEnd();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

size_t Screen::drawBuffer(uint16_t x, uint16_t y, const char *buffer, size_t len)
{
    auto userData = static_cast<UserData*>(userData_);
    auto& oled = userData->oled;
    oled->setCursor(x, y);
    oled->setTextColor(1);
    return oled->write((const uint8_t*) buffer, len);
}

void Screen::drawBitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t width, uint16_t height, uint16_t color)
{
    auto userData = static_cast<UserData*>(userData_);
    userData->oled->drawBitmap(x, y, bitmap, width, height, color);
}

void Screen::onClear()
{
    auto userData = static_cast<UserData*>(userData_);
    for (auto &item : userData->oled->frameBuffer) {
        item = 0;
    }
}

Screen::~Screen()
{
    auto userData = static_cast<UserData*>(userData_);
    glfwDestroyWindow(userData->window);
    glfwTerminate();
    delete static_cast<UserData*>(userData_);
}
