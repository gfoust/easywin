#pragma once
#include "util.hpp"
#include <string>
#include <vector>

namespace easywin {

  using Color = unsigned long;

  constexpr Color rgb(int red, int green, int blue) {
    return (red & 0xFF) | ((green & 0xFF) << 8) | ((blue & 0xFF) << 16);
  }

  struct Colors {
    static constexpr Color Black = 0;
    static constexpr Color White = rgb(255, 255, 255);
    static constexpr Color Gray = rgb(127, 127, 127);
    static constexpr Color Red = rgb(204, 0, 0);
    static constexpr Color Orange = rgb(221, 127, 0);
    static constexpr Color Yellow = rgb(238, 238, 0);
    static constexpr Color Green = rgb(0, 204, 0);
    static constexpr Color Cyan = rgb(0, 204, 204);
    static constexpr Color Blue = rgb(0, 0, 204);
    static constexpr Color Purple = rgb(127, 0, 204);
    static constexpr Color Magenta = rgb(204, 0, 204);
    static constexpr Color Pink = rgb(238, 127, 204);
  };

  class Canvas {
  public:
    virtual ~Canvas() { }

    virtual void drawLine(
      Point begin,
      Point end,
      Color lineColor
    ) = 0;

    virtual void drawRectangle(
      Point topLeft,
      Point bottomRight,
      Color lineColor,
      Color fillColor
    ) = 0;

    virtual void drawEllipse(
      Point center,
      unsigned int radius,
      Color lineColor,
      Color fillColor
    ) = 0;

    virtual void drawPolygon(
      const std::vector<Point>& points,
      Color lineColor,
      Color fillColor
    ) = 0;

    virtual void drawText(
      Point topLeft,
      const std::string& text,
      Color textColor,
      Color fillColor
    ) = 0;
  };

}  // namespace easywin
