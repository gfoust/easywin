#pragma once
#include "easywin.hpp"
#include <vector>

using Points = std::vector<easywin::Point>;

struct PointsState {
  Points points;
};

void paintHandler(easywin::Canvas& canvas, PointsState& state) {
  for (auto& point : state.points) {
    canvas.drawEllipse(point, 10, 0, 0x00dd00);
  }
}

void clickHandler(int x, int y, PointsState& state) {
  state.points.push_back({ x, y });
}

