#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "event_linstener.h"
#include "game_object.h"

#include <vector>

//

namespace sopa {

struct action_t {
  action_t() {
    ix = 0;
    fire = false;
    duration = 0;
  }
  action_t(float pix, float piy, bool pfire, unsigned int pduration)
    : ix(pix), fire(pfire), duration(pduration) {
  }
  float ix;
  bool fire;
  unsigned int duration;
};

class ParticleEngine;
class FireEngine;

class Enemy : public GameObject, public EventListener {
 public:
  Enemy(ParticleEngine *part_eng, FireEngine *fire_eng);
  virtual ~Enemy();
  void firePaint();
  void fireAction();
  virtual void activate(float wx, float wy);
  virtual void destroy();
 private:
  void buildActions();
  std::vector<action_t> actions_;
  float iy_;
  float ix_;
  unsigned int last_movment_;
  unsigned int action_time_;
  unsigned int currrent_action_;
  unsigned int action_count_;
  unsigned int lives_;
  bool should_fire_;
  ParticleEngine *part_eng_;
  FireEngine *fire_eng_;
};

}

#endif // ENEMY_H_INCLUDED
