#pragma once
#include "panel.hpp"
#include <concepts>
#include <memory>
#include <optional>
#include <utility>
#include <variant>

namespace easywin {

  struct NoAnchor {
  } const noAnchor;

  class Container : public Panel {
  public:
    using Panel::Panel;

    template <typename ComponentT, typename... Args>
      requires component_created_from<ComponentT, Args...>
    ComponentT& addChild(Args&&... args) {
      auto component = std::make_unique<ComponentT>();
      component->create(std::forward<Args>(args)..., _hwnd, (long long)_children.size() + 1);
      return static_cast<ComponentT&>(addChild(std::move(component)));
    }

    void anchor(
      const Component& child, 
      std::variant<NoAnchor, int> left,
      std::variant<NoAnchor, int> top,
      std::variant<NoAnchor, int> right,
      std::variant<NoAnchor, int> bottom
    );

    Component& operator[](std::size_t i) {
      return *_children.at(i).child;
    }

    const Component& operator[](std::size_t i) const {
      return *_children.at(i).child;
    }

    void onResize(int width, int height) override;

  protected:
    struct AnchorPoints {
      std::optional<int> left, top, right, bottom;
    };

    struct ChildRec {
      std::unique_ptr<Component> child;
      AnchorPoints anchors;
    };

    std::vector<ChildRec> _children;

    Component& addChild(std::unique_ptr<Component> child);
  };

}