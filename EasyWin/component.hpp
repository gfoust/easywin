#pragma once
#pragma once
#include "util.hpp"
#include <concepts>
#include <string>

typedef struct HWND__* HWND;

namespace easywin {

  class Component {
  public:
    Component() = default;

    Component(const Component&) = delete;
    Component& operator =(const Component&) = delete;

    Component(Component&& src) noexcept;
    Component& operator =(Component&& src) noexcept;

    virtual void setHwnd(HWND hwnd);
    virtual ~Component();

    Size clientSize() const;
    Rect clientRect() const;
    std::string text() const;
    void setText(const char* text);
    void setText(const std::string& text) {
      setText(text.data());
    }

  protected:
    HWND _hwnd = nullptr;

    void create(
      const char* className,
      const char* text,
      unsigned long style,
      Point position,
      Size size,
      HWND parent,
      long long id
    );
    void destroy();
  };

  class ComponentRef {
  public:
    using component_type = Component;

    ComponentRef() = default;

    ComponentRef(component_type& component) : _component{ &component } {
    }

    component_type& component() const {
      return *_component;
    }

    Size clientSize() const {
      return component().clientSize();
    }

    Rect clientRect() const {
      return component().clientRect();
    }

    std::string text() const {
      return component().text();
    }

    void setText(const char* text) {
      component().setText(text);
    }

    void setText(const std::string& text) {
      component().setText(text.data());
    }

  protected:
    component_type* _component = nullptr;
  };

  template <typename RefT>
  concept component_ref =
    std::derived_from<RefT, ComponentRef> &&
    std::derived_from<typename RefT::component_type, Component> &&
    std::constructible_from<RefT, typename RefT::component_type&> &&
    std::same_as<typename RefT::component_type&, decltype(std::declval<RefT>().component())>;
  static_assert(component_ref<ComponentRef>);
}
