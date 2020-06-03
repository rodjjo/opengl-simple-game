#ifndef GAME_WINDOW_H_INCLUDED
#define GAME_WINDOW_H_INCLUDED

#include <Windows.h>
#include <gl/gl.h>
#include <list>
#include "event_linstener.h"

namespace sopa {

class GameWindow {
 public:
  GameWindow(unsigned short width, unsigned short height);
  ~GameWindow();
  void run();
  void addEventListener(EventListener* listener);
 private:
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  LRESULT ProcessMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  void EnableOpenGL();
  void DisableOpenGL();
  static bool registered_;
  HWND hwnd_;
  HDC hdc_;
  HGLRC hrc_;
  bool should_quit_;
  std::list<EventListener *> listeners_;
};

}

#endif // GAME_WINDOW_H_INCLUDED
