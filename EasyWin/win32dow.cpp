#include "win32dow.hpp"
#include "panel.hpp"
#include "gdi_canvas.hpp"
#include <windowsx.h>
#include <stdexcept>
#include <vector>
#include <chrono>

namespace easywin::impl {

 /*=========================================================
  * Global variables
  */
  HINSTANCE hInstance = GetModuleHandle(NULL);

  static const struct Extents {
    int xChar;
    int yChar;
    int scrollBarWidth;
    int scrollBarHeight;

    Extents() {
      HDC hdc = GetDC(NULL);

      // Extract font dimensions from the text metrics. 
      TEXTMETRIC tm;
      GetTextMetrics(hdc, &tm);
      xChar = tm.tmAveCharWidth;
      // int xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar / 2;
      yChar = tm.tmHeight + tm.tmExternalLeading;

      // Free the device context. 
      ReleaseDC(NULL, hdc);

      scrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
      scrollBarHeight = GetSystemMetrics(SM_CXHSCROLL);
    }

  } extents;

  /*=========================================================
   * WndProc
   */

  BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) {
    Panel* panel = (Panel*)lpCreateStruct->lpCreateParams;
    SetWindowLongPtr(hwnd, 0, (LONG_PTR)panel);
    panel->setHwnd(hwnd);
    panel->onCreate();
    return true;
  }

  void onDestroy(HWND hwnd) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);
    panel->onDestroy();
  }

  void onPaint(HWND hwnd) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);

    RECT client;
    GetClientRect(hwnd, &client);

    PAINTSTRUCT ps;
    HDC screenHdc = BeginPaint(hwnd, &ps);

    HDC hdc = CreateCompatibleDC(screenHdc);
    HBITMAP bitmap =
      CreateCompatibleBitmap(screenHdc, client.right + 1, client.bottom + 1);
    HGDIOBJ oldBitmap = SelectObject(hdc, bitmap);

    //BitBlt(hdc, 0, 0, client.right + 1, client.bottom + 1, screenHdc, 0, 0, SRCCOPY);

    SelectObject(hdc, GetStockObject(DC_PEN));
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    //SetDCPenColor(hdc, 0xaaaaaa);
    //SetDCBrushColor(hdc, 0xaaaaaa);
    //Rectangle(hdc, 0, 0, client.right + 1, client.bottom + 1);

    XFORM xform;
    xform.eM11 = 1.0;
    xform.eM12 = 0.0;
    xform.eM21 = 0.0;
    xform.eM22 = 1.0;
    xform.eDx = 0.0;
    xform.eDy = 0.0;

    if (GetWindowLong(hwnd, GWL_STYLE) & WS_HSCROLL) {
      SCROLLINFO si;
      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_POS;
      GetScrollInfo(hwnd, SB_HORZ, &si);
      xform.eDx = (float)-si.nPos;
    }
    if (GetWindowLong(hwnd, GWL_STYLE) & WS_VSCROLL) {
      SCROLLINFO si;
      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_POS;
      GetScrollInfo(hwnd, SB_VERT, &si);
      xform.eDy = (float)-si.nPos;
    }
    if (xform.eDx != 0 || xform.eDy != 0) {
      SetGraphicsMode(hdc, GM_ADVANCED);
      SetWorldTransform(hdc, &xform);
    }

    GdiCanvas canvas(hdc);

    panel->onPaint(canvas);

    if (xform.eDx != 0 || xform.eDy != 0) {
      xform.eDx = 0;
      xform.eDy = 0;
      SetWorldTransform(hdc, &xform);
    }
    BitBlt(screenHdc, 0, 0, client.right, client.bottom, hdc, 0, 0, SRCCOPY);

    EndPaint(hwnd, &ps);

    SelectObject(hdc, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(hdc);
  }

  static bool isDoubleClick;
  void onLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);

    isDoubleClick = fDoubleClick;
    panel->onMouseButtonDown(x, y);

    SetCapture(hwnd);
  }

  void onMouseMove(HWND hwnd, int x, int y, UINT keyFlags) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);

    panel->onMouseMove(x, y);
  }

  void onLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);

    panel->onMouseButtonUp(x, y);

    RECT client;
    GetClientRect(hwnd, &client);
    if (x >= client.left && x < client.right && y >= client.top && y < client.bottom) {
      if (isDoubleClick) {
        panel->onDoubleClick(x, y);
      }
      else {
        panel->onClick(x, y);
      }
    }

    ReleaseCapture();
  }

  void onMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);

    POINT p = { xPos, yPos };

    ScreenToClient(hwnd, &p);

    panel->onMouseWheel(p.x, p.y, zDelta > 0);
  }

  void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);

    //if (hwndCtl == 0) {
    //  if (codeNotify == 0) {
    //    //window->menuSelect(id);
    //  }
    //}
    //else {
    //  Container* container;
    //  if (codeNotify == BN_CLICKED && (container = dynamic_cast<Container*>(panel))) {
    //    container->child<Button>(id - 1).clickHandler();
    //  }
    //}
  }

  void onSize(HWND hwnd, UINT state, int cx, int cy) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);
    panel->onResize(cx, cy);
  }

  void onHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos) {
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_POS | SIF_PAGE;
    GetScrollInfo(hwnd, SB_HORZ, &si);
    int oldPos = si.nPos;

    switch (code)
    {
    case SB_LINEUP:
      si.nPos -= extents.xChar;
      break;
    case SB_LINEDOWN:
      si.nPos += extents.xChar;
      break;
    case SB_PAGEUP:
      si.nPos -= si.nPage;
      break;
    case SB_PAGEDOWN:
      si.nPos += si.nPage;
      break;
    case SB_THUMBTRACK:
    case SB_THUMBPOSITION:
      si.nPos = pos;
      break;
    }

    si.fMask = SIF_POS;
    SetScrollInfo(hwnd, SB_HORZ, &si, true);
    GetScrollInfo(hwnd, SB_HORZ, &si);
    if (oldPos != si.nPos) {
      ScrollWindow(hwnd, (oldPos - si.nPos), 0, NULL, NULL);
    }
  }

  void onVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos) {
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_POS | SIF_PAGE;
    GetScrollInfo(hwnd, SB_VERT, &si);
    int oldPos = si.nPos;

    switch (code)
    {
    case SB_LINEUP:
      si.nPos -= extents.yChar;
      break;
    case SB_LINEDOWN:
      si.nPos += extents.yChar;
      break;
    case SB_PAGEUP:
      si.nPos -= si.nPage;
      break;
    case SB_PAGEDOWN:
      si.nPos += si.nPage;
      break;
    case SB_THUMBTRACK:
    case SB_THUMBPOSITION:
      si.nPos = pos;
      break;
    }

    si.fMask = SIF_POS;
    SetScrollInfo(hwnd, SB_VERT, &si, true);
    GetScrollInfo(hwnd, SB_VERT, &si);
    if (oldPos != si.nPos) {
      ScrollWindow(hwnd, 0, (oldPos - si.nPos), NULL, NULL);
    }
  }

  struct Timer { 
    HWND hwnd;
    std::chrono::high_resolution_clock::time_point timestamp; 
  };
  static std::vector<Timer> timers;

  void onTimer(HWND hwnd, UINT_PTR id) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);
    auto now = std::chrono::high_resolution_clock::now();
    Timer& timer = timers[id - 1];
    long elapsedMs = (long)std::chrono::duration_cast<std::chrono::milliseconds>(now - timer.timestamp).count();
    timer.timestamp = now;
    panel->onTimer(elapsedMs);
  }

  LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
      HANDLE_MSG(hwnd, WM_CREATE, onCreate);
      HANDLE_MSG(hwnd, WM_PAINT, onPaint);
      HANDLE_MSG(hwnd, WM_LBUTTONDOWN, onLButtonDown);
      HANDLE_MSG(hwnd, WM_MOUSEMOVE, onMouseMove);
      HANDLE_MSG(hwnd, WM_LBUTTONUP, onLButtonUp);
      HANDLE_MSG(hwnd, WM_MOUSEWHEEL, onMouseWheel);
      HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, onLButtonDown);
      HANDLE_MSG(hwnd, WM_COMMAND, onCommand);
      HANDLE_MSG(hwnd, WM_DESTROY, onDestroy);
      HANDLE_MSG(hwnd, WM_SIZE, onSize);
      HANDLE_MSG(hwnd, WM_HSCROLL, onHScroll);
      HANDLE_MSG(hwnd, WM_VSCROLL, onVScroll);
      HANDLE_MSG(hwnd, WM_TIMER, onTimer);
    case WM_ERASEBKGND:
      return 1;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
  }

  /*=========================================================
   * Management functions
   */

  void registerClass(const char* className) {
    WNDCLASS wndclass;
    memset(&wndclass, 0, sizeof(WNDCLASS));

    if (GetClassInfo(hInstance, className, &wndclass) == 0) {
      wndclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndclass.lpfnWndProc = wndProc;
      wndclass.cbClsExtra = 0;
      wndclass.cbWndExtra = sizeof(Panel*);
      wndclass.hInstance = hInstance;
      wndclass.hIcon = NULL;
      wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
      wndclass.hbrBackground = NULL;
      wndclass.lpszMenuName = NULL;
      wndclass.lpszClassName = className;

      if (RegisterClass(&wndclass) == 0)
        throw std::runtime_error(std::string("Unable to register class ") + className);
    }
  }

  HWND createWindow(
    const char* className,
    const char* text,
    unsigned long style,
    int x, int y,
    int width, int height,
    HWND parent,
    long long id,
    Component* component
  ) {
    HWND hwnd = CreateWindow(
      className,            // window class name
      text,                 // window caption
      style,                // window style
      x,                    // initial x position
      y,                    // initial y position
      width,                // initial x size
      height,               // initial y size
      parent,               // parent window handle
      (HMENU)(long long)id, // window menu handle
      hInstance,            // program instance handle
      component             // param to WM_CREATE
    );

    if (!hwnd) {
      throw std::runtime_error(std::string("Unable to create window ") + className);
    }

    return hwnd;
  }

  Size getClientSize(HWND hwnd) {
    RECT client;
    GetClientRect(hwnd, &client);
    return { client.right, client.bottom };
  }

  Rect getClientRect(HWND hwnd) {
    RECT client;
    GetClientRect(hwnd, &client);
    //MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&client, 2);
    return { client.left, client.top, client.right, client.bottom };
  }

  std::string getWindowText(HWND hwnd) {
    char cs[200];
    GetWindowText(hwnd, cs, 200);
    return cs;
  }

  void setScrollSize(HWND hwnd, Size size) {
    RECT client;
    GetClientRect(hwnd, &client);

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    if (GetWindowLong(hwnd, GWL_STYLE) & WS_HSCROLL) {
      si.fMask = SIF_POS;
      GetScrollInfo(hwnd, SB_HORZ, &si);
    }
    else {
      si.nPos = 0;
    }
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nMax = size.width;
    si.nPage = client.right + 1;
    int oldPos = si.nPos;
    SetScrollInfo(hwnd, SB_HORZ, &si, true);
    GetScrollInfo(hwnd, SB_HORZ, &si);
    if (oldPos != si.nPos) {
      ScrollWindow(hwnd, (oldPos - si.nPos), 0, NULL, NULL);
    }

    static int i = 0;
    if (GetWindowLong(hwnd, GWL_STYLE) & WS_VSCROLL) {
      si.fMask = SIF_POS;
      GetScrollInfo(hwnd, SB_VERT, &si);
    }
    else {
      si.nPos = 0;
    }
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nMax = size.height;
    si.nPage = client.bottom + 1;
    oldPos = si.nPos;
    SetScrollInfo(hwnd, SB_VERT, &si, true);
    GetScrollInfo(hwnd, SB_VERT, &si);
    if (oldPos != si.nPos) {
      ScrollWindow(hwnd, 0, (oldPos - si.nPos), NULL, NULL);
    }

    InvalidateRect(hwnd, NULL, true);
  }

  int runWindow(HWND hwnd) {
    try {
      if (hwnd != NULL) {
        ShowWindow(hwnd, SW_SHOWNORMAL);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
        return (int)msg.wParam;
      }
      else {
        return -1;
      }
    }
    catch (std::exception& e) {
      MessageBox(NULL, e.what(), "Error", MB_ICONERROR | MB_OK);
      return -1;
    }
  }

  size_t startTimer(HWND hwnd, long elapseMs) {
    size_t id = 0;
    for (size_t i = 0; i < timers.size(); ++i) {
      if (timers[i].hwnd == hwnd) {
        timers[i].timestamp = std::chrono::high_resolution_clock::now();
        id = i + 1;
        break;
      }
    }
    if (id == 0) {
      timers.push_back({ hwnd, std::chrono::high_resolution_clock::now() });
      id = timers.size();
    }

    SetTimer(hwnd, id, elapseMs, NULL);
    return timers.size();
  }

  void stopTimer(HWND hwnd) {
    size_t id = 0;
    for (size_t i = 0; i < timers.size(); ++i) {
      if (timers[i].hwnd == hwnd) {
        id = i + 1;
      }
    }
    if (id > 0) {
      KillTimer(hwnd, id);
    }
  }

}

int main();

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow) {
  easywin::impl::hInstance = hInstance;
  return main();
}
