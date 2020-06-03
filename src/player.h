#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "game_object.h"
#include "event_linstener.h"

namespace sopa {

class FireEngine;
class ParticleEngine;

class Player : public GameObject, public EventListener {
 public:
  Player(ParticleEngine* particle_engine, FireEngine* fire_engine);
  virtual ~Player();
  void fireKeyDown(unsigned char key);
  void fireKeyUp(unsigned char key);
  void firePaint();
  void fireAction();
  virtual int checkColision(GameObject* object, bool should_kill);
  virtual int checkColision(float px, float py, bool should_kill);
 private:
  bool left_;
  bool right_;
  bool up_;
  bool down_;
  bool fire_;
  unsigned int move_time_;
  unsigned int fire_time_;
  unsigned int die_time_;
  FireEngine* fire_engine_;
  ParticleEngine* particle_engine_;
};

}

#endif // PLAYER_H_INCLUDED
