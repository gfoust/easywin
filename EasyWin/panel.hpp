#pragma once
#include "component.hpp"
#include "canvas.hpp"
#include "util.hpp"
#include <optional>

namespace easywin {

  class Panel : public Component {
  public:

    Panel() = default;

    Panel(Panel&& src) noexcept;
    Panel& operator =(Panel&& src) noexcept;

    void setHwnd(HWND hwnd) override;

    void create(Point position, Size size, HWND parent, long long id);
    void requestRepaint();
    bool contentSizeFixed() const { return (bool)content; }
    Size contentSize() const;
    void resizeContent(Size size);
    void startTimer(long elapseMs);
    void stopTimer();

    virtual void onCreate();
    virtual void onPaint(Canvas& canvas);
    virtual void onResize(int width, int height);
    virtual void onMouseMove(int x, int y);
    virtual void onMouseButtonDown(int x, int y);
    virtual void onMouseButtonUp(int x, int y);
    virtual void onClick(int x, int y);
    virtual void onDoubleClick(int x, int y);
    virtual void onMouseWheel(int x, int y, bool up);
    virtual void onTimer(long ellapsedMs);
    virtual void onDestroy();

  protected:

    void create(
      const char* className,
      const char* text,
      unsigned long style,
      Point position,
      Size size,
      HWND parent,
      long long id
    );

  private:
    std::optional<Size> content;
  };

  class PanelRef : public ComponentRef {
  public:
    using component_type = Panel;

    PanelRef() = default;

    PanelRef(component_type& panel) : ComponentRef{ panel } {
    }

    component_type& component() const {
      return static_cast<Panel&>(*_component);
    }

    void requestRepaint() {
      component().requestRepaint();
    }

    Size contentSize() const {
      return component().contentSize();
    }

    void resizeContent(Size size) {
      component().resizeContent(size);
    }
  };

  static_assert(component_ref<PanelRef>);
  
}