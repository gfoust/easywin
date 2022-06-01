#include "container.hpp"
#include <stdexcept>

namespace easywin {
  void Container::addChild(Component& child, Point position, Size size) {
    if (child.hwnd != NULL) {
      throw std::logic_error("Attempted to add child that already had a parent");
    }

    children.push_back(&child);
    child.create(hwnd, (int)children.size(), position, size);

    Size content = contentSize();
    if (content.width < position.x + size.width) {
      content.width = position.x + size.width;
    }
    if (content.height < position.y + size.height) {
      content.height = position.y + size.height;
    }
    resizeContent(content);
  }
}