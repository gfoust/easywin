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
    void requestRepaint();

  public:
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
  };

}
