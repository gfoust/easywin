#include "container.hpp"

namespace easywin {

  Component& Container::addChild(std::unique_ptr<Component> child) {
    Rect client = child->clientRect();

    _children.push_back(std::move(child));

    Size content = contentSize();
    if (content.width < client.right) {
      content.width = client.right;
    }
    if (content.height < client.bottom) {
      content.height = client.bottom;
    }
    resizeContent(content);

    return *_children.back();
  }


}