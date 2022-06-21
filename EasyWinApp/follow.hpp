#pragma once
#include "easywin.hpp"

void paintHandler(easywin::Canvas& canvas, int cx, int cy) {
  canvas.drawEllipse({ cx, cy }, 50, easywin::Colors::Black, easywin::Colors::Blue);
}

void mouseMoveHandler(int mx, int my, int& cx, int& cy) {
  cx = mx;
  cy = my;
}
