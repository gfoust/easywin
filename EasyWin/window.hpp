#pragma once
#include "container.hpp"

namespace easywin {

  class Window : public Container {
  public:
    Window() = default;

    Window(const char* title) {
      create(title);
    }

    void create(const char* title);
  };

  class MainWindow : public Window {
  public:
    using Window::Window;

    void onDestroy() override;
    void run();
  };

  class WindowRef : public ContainerRef {
  public:
    using component_type = Window;

    WindowRef() = default;

    WindowRef(component_type& window) : ContainerRef{ window } {
    }

    component_type& component() const {
      return static_cast<Window&>(*_component);
    }
  };

  static_assert(component_ref<WindowRef>);

}