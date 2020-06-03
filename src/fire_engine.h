#ifndef FIRE_ENGINE_H_INCLUDED
#define FIRE_ENGINE_H_INCLUDED

#include <vector>
#include "game_object.h"
#include "event_linstener.h"

namespace sopa {

struct fire_t {
  fire_t() {active = false;};
  bool  active;
  float px;
  float py;
  float ix;
  float iy;
};

class ParticleEngine;

class FireEngine : public EventListener {
 public:
  FireEngine(ParticleEngine *part_eng);
  virtual ~FireEngine();
  virtual void setPlayer(GameObject* player);
  virtual void playerFires(float px, float py);
  virtual void enemyFires(GameObject* enemy);
  virtual void newPlayerLive();
  virtual void firePaint();
  virtual void fireAction();
  virtual void checkShotColision(GameObject* enemy);
  virtual void resetScore();
 private:
  void paintNumber(unsigned char number);
  std::vector<fire_t> player_shots_;
  std::vector<fire_t> enemy_shots_;
  unsigned int last_mov_;
  GameObject* player_;
  ParticleEngine *part_eng_;
  unsigned int score_;
  float num_right_;
};

}

#endif // FIRE_ENGINE_H_INCLUDED
