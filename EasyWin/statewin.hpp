#pragma once
#include "panel.hpp"
#include <concepts>
#include <tuple>
#include <type_traits>

namespace easywin {

  /*--------------------------------------------------------
   * Create
   */
  template <typename... StateTs>
  concept has_basic_create = requires {
    createHandler(std::declval<StateTs&>()...);
  };

  template <typename PanelT, typename... StateTs>
  concept has_extended_create =
    std::derived_from<PanelT, Panel> &&
    requires(PanelT& panel) {
      createHandler(panel, std::declval<StateTs&>()...);
    };

  template <typename... Args>
  concept has_create = has_basic_create<Args...> || has_extended_create<Args...>;


  /*--------------------------------------------------------
   * Paint
   */
  template <typename... StateTs>
  concept has_basic_paint = requires(Canvas& canvas) {
    paintHandler(canvas, std::declval<StateTs&>()...);
  };

  template <typename PanelT, typename... StateTs>
  concept has_extended_paint = 
    std::derived_from<PanelT, Panel> && 
    requires(PanelT& panel, Canvas& canvas) {
      paintHandler(panel, canvas, std::declval<StateTs&>()...);
    };

  template <typename... Args>
  concept has_paint = has_basic_paint<Args...> || has_extended_paint<Args...>;


  /*--------------------------------------------------------
   * Resize
   */
  template <typename... StateTs>
  concept has_basic_resize = requires {
    resizeHandler(0, 0, std::declval<StateTs&>()...);
  };

  template <typename PanelT, typename... StateTs>
  concept has_extended_resize =
    std::derived_from<PanelT, Panel> &&
    requires(PanelT & panel) {
      paintHandler(panel, 0, 0, std::declval<StateTs&>()...);
    };

  template <typename... Args>
  concept has_resize = has_basic_resize<Args...> || has_extended_resize<Args...>;


  /*--------------------------------------------------------
   * MouseMove
   */
  template <typename... StateTs>
  concept has_basic_mouse_move = requires(int x, int y) {
    mouseMoveHandler(x, y, std::declval<StateTs&>()...);
  };

  template <typename PanelT, typename... StateTs>
  concept has_extended_mouse_move = 
    std::derived_from<PanelT, Panel> &&
    requires(PanelT& panel, int x, int y) {
      mouseMoveHandler(panel, x, y, std::declval<StateTs&>()...);
    };

  template <typename... Args>
  concept has_mouse_move = has_basic_mouse_move<Args...> || has_extended_mouse_move<Args...>;


  /*--------------------------------------------------------
   * MouseDown
   */
  template <typename... StateTs>
  concept has_basic_mouse_button_down = requires(int x, int y) {
    mouseButtonDownHandler(x, y, std::declval<StateTs&>()...);
  };

  template <typename PanelT, typename... StateTs>
  concept has_extended_mouse_button_down =
    std::derived_from<PanelT, Panel> &&
    requires(PanelT & panel, int x, int y) {
      mouseButtonDownHandler(panel, x, y, std::declval<StateTs&>()...);
    };

  template <typename... Args>
  concept has_mouse_button_down = has_basic_mouse_button_down<Args...> || has_extended_mouse_button_down<Args...>;


  /*--------------------------------------------------------
   * MouseUp
   */
  template <typename... StateTs>
  concept has_basic_mouse_button_up = requires(int x, int y) {
    mouseButtonUpHandler(x, y, std::declval<StateTs&>()...);
  };

  template <typename PanelT, typename... StateTs>
  concept has_extended_mouse_button_up =
    std::derived_from<PanelT, Panel> &&
    requires(PanelT & panel, int x, int y) {
      mouseButtonUpHandler(panel, x, y, std::declval<StateTs&>()...);
    };

  template <typename... Args>
  concept has_mouse_button_up = has_basic_mouse_button_up<Args...> || has_extended_mouse_button_up<Args...>;


  /*--------------------------------------------------------
   * Click
   */
  template <typename... StateTs>
  concept has_basic_click = requires(int x, int y) {
    clickHandler(x, y, std::declval<StateTs&>()...);
  };

  template <typename PanelT, typename... StateTs>
  concept has_extended_click =
    std::derived_from<PanelT, Panel> &&
    requires(PanelT & panel, int x, int y) {
    clickHandler(panel, x, y, std::declval<StateTs&>()...);
  };

  template <typename... Args>
  concept has_click = has_basic_click<Args...> || has_extended_click<Args...>;


  /*--------------------------------------------------------
   * DoubleClick
   */
  template <typename... StateTs>
  concept has_basic_double_click = requires(int x, int y) {
    doubleClickHandler(x, y, std::declval<StateTs&>()...);
  };

  template <typename PanelT, typename... StateTs>
  concept has_extended_double_click =
    std::derived_from<PanelT, Panel> &&
    requires(PanelT & panel, int x, int y) {
    doubleClickHandler(panel, x, y, std::declval<StateTs&>()...);
  };

  template <typename... Args>
  concept has_double_click = has_basic_double_click<Args...> || has_extended_double_click<Args...>;


  /*--------------------------------------------------------
   * Timer
   */
  template <typename... StateTs>
  concept has_basic_timer = requires {
    timerHandler(0, std::declval<StateTs&>()...);
  };

  template <typename PanelT, typename... StateTs>
  concept has_extended_timer =
    std::derived_from<PanelT, Panel> &&
    requires(PanelT & panel) {
      timerHandler(panel, 0, std::declval<StateTs&>()...);
    };

  template <typename... Args>
  concept has_timer = has_basic_timer<Args...> || has_extended_timer<Args...>;


  /*========================================================
   * StateComponent
   */
  template <std::derived_from<Panel> PanelT, typename... StateTs>
  class StateComponent : public PanelT {
    std::tuple<StateTs...> states;

  public:
    StateComponent(StateTs... states)
      : states{ std::forward<StateTs>(states)... } {
    }

    void onCreate() override {
      if constexpr (has_extended_create<PanelT, StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            createHandler(*this, states...);
          }, states
        );
      }
      else if constexpr (has_basic_create<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            createHandler(states...);
          }, states
        );
      }
    }


    void onPaint(Canvas& canvas) override {
      PanelT::onPaint(canvas);
      if constexpr (has_extended_paint<PanelT, StateTs...>) {
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

    void onResize(int width, int height) override {
      PanelT::onResize(width, height);
      if constexpr (has_extended_resize<PanelT, StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            resizeHandler(*this, width, height, states...);
          }, states
        );
        this->requestRepaint();
      }
      else if constexpr (has_basic_resize<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            resizeHandler(width, height, states...);
          }, states
        );
        this->requestRepaint();
      }
    }

    void onMouseMove(int x, int y) override {
      if constexpr (has_extended_mouse_move<PanelT, StateTs...>) {
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

    void onMouseButtonDown(int x, int y) override {
      if constexpr (has_extended_mouse_button_down<PanelT, StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            mouseButtonDownHandler(*this, x, y, states...);
          }, states
        );
        this->requestRepaint();
      }
      else if constexpr (has_basic_mouse_button_down<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            mouseButtonDownHandler(x, y, states...);
          }, states
        );
        this->requestRepaint();
      }
    }

    void onMouseButtonUp(int x, int y) override {
      if constexpr (has_extended_mouse_button_up<PanelT, StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            mouseButtonUpHandler(*this, x, y, states...);
          }, states
        );
        this->requestRepaint();
      }
      else if constexpr (has_basic_mouse_button_up<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            mouseButtonUpHandler(x, y, states...);
          }, states
        );
        this->requestRepaint();
      }
    }

    void onClick(int x, int y) override {
      if constexpr (has_extended_click<PanelT, StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            clickHandler(*this, x, y, states...);
          }, states
        );
        this->requestRepaint();
      }
      else if constexpr (has_basic_click<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            clickHandler(x, y, states...);
          }, states
        );
        this->requestRepaint();
      }
    }

    void onDoubleClick(int x, int y) override {
      if constexpr (has_extended_double_click<PanelT, StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            doubleClickHandler(*this, x, y, states...);
          }, states
        );
        this->requestRepaint();
      }
      else if constexpr (has_basic_double_click<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            doubleClickHandler(x, y, states...);
          }, states
        );
        this->requestRepaint();
      }
    }

    void onTimer(long elapsedMs) override {
      if constexpr (has_extended_timer<PanelT, StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            timerHandler(*this, elapsedMs, states...);
          }, states
        );
        this->requestRepaint();
      }
      else if constexpr (has_basic_timer<StateTs...>) {
        std::apply(
          [&](StateTs&... states) {
            timerHandler(elapsedMs, states...);
          }, states
        );
        this->requestRepaint();
      }
    }
  };

  template <typename... StateTs>
  using StatePanel = StateComponent<Panel, StateTs...>;

  template <typename... StateTs>
  using StateWindow = StateComponent<MainWindow, StateTs...>;

  extern MainWindow* mainWindow = nullptr;

  template <typename... StateTs>
  void runWindow(const char* title, StateTs&&... states) {
    if (mainWindow == nullptr) {
      mainWindow = new StateComponent<MainWindow, StateTs...>(std::forward<StateTs>(states)...);
      mainWindow->create(title);
      mainWindow->run();
      delete mainWindow;
      mainWindow = nullptr;
    }
  }

  template <typename... StateTs>
  void runWindow(const std::string& title, StateTs&&... states) {
    runWindow(title.data(), std::forward<StateTs>(states)...);
  }

  inline
  void startWindowTimer(long elapseMs) {
    mainWindow->startTimer(elapseMs);
  }
}