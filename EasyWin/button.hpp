#pragma once
#include "component.hpp"
#include <functional>

namespace easywin {

  class ButtonManager : public Component {

  public:
    std::function<void()> onClick;

    ButtonManager(const char* text, Point position, Size size, HWND parent, long long id);
  };


  class ButtonRef : public ComponentRef {
  public:
    using component_type = ButtonManager;

    ButtonRef() = default;
    ButtonRef(component_type& component) : ComponentRef{ component } {
    }

    component_type& component() const {
      return static_cast<ButtonManager&>(*_component);
    }

  };

  static_assert(component_ref<ButtonRef>);

}