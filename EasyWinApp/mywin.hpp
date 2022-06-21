#pragma once
#include "easywin.hpp"

class ManPanel : public easywin::Panel {

  using Panel::Panel;

  void onPaint(easywin::Canvas& canvas) override {
    auto sz = clientSize();
    auto mid = sz.width / 2;
    canvas.drawRectangle({ 0, 0 }, { sz.width, sz.height }, easywin::Colors::Black, easywin::Colors::White);
    canvas.drawEllipse({ mid, 200 }, 50, 0, easywin::rgb(255, 255, 255));
    canvas.drawLine({ mid, 250 }, { mid, 380 }, 0);
    canvas.drawLine({ mid, 280 }, { mid - 100, 250 }, 0);
    canvas.drawLine({ mid, 280 }, { mid + 100, 250 }, 0);
    canvas.drawLine({ mid, 380 }, { mid - 50, 500 }, 0);
    canvas.drawLine({ mid, 380 }, { mid + 50, 500 }, 0);
  }

};

class MyWin : public easywin::MainWindow {
public:
  using MainWindow::MainWindow;

  MyWin() : MainWindow("My Window") {
    addChild<ManPanel>(easywin::Point{ 100, 100 }, easywin::Size{ 250, 600 });
  }

  void onPaint(easywin::Canvas& canvas) override {
    auto size = clientSize();
    canvas.drawRectangle({ 0, 0 }, { size.width, size.height }, easywin::Colors::Black, easywin::Colors::Blue);
  }
};
