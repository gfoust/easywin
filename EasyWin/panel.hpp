#pragma once
#include "component.hpp"
#include "win32dow.hpp"
#include <optional>

namespace easywin {

  class Panel : public Component {
    std::optional<Size> content;

  protected:
    const char* className() const override;
    unsigned long style() const override;
    using Component::create;
    void create(Size size) {
      Component::create(NULL, 0, Point{ 0, 0 }, size, "");
    }

  public:
    Panel() = default;
    Panel(Panel&& src) noexcept;
    Panel& operator =(Panel&& src) noexcept;

    void requestRepaint();
    Size contentSize() const;
    void resizeContent(Size size);
    virtual void onCreate();
    virtual void onDestroy();
    virtual void onPaint(Canvas& canvas);
    virtual void onResize(int width, int height);
    virtual void onMouseMove(int x, int y);
    virtual void onMouseButtonDown(int x, int y);
    virtual void onMouseButtonUp(int x, int y);
    virtual void onMouseButtonDoubleClick(int x, int y);
    virtual void onMouseWheel(int x, int y, bool up);
    
    friend BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
    friend class Container;
  };

}
