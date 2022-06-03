#pragma once
#include "panel.hpp"
#include <concepts>
#include <memory>
#include <utility>

namespace easywin {

  class Container : public Panel {
  protected:
    std::vector<std::unique_ptr<Component>> children;

    void addChild(Point position, std::unique_ptr<Component> child);

  public:

    // Child accessors

    template <typename ComponentT = Component>
      requires std::derived_from<ComponentT, Component>
    ComponentT& child(size_t i) {
      return dynamic_cast<ComponentT&>(*children.at(i));
    }

    template <typename ComponentT = Component>
      requires std::derived_from<ComponentT, Component>
    const ComponentT& child(size_t i) const {
      return dynamic_cast<const ComponentT&>(*children.at(i));
    }

    template <typename ComponentT>
      requires std::derived_from<ComponentT, Component>
    void addChild(Point position, ComponentT child) {
      addChild(position, std::make_unique<ComponentT>(std::move(child)));
    }

    template <typename ComponentT, typename... Args>
      requires std::derived_from<ComponentT, Component> 
    void addChild(Point position, Args... args) {
      addChild(position, std::make_unique<ComponentT>(std::forward(args...)));
    }

    template <typename ComponentT>
      requires std::derived_from<ComponentT, Component>
    inline void addChild(ComponentT child, Point position, Size size, const std::string& text) {
      addChild(child, position, size, text.data());
    }
  };
}