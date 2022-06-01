#pragma once
#undef UNICODE  
#include <windows.h>
#include "util.hpp"

namespace easywin {

  void registerClass(const char* className);

  void createWindow(
    const char* className,
    const char* text,
    unsigned long style,
    int x, int y,
    int width, int height,
    HWND parent,
    long long id,
    LPVOID param
  );

  void setComponent(HWND hwnd, class Panel* panel);
  
  void setScrollSize(HWND hwnd, Size size);
};
