#pragma once
#include "component.hpp"
#include <functional>

namespace easywin {

  class ButtonManager : public Component {

  public:
    std::function<void()> onClick;

    ButtonManager(const char* text, Point position, Size size, HWND parent, long long id);
  };

}