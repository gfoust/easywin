#include <exception>
#include "window.hpp"
#include <iostream>

using easywin::Canvas;
using easywin::Panel;
using easywin::Window;
using easywin::Colors;

class BluePanel : public Panel {
public:

  void onPaint(Canvas& canvas) override {
    auto sz = clientSize();
    canvas.drawRectangle({ 0, 0 }, { sz.width, sz.height }, 0, Colors::Blue);
  }
};

class ManPanel : public Panel {

  void onPaint(Canvas& canvas) override {
    auto sz = clientSize();
    auto mid = sz.width / 2;
    canvas.drawRectangle({ 0, 0 }, { sz.width, sz.height }, Colors::Black, Colors::White);
    canvas.drawEllipse({ mid, 200 }, 50, 0, easywin::rgb(255, 255, 255));
    canvas.drawLine({ mid, 250 }, { mid, 380 }, 0);
    canvas.drawLine({ mid, 280 }, { mid - 100, 250 }, 0);
    canvas.drawLine({ mid, 280 }, { mid + 100, 250 }, 0);
    canvas.drawLine({ mid, 380 }, { mid - 50, 500 }, 0);
    canvas.drawLine({ mid, 380 }, { mid + 50, 500 }, 0);
  }

};

class MyWin : public Window {
public:
  BluePanel blue;
  ManPanel man;

  void onCreate() override {
    resizeContent({ 800, 600 });
    addChild(blue, { 10, 10 }, { 100, 300 });
    addChild(man, { 200, 250 }, { 400, 600 });
  }

  void onPaint(Canvas& canvas) override {
    auto size = clientSize();
    //canvas.drawRectangle({ 0, 0 }, { size.width, size.height }, Colors::White, Colors::White);
    size = contentSize();
    int width = size.width / 100 + (size.width % 100 ? 1 : 0);
    int height = size.height / 100 + (size.height % 100 ? 1 : 0);
    for (int i = 0; i < width; ++i) {
      for (int j = 0; j < height; ++j) {
        auto color =
          ((i + j) % 2 == 0) ? Colors::White : Colors::Green;
        canvas.drawRectangle({ 100*i, 100*j }, { 100*i + 101, 100*j + 101 }, Colors::Black, color);
      }
    }
  }

};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow) {
  AllocConsole();
  FILE *oldin, *oldout, *olderr;
  freopen_s(&oldin, "CONIN$", "r", stdin);
  freopen_s(&oldout, "CONOUT$", "w", stdout);
  freopen_s(&olderr, "CONOUT$", "w", stderr);

  MyWin window;
  return window.run();
}

int main() {

}