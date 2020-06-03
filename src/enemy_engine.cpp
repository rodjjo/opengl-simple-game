#include "enemy_engine.h"
#include <cstddef>
#include <cstdlib>
#include "particle_engine.h"
#include "fire_engine.h"
#include "accurate_timer.h"

#define MIN_SPAWN_TIME  200
#define MAX_SPAWN_TIME  2000
#define MAX_ENEMIES 20

namespace sopa {

EnemyEngine::EnemyEngine(ParticleEngine *part_eng, FireEngine *fire_eng) {
  part_eng_ = part_eng;
  next_spawn_ = 0;
  for (size_t i = 0; i < MAX_ENEMIES; ++i) {
    enemies_.push_back(Enemy(part_eng, fire_eng));
  }
}

void EnemyEngine::firePaint() {
  for (size_t i = 0; i < enemies_.size(); ++i) {
    enemies_[i].firePaint();
  }
};

void EnemyEngine::fireAction() {
  if (getCurrentTime() > next_spawn_) {
    for (size_t i = 0; i < enemies_.size(); ++i) {
      if (!enemies_[i].getActive()) {
        enemies_[i].activate(-1.0f + (rand() % 100) * 0.02,1.2);
        next_spawn_ = getCurrentTime() + MIN_SPAWN_TIME + (rand() % (MAX_SPAWN_TIME - MIN_SPAWN_TIME));
        break;
      }
    }
  }
  for (size_t i = 0; i < enemies_.size(); ++i) {
    enemies_[i].fireAction();
  }

};

GameObject* EnemyEngine::getEnemy(unsigned int index) {
  if (index < getEnemyCount())
    if (enemies_[index].getActive())
      return &enemies_[index];
  return 0;
}

unsigned int EnemyEngine::getEnemyCount() {
  return enemies_.size();
}

}

