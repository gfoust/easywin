#pragma once
#include "component.hpp"
#include <functional>

namespace easywin {

  class Button : public Component {
    
    const char* className() const override;
    unsigned long style() const override;

  public:

    std::function<void()> clickHandler;

    Button();
    Button(std::function<void()> clickHandler)
      : clickHandler{ std::move(clickHandler) } {
    }

  };

}