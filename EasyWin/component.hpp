#pragma once
#include "canvas.hpp"
#include <string>

typedef struct HWND__* HWND;

namespace easywin {

  // Component - HWND
  // +- Button
  // +- TextEdit
  // +- Panel
  //    + Container
  //      + Window

  class Component {

  protected:
    HWND hwnd = NULL;

    virtual const char* className() const = 0;
    virtual unsigned long style() const = 0;
    virtual Size size(const char* text) const;
    virtual void create(HWND parent, int id, Point position, Size size, const char* text);

  public:
    Component() = default;
    Component(const Component&) = delete;
    Component& operator =(const Component&) = delete;
    Component(Component&&) noexcept;
    Component& operator =(Component&&) noexcept;
    virtual ~Component();
    Size clientSize() const;

    friend class Container;
  };

}