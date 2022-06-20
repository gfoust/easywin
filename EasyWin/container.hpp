#pragma once
#include "panel.hpp"
#include <memory>
#include <concepts>
#include <utility>

namespace easywin {

  class Container : public Panel {
  public:
    using Panel::Panel;

    template <std::derived_from<Component> ComponentT, typename... Args>
      requires std::constructible_from<ComponentT, Args..., HWND, long long>
    ComponentT& addChild(Args&&... args) {
      return static_cast<ComponentT&>(addChild(
        std::make_unique<ComponentT>(std::forward<Args>(args)..., _hwnd, (long long)_children.size() + 1)
      ));
    }

    template <component_ref RefT, typename... Args>
      requires std::constructible_from<typename RefT::Manager, Args..., HWND, long long>
    typename RefT::Manager& addChild(Args&&... args) {
      return addChild(
        std::make_unique<typename RefT::Manager>(std::forward<Args>(args)..., _hwnd, (long long)_children.size() + 1)
      );
    }

  protected:
    std::vector<std::unique_ptr<Component>> _children;

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