#include <windows.h>
#include <gl/gl.h>
#include "accurate_timer.h"
#include "game_window.h"
#include "particle_engine.h"
#include "enemy_engine.h"
#include "fire_engine.h"
#include "player.h"
#include "colision_engine.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    if (!sopa::timerInstalled()) {
      MessageBoxA(NULL, "Falha registrando relogio", "Falha registrando relogio", MB_OK|MB_ICONEXCLAMATION);
      return 1;
    }
    sopa::ParticleEngine part_eng;
    sopa::FireEngine fire_eng(&part_eng);
    sopa::EnemyEngine enemy_eng(&part_eng, &fire_eng);
    sopa::Player player(&part_eng, &fire_eng);
    sopa::ColisionEngine col_eng(&player, &enemy_eng, &fire_eng);
    {
      player.activate(0.0f, 0.0f);
      sopa::GameWindow window(1024, 768);
      window.addEventListener(&player);
      window.addEventListener(&fire_eng);
      window.addEventListener(&enemy_eng);
      window.addEventListener(&col_eng);
      window.addEventListener(&part_eng);
      window.run();
    }
    return 0;
}


