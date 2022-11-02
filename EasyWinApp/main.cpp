#include "easywin.hpp"
#include <string>

using easywin::Colors;

int main() {

  easywin::MainWindow window;
  window.create("It's a GUI!");
  window.run();

}
