#define _WIN32_WINNT 0x500
#include "gdi_canvas.hpp"
#include <stdexcept>

namespace easywin {

  GdiCanvas::GdiCanvas(HDC hdc) : hdc(hdc) {
  }

  void GdiCanvas::drawLine(Point begin, Point end, Color lineColor) {
    if (hdc == 0) {
      throw std::logic_error("Only call drawLine from the Paint method");
    }
    SetDCPenColor(hdc, lineColor);
    MoveToEx(hdc, begin.x, begin.y, nullptr);
    LineTo(hdc, end.x, end.y);
  }

  void GdiCanvas::drawRectangle(Point topLeft, Point bottomRight, Color lineColor, Color fillColor) {
    if (hdc == 0) {
      throw std::logic_error("Only call drawRectangle from the Paint method");
    }
    SetDCPenColor(hdc, lineColor);
    SetDCBrushColor(hdc, fillColor);
    Rectangle(hdc, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
  }

  void GdiCanvas::drawEllipse(Point center, unsigned int radius,
    Color lineColor, Color fillColor) {
    if (hdc == 0) {
      throw std::logic_error("Only call drawEllipse from the Paint method");
    }
    SetDCPenColor(hdc, lineColor);
    SetDCBrushColor(hdc, fillColor);
    Ellipse(
      hdc,
      center.x - radius,
      center.y - radius,
      center.x + radius + 1,
      center.y + radius + 1
    );
  }

  void GdiCanvas::drawPolygon(const std::vector<Point>& points, Color lineColor,
    Color fillColor) {
    if (hdc == 0) {
      throw std::logic_error("Only call drawPolygon from the Paint method");
    }
    SetDCPenColor(hdc, lineColor);
    SetDCBrushColor(hdc, fillColor);
    Polygon(hdc, (POINT*)points.data(), (int)points.size());
  }

  void GdiCanvas::drawText(Point topLeft, const std::string& text,
    Color textColor, Color fillColor) {
    if (hdc == 0) {
      throw std::logic_error("Only call drawText from the Paint method");
    }
    SetBkColor(hdc, fillColor);
    TextOut(hdc, topLeft.x, topLeft.y, text.data(), (int)text.size());
  }

}  // namespace shapes