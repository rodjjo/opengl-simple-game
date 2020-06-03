#include "accurate_timer.h"
#include <windows.h>
#include <Mmsystem.h>


namespace sopa {

static LONG volatile gTmr;
static MMRESULT gCurrentTimer;


class TimeCounter {
 public:
  TimeCounter() {
    if (gCurrentTimer)
      return;

    TIMECAPS tc = {0};

    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
      return;
    gCurrentTimer = timeSetEvent(1,
                 min(max(tc.wPeriodMin, 1), tc.wPeriodMax),
                 (LPTIMECALLBACK)(&timeProc),
                 0,
                 TIME_PERIODIC);
  }

  static void CALLBACK timeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) {
    InterlockedIncrement(&gTmr);
  };

  ~TimeCounter() {
    if (gCurrentTimer == 0)
      return;
    timeKillEvent(gCurrentTimer);
    gCurrentTimer = 0;
  }
};

static TimeCounter tmrControll;

unsigned int getCurrentTime() {
  return InterlockedCompareExchange(&gTmr, 0, 0);
}

bool timerInstalled() {
  return gCurrentTimer != 0;
}


}  // namespace z3d
