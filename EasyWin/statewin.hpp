#pragma once
#include "panel.hpp"
#include <concepts>
#include <tuple>
#include <type_traits>

namespace easywin {

  template <typename... StateTs>
  concept has_basic_paint = requires(Canvas& canvas) {
    paintHandler(canvas, std::declval<StateTs&>()...);
  };

  template <typename... StateTs>
  concept has_extended_paint = requires(Panel& panel, Canvas& canvas) {
    paintHandler(panel, canvas, std::declval<StateTs&>()...);
  };

  template <typename... StateTs>
  concept has_paint = has_basic_paint<StateTs...> || has_extended_paint<StateTs...>;


  template <typename... StateTs>
  concept has_basic_mouse_move = requires(int x, int y) {
    mouseMoveHandler(x, y, std::declval<StateTs&>()...);
  };

  template <typename... StateTs>
  concept has_extended_mouse_move = requires(Panel panel, int x, int y) {
    mouseMoveHandler(panel, x, y, std::declval<StateTs&>()...);
  };

  template <typename...StateTs>
  concept has_mouse_move = has_basic_mouse_move<StateTs...> || has_extended_mouse_move<StateTs...>;


  template <std::derived_from<Panel> PanelT, typename... StateTs>
  class StateComponent : public PanelT {
    std::tuple<StateTs...> states;

  public:
    StateComponent(StateTs... states)
      : states{ std::forward<StateTs>(states)... } {
    }

    void onPaint(Canvas& canvas) {
      if constexpr (has_extended_paint<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            paintHandler(*this, canvas, states...);
          }, states
        );
      }
      else if constexpr (has_basic_paint<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            paintHandler(canvas, states...);
          }, states
        );
      }
    }

    void onMouseMove(int x, int y) {
      if constexpr (has_extended_mouse_move<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            mouseMoveHandler(*this, x, y, states...);
          }, states
        );
        this->requestRepaint();
      }
      else if constexpr (has_basic_mouse_move<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            mouseMoveHandler(x, y, states...);
          }, states
        );
        this->requestRepaint();
      }
    }
  };

  template <typename... StateTs>
  using StateWindow = StateComponent<MainWindow, StateTs...>;
}