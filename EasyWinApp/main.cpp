#include "easywin.hpp"
#include <string>

class MyWindow : public easywin::MainWindow {
  void onPaint(easywin::Canvas&) override;
};

void MyWindow::onPaint(easywin::Canvas& canvas) {
  canvas.drawEllipse({ 100, 100 }, 50, 0, 0);
}

int main() {

  MyWindow window;
  window.create("Cool Grapes");
  window.run();

}
