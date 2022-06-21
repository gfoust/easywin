#include "easywin.hpp"
using easywin::ButtonRef;
using easywin::Canvas;
using easywin::Colors;
using easywin::MainWindow;
using easywin::Window;
using easywin::Panel;
using easywin::Size;
using easywin::Point;

#include "follow.hpp"
#include "click.hpp"
#include "statewin.hpp"
static_assert(easywin::has_paint<int, int>);
static_assert(easywin::has_mouse_move<int, int>);

#include "bounce.hpp"
static_assert(easywin::has_create<Window, State>);
static_assert(easywin::has_resize<State>);
static_assert(easywin::has_paint<State>);
static_assert(easywin::has_timer<State>);


int main() {
  //MyWin window("Hello, world");
  //window.addChild<Button>("Click Me", easywin::Point{ 100, 100 }, easywin::Size{ 75, 25 });
  //window.addChild<ManPanel>(Point{ 100, 100 }, Size{ 250, 500 });
  //window.addChild<ManPanel>(Point{ 400, 200 }, Size{ 250, 500 });
  //easywin::StateWindow<int, int> window{ 200, 200 };

  //easywin::StateWindow<State> window{ state };
  //window.create("Something Cool");
  //window.run();
  //easywin::runWindow("Something Cool", State{});
  Points points;
  easywin::runWindow("Clicky clicky", PointsState{ points });
}
