#include "easywin.hpp"
using easywin::ButtonRef;
using easywin::Canvas;
using easywin::Colors;
using easywin::MainWindow;
using easywin::Panel;
using easywin::Size;
using easywin::Point;

class ManPanel : public Panel {

  using Panel::Panel;

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

class MyWin : public MainWindow {
public:
  using MainWindow::MainWindow;

  MyWin() : MainWindow("My Window") {
    addChild<ManPanel>(Point{ 100, 100 }, Size{ 250, 600 });
  }

  void onPaint(Canvas& canvas) override {
    auto size = clientSize();
    canvas.drawRectangle({ 0, 0 }, { size.width, size.height }, Colors::Black, Colors::Blue);
  }
};

void paintHandler(Canvas& canvas, int cx, int cy) {
  canvas.drawEllipse({ cx, cy }, 50, Colors::Black, Colors::Blue);
}

void mouseMoveHandler(int mx, int my, int& cx, int& cy) {
  cx = mx;
  cy = my;
}

#include "statewin.hpp"

static_assert(easywin::has_paint<int, int>);
static_assert(easywin::has_mouse_move<int, int>);

struct State {
  int width, height;
  double r, x, y, dx, dy;
};

void paintHandler(Canvas& canvas, State& state) {
  canvas.drawEllipse({ (long)state.x, (long)state.y }, 20, Colors::Black, Colors::Blue);
}

static_assert(easywin::has_paint<State>);

int main() {
  //MyWin window("Hello, world");
  //window.addChild<Button>("Click Me", easywin::Point{ 100, 100 }, easywin::Size{ 75, 25 });
  //window.addChild<ManPanel>(Point{ 100, 100 }, Size{ 250, 500 });
  //window.addChild<ManPanel>(Point{ 400, 200 }, Size{ 250, 500 });
  //easywin::StateWindow<int, int> window{ 200, 200 };
  State state;
  state.width = state.height = 0;
  state.x = state.y = state.r = 20;
  state.dx = 1;
  state.dy = 2;

  easywin::StateWindow<State> window{ state };
  window.create("Something Cool");
  window.run();
}
