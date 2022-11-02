#pragma once
#include "easywin.hpp"
#include <vector>

using Points = std::vector<easywin::Point>;

struct PointsState {
  bool dragging = false;
  Points points;
};

void paintHandler(easywin::Canvas& canvas, PointsState& state) {
  for (auto& point : state.points) {
    canvas.drawEllipse(point, 10, 0, 0x00dd00);
  }
}

void createHandler(PointsState& state) {
  state.points.push_back({ 0, 0 });
}

//void mouseButtonDownHandler(int x, int y, PointsState& state) {
//  state.dragging = true;
//  state.points.push_back({ x, y });
//}
//
//void mouseButtonUpHandler(int x, int y, PointsState& state) {
//  state.dragging = false;
//}

void mouseMoveHandler(int x, int y, PointsState& state) {
  state.points.back() = { x, y };
}

//void clickHandler(int x, int y, PointsState& state) {
//  state.points.push_back({ x, y });
//}

void doubleClickHandler(int x, int y, PointsState& state) {
  state.points.push_back({ x, y });
}
