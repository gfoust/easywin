#include "component.hpp"
#include "win32dow.hpp"
#include <stdexcept>

namespace easywin {

  Component::Component(Component&& src) noexcept : _hwnd{ src._hwnd } {
    src._hwnd = nullptr;
  }

  Component& Component::operator =(Component&& src) noexcept {
    if (this != &src) {
      destroy();
      _hwnd = src._hwnd;
      src._hwnd = nullptr;
    }
    return *this;
  }

  Size Component::clientSize() const {
    return impl::getClientSize(_hwnd);
  }

  Rect Component::clientRect() const {
    return impl::getClientRect(_hwnd);
  }

  std::string Component::text() const {
    return impl::getWindowText(_hwnd);
  }

  void Component::setText(const char* text) {
    impl::setWindowText(_hwnd, text);
  }

  void Component::setHwnd(HWND hwnd) {
    if (_hwnd != hwnd) {
      destroy();
      _hwnd = hwnd;
    }
  }

  Component::~Component() {
    destroy();
  }

  void Component::create(
    const char* className,
    const char* text,
    unsigned long style,
    Point position,
    Size size,
    HWND parent,
    long long id
  ) {
    if (parent == NULL && style & WS_CHILDWINDOW) {
      throw std::logic_error("Child window requires parent");
    }
    if (parent != NULL && !(style & WS_CHILDWINDOW)) {
      throw std::logic_error("Parent requires child window");
    }

    impl::registerClass(className);  // no-op if already defined

    _hwnd = impl::createWindow(
      className,            // window class name
      text,                 // window caption
      style,                // window style
      position.x,           // initial x position
      position.y,           // initial y position
      size.width,           // initial x size
      size.height,          // initial y size
      parent,               // parent window handle
      (long long)id         // window menu handle
    );
  }

  void Component::destroy() {
    if (_hwnd) {
      impl::destroyWindow(_hwnd);
    }
    _hwnd = nullptr;
  }

}