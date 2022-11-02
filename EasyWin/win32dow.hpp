#pragma once
#define _WIN32_WINNT 0x500
#undef UNICODE  
#include <windows.h>
#include <string>
#include "panel.hpp"
#include "util.hpp"

namespace easywin::impl {

  void registerClass(const char* className);

  HWND createWindow(
    const char* className,
    const char* text,
    unsigned long style,
    int x, int y,
    int width, int height,
    HWND parent,
    long long id,
    Component* component
  );

  inline
  void destroyWindow(HWND hwnd) {
    DestroyWindow(hwnd);
  }

  inline
  void setPanel(HWND hwnd, Panel& panel) {
    SetWindowLongPtr(hwnd, 0, (LONG_PTR)&panel);
  }

  inline
  void requestRepaint(HWND hwnd) {
    InvalidateRect(hwnd, nullptr, true);
  }

  inline
  void terminateApplication() {
    PostQuitMessage(0);
  }

  inline
  void setWindowText(HWND hwnd, const char* text) {
    SetWindowText(hwnd, text);
  }

  inline
  void moveWindow(HWND hwnd, Rect rect) {
    MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
  }

  Size getClientSize(HWND hwnd);
  Rect getClientRect(HWND hwnd);
  std::string getWindowText(HWND hwnd);
  void setScrollSize(HWND hwnd, Size size);
  int runWindow(HWND hwnd);
  size_t startTimer(HWND hwnd, long elapseMs);
  void stopTimer(HWND hwnd);
}