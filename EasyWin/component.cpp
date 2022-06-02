#include "component.hpp"
#include "panel.hpp"
#include "win32dow.hpp"

namespace easywin {

  const char* Component::text() const {
    return "";
  }

  void Component::create(HWND parent, int id, Point position, Size size) {
    registerClass(className());  // no-op if already defined

    createWindow(
      className(),          // window class name
      text(),               // window caption
      style(),              // window style
      position.x,           // initial x position
      position.y,           // initial y position
      size.width,           // initial x size
      size.height,          // initial y size
      parent,               // parent window handle
      (long long)id,        // window menu handle
      this
    );

  }

  Component::~Component() {
  }

  Component::Component(Component&& src) noexcept {
    hwnd = src.hwnd;
    src.hwnd = NULL;
  }

  Component& Component::operator =(Component&& src) noexcept {
    if (this != &src) {
      if (hwnd != NULL) {
        DestroyWindow(hwnd);
      }
      hwnd = src.hwnd;
      src.hwnd = NULL;
    }
    return *this;
  }

  Size Component::clientSize() const {
    RECT client;
    GetClientRect(hwnd, &client);
    return { client.right, client.bottom };
  }
}