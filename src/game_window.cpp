#include "game_window.h"
#include <exception>

#define SOPA_WINDOW "GL_SopaClass"

namespace sopa {

bool GameWindow::registered_ = false;

GameWindow::GameWindow(unsigned short width, unsigned short height) {
  if (!registered_) {
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = GameWindow::WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = (HINSTANCE)GetModuleHandle(NULL);
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = SOPA_WINDOW;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    if (!RegisterClassEx(&wcex)) {
      throw std::exception();
    }
    registered_ = true;
  }
  hdc_ = NULL;
  hrc_ = NULL;
  should_quit_ = false;
  hwnd_ = CreateWindowEx(0, SOPA_WINDOW, "Sopa Games", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                        NULL, NULL, (HINSTANCE)GetModuleHandle(NULL), this);
  EnableOpenGL();
}

void GameWindow::EnableOpenGL() {
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;
    /* get the device context (DC) */
    hdc_ = GetDC(hwnd_);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(hdc_, &pfd);

    SetPixelFormat(hdc_, iFormat, &pfd);

    /* create and enable the render context (RC) */
    hrc_ = wglCreateContext(hdc_);

    wglMakeCurrent(hdc_, hrc_);
}

void GameWindow::DisableOpenGL() {
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hrc_);
  ReleaseDC(hwnd_, hdc_);
}

GameWindow::~GameWindow() {
  DisableOpenGL();
}

void GameWindow::run() {
  should_quit_ = false;
  MSG msg;
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  while (!should_quit_) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        should_quit_ = true;
      } else {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix();
        for(std::list<EventListener *>::iterator it = listeners_.begin(); it != listeners_.end(); ++it) {
          (*it)->fireAction();
          (*it)->firePaint();
        }
        glPopMatrix();
        SwapBuffers(hdc_);
        Sleep(1);
    }
  }
}

LRESULT CALLBACK GameWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_CREATE) {
    LPCREATESTRUCT cs = (LPCREATESTRUCT)lParam;
    SetWindowLong(hwnd, GWL_USERDATA, reinterpret_cast<LONG>(cs->lpCreateParams));
    return 0L;
  }
  GameWindow *game_window = reinterpret_cast<GameWindow *>(GetWindowLong(hwnd, GWL_USERDATA));
  if (game_window)
    return game_window->ProcessMessage(hwnd, uMsg, wParam, lParam);
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT GameWindow::ProcessMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_KEYDOWN:
      if (wParam < 256)
        for(std::list<EventListener *>::iterator it = listeners_.begin(); it != listeners_.end(); ++it)
          (*it)->fireKeyDown(wParam);
    break;
    case WM_KEYUP:
      if (wParam < 256)
        for(std::list<EventListener *>::iterator it = listeners_.begin(); it != listeners_.end(); ++it)
          (*it)->fireKeyUp(wParam);
    break;
    case WM_CLOSE:
      PostQuitMessage(0);
    break;
    case WM_DESTROY:
    return 0L;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void GameWindow::addEventListener(EventListener* listener) {
  listeners_.push_back(listener);
}


}
