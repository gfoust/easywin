#define _WIN32_WINNT 0x500
#include "win32dow.hpp"
#include "panel.hpp"
#include "window.hpp"
#include "button.hpp"
#include "gdi_canvas.hpp"

#include <windowsx.h>
#include <stdexcept>
#include <string>
#include <strsafe.h>

namespace easywin {

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

  BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) {
    Panel* panel = (Panel*)lpCreateStruct->lpCreateParams;
    SetWindowLongPtr(hwnd, 0, (LONG_PTR)panel);
    panel->hwnd = hwnd;
    panel->onCreate();
    return true;
  }

  void onDestroy(HWND hwnd) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);
    panel->onDestroy();
    if (dynamic_cast<Window*>(panel)) {
      PostQuitMessage(0);
    }
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
    SetDCPenColor(hdc, 0xaaaaaa);
    SetDCBrushColor(hdc, 0xaaaaaa);
    Rectangle(hdc, 0, 0, client.right + 1, client.bottom + 1);

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
      xform.eDx = (float) -si.nPos;
    }
    if (GetWindowLong(hwnd, GWL_STYLE) & WS_VSCROLL) {
      SCROLLINFO si;
      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_POS;
      GetScrollInfo(hwnd, SB_VERT, &si);
      xform.eDy = (float) -si.nPos;
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

  void onLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);

    if (fDoubleClick) {
      panel->onMouseButtonDoubleClick(x, y);
    }
    else {
      panel->onMouseButtonDown(x, y);
    }

    SetCapture(hwnd);
  }

  void onMouseMove(HWND hwnd, int x, int y, UINT keyFlags) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);

    panel->onMouseMove(x, y);
  }

  void onLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {
    Panel* panel = (Panel*)GetWindowLongPtr(hwnd, 0);

    panel->onMouseButtonUp(x, y);

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

    if (hwndCtl == 0) {
      if (codeNotify == 0) {
        //window->menuSelect(id);
      }
    }
    else {
      Container* container;
      if (codeNotify == BN_CLICKED && (container = dynamic_cast<Container*>(panel))) {
        container->child<Button>(id - 1).clickHandler();
      }
    }
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
    case WM_ERASEBKGND:
      return 1;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
  }

  void registerClass(const char* className) {
    WNDCLASS wndclass;
    memset(&wndclass, 0, sizeof(WNDCLASS));

    if (GetClassInfo(hInstance, className, &wndclass) == 0) {
      wndclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndclass.lpfnWndProc = wndProc;
      wndclass.cbClsExtra = 0;
      wndclass.cbWndExtra = sizeof(Window*);
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
    LPVOID param
  ) {
    HWND check = CreateWindow(
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
      param
    );

    if (!check) {
      LPVOID lpMsgBuf;
      DWORD dw = GetLastError();

      FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

      // Display the error message and exit the process

      LocalFree(lpMsgBuf);

    }
    return check;
  }

  void setComponent(HWND hwnd, Panel* panel) {
    if (hwnd && (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC) == wndProc) {
      SetWindowLongPtr(hwnd, 0, (LONG_PTR)panel);
    }
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

  void allocateConsole() {
    AllocConsole();
    FILE* oldin, * oldout, * olderr;
    freopen_s(&oldin, "CONIN$", "r", stdin);
    freopen_s(&oldout, "CONOUT$", "w", stdout);
    freopen_s(&olderr, "CONOUT$", "w", stderr);
  }

  std::string getText(HWND hwnd) {
    char buffer[200];
    GetWindowText(hwnd, buffer, 200);
    return buffer;
  }

}

int main();

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow) {
  easywin::hInstance = hInstance;
  return main();
}
