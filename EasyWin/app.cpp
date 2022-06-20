#include "app.hpp"
#include "win32dow.hpp"
#include <stdio.h>

namespace easywin {

  void allocateConsole() {
    AllocConsole();
    FILE* oldin, * oldout, * olderr;
    freopen_s(&oldin, "CONIN$", "r", stdin);
    freopen_s(&oldout, "CONOUT$", "w", stdout);
    freopen_s(&olderr, "CONOUT$", "w", stderr);
  }

}