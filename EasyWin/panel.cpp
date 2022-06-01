#include "panel.hpp"
#include "win32dow.hpp"
#include <typeinfo>

namespace easywin {

  const char* Panel::className() const {
    return typeid(*this).name() + 6;
  }

  unsigned long Panel::style() const {
    return WS_CHILDWINDOW | WS_VISIBLE;
  }

  void Panel::requestRepaint() {
    InvalidateRect(hwnd, NULL, true);
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
    if (
      !content ||
      size.width != content->width || 
      size.height != content->height
    ) {
      content = size;
      setScrollSize(hwnd, size);
    }
  }

  void Panel::onCreate() {
    //RECT client;
    //GetClientRect(hwnd, &client);

    //int rows = client.bottom / extents.yChar;

    //SCROLLINFO si;
    //si.cbSize = sizeof(SCROLLINFO);
    //si.fMask = SIF_RANGE | SIF_PAGE;
    //si.nMin = 0;
    //si.nMax = rows;
    //si.nPage = rows;
    //SetScrollInfo(hwnd, SB_VERT, &si, true);
  }

  void Panel::onDestroy() {

  }

  void Panel::onPaint(Canvas& canvas) {
    RECT client;
    GetClientRect(hwnd, &client);

    canvas.drawRectangle({ 0, 0 }, { client.right, client.bottom }, 0xffffff, 0xffffff);
  }

  void Panel::onResize(int width, int height) {
    setScrollSize(hwnd, contentSize());
  }

  void Panel::onMouseMove(int x, int y) {
  }

  void Panel::onMouseButtonDown(int x, int y) {
  }

  void Panel::onMouseButtonUp(int x, int y) {
  }

  void Panel::onMouseButtonDoubleClick(int x, int y) {
  }

  void Panel::onMouseWheel(int x, int y, bool up) {
  }


}