#pragma once
#include "panel.hpp"
#include "win32dow.hpp"
#include <concepts>
#include <memory>
#include <utility>

namespace easywin {

  class Container : public Panel {
  protected:
    std::vector<std::unique_ptr<Component>> children;
  
  public:

    template <typename ComponentT>
      requires std::derived_from<ComponentT, Component>
    void addChild(ComponentT child, Point position, Size size) {

      if (child.hwnd == NULL) {
        child.create(hwnd, (int)children.size() + 1, position, size);
      }
      else {
        setParent(hwnd, child.hwnd);
        setId(child.hwnd, (int)children.size());
        reposition(child.hwnd, position, size);
      }
      children.push_back(std::make_unique<ComponentT>(std::move(child)));

      Size content = contentSize();
      if (content.width < position.x + size.width) {
        content.width = position.x + size.width;
      }
      if (content.height < position.y + size.height) {
        content.height = position.y + size.height;
      }
      resizeContent(content);
    }
  };

}