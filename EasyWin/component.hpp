#pragma once
#include "win32dow.hpp"
#include "canvas.hpp"

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
    virtual const char* text() const;
    virtual void create(HWND parent, int id, Point position, Size size);

  public:
    Component() = default;
    Component(const Component&) = delete;
    Component(Component&&) noexcept;
    Component& operator =(const Component&) = delete;
    Component& operator =(Component&&) noexcept;
    virtual ~Component();
    Size clientSize() const;

    friend class Container;
  };

}