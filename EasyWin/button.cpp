#include "button.hpp"
#include "win32dow.hpp"

namespace easywin {

  ButtonManager::ButtonManager(const char* text, Point position, Size size, HWND parent, long long id) {
    create("button", text, BS_PUSHBUTTON | WS_CHILDWINDOW | WS_VISIBLE, position, size, parent, id);
  }

}