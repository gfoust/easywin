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
    const char* text() const override { return "Window"; }

  public:
    void create();
    int run();
    std::string getTitle();
    void setTitle(const std::string& title);

  };

}  // namespace easywin

