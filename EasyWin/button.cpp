#include "button.hpp"
#include "win32dow.hpp"

namespace easywin {

  static void noop() {
  }

  const char* Button::className() const {
    return "button";
  }

  unsigned long Button::style() const {
    return BS_PUSHBUTTON | WS_CHILDWINDOW | WS_VISIBLE;
  }

  Button::Button() : clickHandler{ noop } {
  }

}