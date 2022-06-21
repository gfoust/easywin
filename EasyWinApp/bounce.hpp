#pragma once
#include "easywin.hpp"

struct State {
  int width = 0, height = 0;
  double r = 20;
  double x = 20, y = 20;
  double dx = 0.2, dy = 0.15;
};

void createHandler(easywin::Window& window, State& state) {
  window.startTimer(20);
}

void resizeHandler(int width, int height, State& state) {
  state.width = width - state.r;
  state.height = height - state.r;
}

void paintHandler(easywin::Canvas& canvas, State& state) {
  canvas.drawEllipse({ (long)state.x, (long)state.y }, state.r, easywin::Colors::Black, easywin::Colors::Blue);
}

void timerHandler(int elapsedMs, State& state) {
  state.x += state.dx * elapsedMs;
  state.y += state.dy * elapsedMs;
  if (state.x < state.r) {
    state.x = 2*state.r - state.x;
    state.dx = -state.dx;
  }
  if (state.x > state.width) {
    state.x = 2 * state.width - state.x;
    state.dx = -state.dx;
  }
  if (state.y < state.r) {
    state.y = 2*state.r - state.y;
    state.dy = -state.dy;
  }
  if (state.y > state.height) {
    state.y = 2 * state.height - state.y;
    state.dy = -state.dy;
  }
}
