#include "player.h"
#include <windows.h>
#include "accurate_timer.h"
#include "fire_engine.h"
#include "particle_engine.h"

#define PLAYER_DESTROYED_TIME 1500
#define IMORTALITY_DURATION 4000

namespace sopa {

Player::Player(ParticleEngine* particle_engine, FireEngine* fire_engine) : GameObject(particle_engine) {
  fire_engine_ = fire_engine;
  particle_engine_ = particle_engine;
  left_ = false;
  right_ = false;
  up_ = false;
  down_ = false;
  fire_ = false;
  move_time_ = 0;
  die_time_ = 0;
  colorVertex(0.0f, 1.0f, 0.0f);
  nextVertex(0.0f, 0.1f);
  colorVertex(0.0f, 0.1f, 0.0f);
  nextVertex(0.1f, -0.1f);
  nextVertex(-0.1f, -0.1f);
  addEmissionPoint(-0.001f, -0.1f);
  fire_engine->setPlayer(this);
}

Player::~Player() {
}

void Player::fireKeyDown(unsigned char key) {
  switch(key) {
    case VK_LEFT:
      left_ = true;
    break;
    case VK_RIGHT:
      right_ = true;
    break;
    case VK_UP:
      up_ = true;
    break;
    case VK_DOWN:
      down_ = true;
    break;
    case VK_CONTROL:
      fire_ = true;
    break;
  }
}

void Player::fireKeyUp(unsigned char key) {
   switch(key) {
    case VK_LEFT:
      left_ = false;
    break;
    case VK_RIGHT:
      right_ = false;
    break;
    case VK_UP:
      up_ = false;
    break;
    case VK_DOWN:
      down_ = false;
    break;
    case VK_CONTROL: {
      fire_ = false;
      fire_time_ = 0;
    }
    break;
  }
}

void Player::firePaint() {
  if (getDestroyed()) {
    if (getCurrentTime() - getDestroyedTime() > PLAYER_DESTROYED_TIME) {
      activate(0.0f, 0.0f);
      fire_engine_->newPlayerLive();
      die_time_ = getCurrentTime();
    }
  }
  paint();
}

int Player::checkColision(GameObject* object, bool should_kill) {
  if (getDestroyed())
    return 0;
  if (getCurrentTime() - die_time_ > IMORTALITY_DURATION) {
    return GameObject::checkColision(object, true);
  }
  return GameObject::checkColision(object, false);
}

int Player::checkColision(float px, float py, bool should_kill) {
  if (getDestroyed())
    return 0;
  if (getCurrentTime() - die_time_ > IMORTALITY_DURATION) {
    return GameObject::checkColision(px, py, true);
  }
  return GameObject::checkColision(px, py, false);
}

void Player::fireAction() {
  if (!getActive() || getDestroyed())
    return;
  float px, py;
  getCoordinates(&px, &py);
  if (getCurrentTime() - move_time_ > 15) {
    if (left_)
      px -= 0.02;
    else if (right_)
      px += 0.02;
    if (up_)
      py += 0.02;
    else if (down_)
      py -= 0.02;
    move(px, py);
    if (left_ || right_ || up_ || down_)
      move_time_ = getCurrentTime();
  }
  if (fire_ && getCurrentTime() - fire_time_ > 85) {
    fire_engine_->playerFires(px, py + 0.1f);
    fire_time_ = getCurrentTime();
  }
  emit(false);
}


}
