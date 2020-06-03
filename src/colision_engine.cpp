#include "colision_engine.h"
#include "enemy_engine.h"
#include "fire_engine.h"

namespace sopa {

ColisionEngine::ColisionEngine(GameObject* player, EnemyEngine *enemy_eng, FireEngine *fire_eng) {
  player_ = player;
  enemy_eng_ = enemy_eng;
  fire_eng_ = fire_eng;
}

void ColisionEngine::fireAction() {
  unsigned int enemy_count = enemy_eng_->getEnemyCount();
  for (unsigned int i = 0; i < enemy_count; ++i)
    if (player_->checkColision(enemy_eng_->getEnemy(i), true)) {
      fire_eng_->resetScore();
      break;
    }

  for (unsigned int i = 0; i < enemy_count; ++i)
    fire_eng_->checkShotColision(enemy_eng_->getEnemy(i));
}

}
