#include "enemy.h"
#include <cstdlib>
#include "accurate_timer.h"
#include "particle_engine.h"
#include "fire_engine.h"

// menor e maior tempo que um inimigo fica na tela
#define MIN_SCREEN_TIME  4000
#define MAX_SCREEN_TIME 10000
#define MIN_SCREEN_ACTIONS 4
#define MAX_SCREEN_ACTIONS 10

namespace sopa {

Enemy::Enemy(ParticleEngine *part_eng, FireEngine *fire_eng) : GameObject(part_eng) {
  action_count_ = 0;
  last_movment_ = 0;
  currrent_action_ = 0;
  part_eng_ = part_eng;
  fire_eng_ = fire_eng;
  actions_.resize(MAX_SCREEN_ACTIONS);
  colorVertex(1.0f, 0.0f, 0.0f);
  nextVertex(0.0f, -0.1f);
  colorVertex(0.1f, 0.0f, 0.0f);
  nextVertex(0.1f, 0.1f);
  nextVertex(-0.1f, 0.1f);
  addEmissionPoint(-0.001,0.1f);
  lives_ = 0;
}

Enemy::~Enemy() {
}

void Enemy::firePaint() {
  paint();
}

void Enemy::fireAction() {
  if (!getActive() || getDestroyed())
    return;
  float px, py;
  getCoordinates(&px, &py);
  if (getCurrentTime() - last_movment_ > 15) {
    py -= iy_;
    px += actions_[currrent_action_].ix;
    if (px < -0.9)
      px  = -0.9;
    if (px > 0.9)
      px  = 0.9;
    move(px, py);
    if (py < -1.0)
      deactivate();
    last_movment_ = getCurrentTime();
    if (should_fire_ && py < 0.9 && py > -0.9) {
      fire_eng_->enemyFires(this);
      should_fire_ = false;
    }
  }

  if (getCurrentTime() - actions_[currrent_action_].duration > action_time_) {
    if (currrent_action_ < action_count_) {
      should_fire_ |=  actions_[currrent_action_].fire;
      ++currrent_action_;
      ix_ =  actions_[currrent_action_].ix;
    }
    action_time_ = getCurrentTime();
  }

  emit(true);
}

void Enemy::destroy() {
  if (lives_ < 1) {
    GameObject::destroy();
    deactivate();
  } else {
    --lives_;
  }
}

void Enemy::activate(float wx, float wy) {
  lives_ = 2;
  GameObject::activate(wx, 1.2f); // ignoramos o parametro wy pq inimigo sempre nasce no topo da tela
  buildActions();
}

void Enemy::buildActions() {
  // cria movimentação aleatória para o inimigo
  // menor quantidade de açoes 3 e maxima = 8, 2 o inimigo solta de 1 a 2 tiros dentro da tela a cada final de ação
  unsigned int time_on_screen = rand() % (MAX_SCREEN_TIME - MIN_SCREEN_TIME) + MIN_SCREEN_TIME;
  action_count_ = rand() % (MAX_SCREEN_ACTIONS - MIN_SCREEN_ACTIONS) + MIN_SCREEN_ACTIONS;
  if (actions_.size() != MAX_SCREEN_ACTIONS)
    actions_.resize(MAX_SCREEN_ACTIONS);
  unsigned time_fraction = time_on_screen / (action_count_ * 3);
  float px, py;
  getCoordinates(&px, &py);
  // quanto falta
  py = py + 1.2;
  iy_ = py / (time_on_screen / 15.0f); // passos de 15 milisegundos
  int remain_time = time_on_screen;
  for (size_t i = 0; i < action_count_; ++i) {
    actions_[i].duration = time_fraction;
    actions_[i].ix = (rand() % 2 == 0)  ? iy_ : - iy_;
    actions_[i].fire = true;
    remain_time -= time_fraction;
  }
  while (remain_time>0) { // distribui aleatoriamente o tempo de uma ação
    actions_[rand() % (action_count_ + 1)].duration += time_fraction;
    remain_time -= time_fraction;
  }
  for (size_t i = 0; i < action_count_; ++i) {
    if ((rand() % 8 == 0))
      actions_[rand() % (action_count_ + 1)].fire = false;
  }
  should_fire_ = false;
  ix_ = actions_[0].ix;
  currrent_action_ = 0;
  action_time_ = getCurrentTime();
}

}
