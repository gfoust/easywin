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



  class ContainerRef : public PanelRef {
  public:
    using component_type = Container;

    ContainerRef() = default;

    ContainerRef(component_type& container) : PanelRef{ container } {
    }

    component_type& component() const {
      return static_cast<Container&>(*_component);
    }

    template <std::derived_from<Component> ComponentT, typename... Args>
      requires std::constructible_from<ComponentT, Args..., HWND, long long>
    ComponentT& addChild(Args&&... args) {
      return component().addChild<ComponentT, Args...>(std::forward(args));
    }

    template <component_ref RefT, typename... Args>
      requires std::constructible_from<typename RefT::Manager, Args..., HWND, long long>
    typename RefT::Manager& addChild(Args&&... args) {
      return component().addChild<typename RefT::Manager, Args...>(std::forward(args));
    }
  };

  static_assert(component_ref<ContainerRef>);

}