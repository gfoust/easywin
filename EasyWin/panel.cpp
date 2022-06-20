#include "panel.hpp"
#include "win32dow.hpp"

namespace easywin {

  Panel::Panel(Panel&& src) noexcept : Component{ std::move(src) } {
    impl::setPanel(_hwnd, *this);
  }

  Panel& Panel::operator =(Panel&& src) noexcept {
    Component::operator =(std::move(src));
    impl::setPanel(_hwnd, *this);
    return *this;
  }

  void Panel::setHwnd(HWND hwnd) {
    Component::setHwnd(hwnd);
    impl::setPanel(hwnd, *this);
  }

  void Panel::requestRepaint() {
    impl::requestRepaint(_hwnd);
  }

  Size Panel::contentSize() const {
    if (content) {
      return *content;
    }
    else {
      return clientSize();
    }
  }

  void Panel::resizeContent(Size size) {
    if (!content || size.width != content->width || size.height != content->height) {
      content = size;
      impl::setScrollSize(_hwnd, size);
    }
  }

  void Panel::onCreate() {
  }

  void Panel::onPaint(Canvas& canvas) {
    auto client = clientSize();
    canvas.drawRectangle({ 0, 0 }, { client.width, client.height }, 0xffffff, 0xffffff);
  }

  void Panel::onResize(int width, int height) {
    impl::setScrollSize(_hwnd, contentSize());
  }

  void Panel::onMouseMove(int x, int y) {
  }

  void Panel::onMouseButtonDown(int x, int y) {
  }

  void Panel::onMouseButtonUp(int x, int y) {
  }

  void Panel::onClick(int x, int y) {
  }

  void Panel::onDoubleClick(int x, int y) {
  }

  void Panel::onMouseWheel(int x, int y, bool up) {
  }

  void Panel::onDestroy() {
  }

  void Panel::create(Point position, Size size, HWND parent, long long id) {
    create("Panel", "", WS_CHILDWINDOW | WS_VISIBLE, position, size, parent, id);
  }

  void Panel::create(
    const char* className,
    const char* text,
    unsigned long style,
    Point position,
    Size size,
    HWND parent,
    long long id
  ) {
    Component::create(className, text, style, position, size, parent, id);
    impl::setPanel(_hwnd, *this);
    onCreate();
  }

}