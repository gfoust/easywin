#include "window.hpp"
#include "win32dow.hpp"

namespace easywin {

  void Window::create(const char* title) {
    Container::create(
      "Window",
      title,
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL,
      { CW_USEDEFAULT, CW_USEDEFAULT },
      { CW_USEDEFAULT, CW_USEDEFAULT },
      NULL,
      0
    );
  }

  void MainWindow::onDestroy() {
    impl::terminateApplication();
  }

  void MainWindow::run() {
    impl::runWindow(_hwnd);
  }
}