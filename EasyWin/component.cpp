#include "component.hpp"
#include "panel.hpp"
#include "win32dow.hpp"
#include <stdexcept>

namespace easywin {

  Size Component::size(const char* text) const {
    return { 10, 10 };
  }

  void Component::create(HWND parent, int id, Point position, Size size, const char* text) {
    if (hwnd != NULL) {
      throw std::logic_error("Attempting to re-create component");
    }
    
    registerClass(className());  // no-op if already defined

    createWindow(
      className(),          // window class name
      text,                 // window caption
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