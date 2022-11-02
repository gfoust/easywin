#pragma once
#include "container.hpp"

namespace easywin {

  class Window : public Container {
  public:
    Window() = default;

    void create(const char* title);
  };

  class MainWindow : public Window {
  public:
    using Window::Window;

    void onDestroy() override;
    void run();
  };

}