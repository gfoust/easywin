#pragma once
#undef UNICODE
#include <windows.h>
#include "canvas.hpp"

namespace easywin {

  class GdiCanvas : public Canvas {
    HDC hdc;

  public:
    GdiCanvas(HDC hdc);

    void drawLine(
      Point begin,
      Point end,
      Color lineColor
    ) override;

    void drawRectangle(
      Point topLeft,
      Point bottomRight,
      Color lineColor,
      Color fillColor
    ) override;

    void drawEllipse(
      Point center,
      unsigned int radius,
      Color lineColor,
      Color fillColor
    ) override;

    void drawPolygon(
      const std::vector<Point>& points,
      Color lineColor,
      Color fillColor
    ) override;

    void drawText(
      Point topLeft,
      const std::string& text,
      Color textColor,
      Color fillColor
    ) override;
  };

}  // namespace easywin
