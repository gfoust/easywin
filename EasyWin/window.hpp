#pragma once
#undef UNICODE
#include <windows.h>
#include <string>
#include <typeinfo>
#include <vector>
#include "container.hpp"

namespace easywin {

  class Window : public Container {

  protected:
    void addMenu(std::string name);
    void addMenuItem(int menuPos, std::string name, int id);
    unsigned long style() const override;

  public:
    void create(const char* title = "EasyWin Application");
    inline void create(const std::string& title) {
      create(title.data());
    }
    int run();
    std::string getTitle();
    void setTitle(const std::string& title);

  };

}  // namespace easywin

