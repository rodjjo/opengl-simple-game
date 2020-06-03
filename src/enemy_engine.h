#ifndef ENEMY_ENGINE_H_INCLUDED
#define ENEMY_ENGINE_H_INCLUDED

#include "game_object.h"
#include "event_linstener.h"
#include "enemy.h"
#include <vector>

namespace sopa {

class ParticleEngine;
class FireEngine;

class EnemyEngine : public EventListener {
 public:
  EnemyEngine(ParticleEngine *part_eng, FireEngine *fire_eng);
  virtual void firePaint();
  virtual void fireAction();
  virtual GameObject* getEnemy(unsigned int index);
  virtual unsigned int getEnemyCount();
 private:
  unsigned int next_spawn_;
  ParticleEngine *part_eng_;
  std::vector<Enemy> enemies_;
};

}

#endif // ENEMY_ENGINE_H_INCLUDED
