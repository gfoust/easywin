#include "container.hpp"
#include <stdexcept>
#include <utility>

namespace easywin {

  void Container::addChild(Point position, std::unique_ptr<Component> child) {
    if (child->hwnd == NULL) {
      throw std::logic_error("Attempting to add partially formed child");
      //child.create(hwnd, (int)children.size() + 1, position, size, text);
    }

    Size size = child->clientSize();

    setParent(child->hwnd, hwnd);
    setId(child->hwnd, (int)children.size() + 1);
    reposition(child->hwnd, position, size);
    children.emplace_back(std::move(child));

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