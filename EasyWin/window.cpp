#include "Window.hpp"
#include "Win32dow.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

namespace easywin {

  void Window::addMenu(string name) {
    HMENU hmenu = GetMenu(hwnd);
    if (hmenu == 0) {
      hmenu = CreateMenu();
    }

    HMENU hPopup = CreateMenu();

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hPopup, name.c_str());

    if (SetMenu(hwnd, hmenu) == 0) {
      throw runtime_error("Cannot create menu");
    }
  }

  void Window::addMenuItem(int menuPos, std::string item, int id) {
    HMENU hmenu = GetMenu(hwnd);

    if (hmenu == 0) {
      throw logic_error("Called addMenuItem with invalid menuPos");
    }

    HMENU hPopup = GetSubMenu(hmenu, menuPos);

    if (hPopup == 0) {
      throw logic_error("Called addMenuItem with invalid menuPos");
    }

    AppendMenu(hPopup, MF_STRING, (UINT_PTR)id, item.c_str());
  }

  unsigned long Window::style() const {
    return WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL;
  }

  void Window::create() {
    Component::create(
      NULL,
      NULL,
      { CW_USEDEFAULT, CW_USEDEFAULT },
      { CW_USEDEFAULT, CW_USEDEFAULT }
    );
  }

  int Window::run() {
    try {
      if (hwnd == NULL) {
        create();
      }

      ShowWindow(hwnd, SW_SHOWNORMAL);

      MSG msg;
      while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      return (int)msg.wParam;
    }
    catch (std::exception& e) {
      MessageBox(NULL, e.what(), "Error", MB_ICONERROR | MB_OK);
      return -1;
    }
  }

  string Window::getTitle() {
    char buffer[200];
    GetWindowText(hwnd, buffer, 200);
    return buffer;
  }

  void Window::setTitle(const string& title) {
    SetWindowText(hwnd, title.c_str());
  }

}  // namespace easywin