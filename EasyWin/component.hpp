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
    void move(Rect client);
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

  template <typename ComponentT, typename... Args>
  concept component_created_from =
    std::derived_from<ComponentT, Component> &&
    requires (ComponentT component) {
      component.create(std::declval<Args>()..., HWND{}, 0);
    };
}
