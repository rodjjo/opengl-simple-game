#ifndef COLISION_ENGINE_H_INCLUDED
#define COLISION_ENGINE_H_INCLUDED

#include "game_object.h"
#include "event_linstener.h"

namespace sopa {

class EnemyEngine;
class FireEngine;

class ColisionEngine : public EventListener {
 public:
  ColisionEngine(GameObject* player, EnemyEngine *enemy_eng, FireEngine *fire_eng);
  virtual void fireAction();
 private:
  EnemyEngine *enemy_eng_;
  FireEngine *fire_eng_;
  GameObject* player_;
};

}

#endif // COLISION_ENGINE_H_INCLUDED
