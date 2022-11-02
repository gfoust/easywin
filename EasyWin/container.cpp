#include "container.hpp"
#include "win32dow.hpp"

namespace easywin {

  Component& Container::addChild(std::unique_ptr<Component> child) {
    Rect client = child->clientRect();

    _children.push_back({ std::move(child), AnchorPoints{} });

    Size content = contentSize();
    if (content.width < client.right) {
      content.width = client.right;
    }
    if (content.height < client.bottom) {
      content.height = client.bottom;
    }
    resizeContent(content);

    return *_children.back().child;
  }

  void Container::onResize(int width, int height) {
    Size containerSize = { 0, 0 };
    for (auto& rec : _children) {
      Rect childRect = rec.child->clientRect();
      Size childSize = { childRect.right - childRect.left, childRect.bottom - childRect.top };
      if (rec.anchors.left) {
        if (!rec.anchors.right) {
          childRect.right = *rec.anchors.left + childSize.width;
        }
        childRect.left = *rec.anchors.left;
      }
      if (rec.anchors.top) {
        if (!rec.anchors.bottom) {
          childRect.bottom = *rec.anchors.bottom + childSize.height;
        }
        childRect.top = *rec.anchors.top;
      }
      if (rec.anchors.right) {
        if (!rec.anchors.left) {
          childRect.left = width - *rec.anchors.right - childSize.width;
        }
        childRect.right = width - *rec.anchors.right;
      }
      if (rec.anchors.bottom) {
        if (!rec.anchors.top) {
          childRect.top = height - *rec.anchors.bottom - childSize.height;
        }
        childRect.bottom = height - *rec.anchors.bottom;
      }
      Panel* childPanel = dynamic_cast<Panel*>(rec.child.get());
      if (childPanel && childPanel->contentSizeFixed()) {
        auto childContentSize = childPanel->contentSize();
        if (childRect.right - childRect.left < childContentSize.width) {
          if (!rec.anchors.left) {
            childRect.left = childRect.right - childContentSize.width;
          }
          else {
            childRect.right = childRect.left + childContentSize.width;
          }
        }
        if (childRect.bottom - childRect.top < childContentSize.height) {
          if (!rec.anchors.top) {
            childRect.top = childRect.bottom - childContentSize.height;
          }
          else {
            childRect.bottom = childRect.top + childContentSize.height;
          }
        }
      }
      rec.child->move(childRect);
      if (containerSize.width < childRect.right) {
        containerSize.width = childRect.right;
      }
      if (containerSize.height < childRect.bottom) {
        containerSize.height = childRect.bottom;
      }
    }
    resizeContent(containerSize);
    Panel::onResize(width, height);
  }

  struct AnchorValue {
    std::optional<int> operator()(NoAnchor) {
      return std::nullopt;
    }
    std::optional<int> operator()(int i) {
      return i;
    }
  } anchorValue;

  void Container::anchor(
    const Component& child,
    std::variant<NoAnchor, int> left,
    std::variant<NoAnchor, int> top,
    std::variant<NoAnchor, int> right,
    std::variant<NoAnchor, int> bottom
  ) {
    for (auto& rec : _children) {
      if (rec.child.get() == &child) {
        Size size = clientSize();
        Rect childPos = rec.child->clientRect();
        rec.anchors.left = std::visit(anchorValue, left);
        if (rec.anchors.left) {
          childPos.left = *rec.anchors.left;
        }
        rec.anchors.top = std::visit(anchorValue, top);
        if (rec.anchors.top) {
          childPos.top = *rec.anchors.top;
        }
        rec.anchors.right = std::visit(anchorValue, right);
        if (rec.anchors.right) {
          childPos.right = size.width - *rec.anchors.right;
        }
        rec.anchors.bottom = std::visit(anchorValue, bottom);
        if (rec.anchors.bottom) {
          childPos.bottom = size.height - *rec.anchors.bottom;
        }
        rec.child->move(childPos);
      }
    }
  }

}