#pragma once
#include "panel.hpp"

namespace easywin {

  class Container : public Panel {
  protected:
    std::vector<Component*> children;
  
  public:

    void addChild(Component& child, Point position, Size size);
  };

}