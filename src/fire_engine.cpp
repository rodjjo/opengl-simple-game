#include "fire_engine.h"
#include <cmath>
#include <gl/gl.h>
#include <cstdlib>
#include "accurate_timer.h"
#include "particle_engine.h"
#define MAX_ENEMY_SHOTS 15
#define MAX_PLAYER_SHOTS 15
#define SHOT_MOVE_INTERVAL 15


namespace sopa {

static const char number_font[200] =
  {
    '#', '#', '#', '#','#', ' ', ' ', '#','#', ' ', ' ', '#','#', ' ', ' ', '#','#', '#', '#', '#',' ', ' ', ' ', '#',
    ' ', ' ', ' ', '#',' ', ' ', ' ', '#',' ', ' ', ' ', '#',' ', ' ', ' ', '#','#', '#', '#', '#',' ', ' ', ' ', '#',
    '#', '#', '#', '#','#', ' ', ' ', ' ','#', '#', '#', '#','#', '#', '#', '#',' ', ' ', ' ', '#','#', '#', '#', '#',
    ' ', ' ', ' ', '#','#', '#', '#', '#','#', ' ', ' ', '#','#', ' ', ' ', '#','#', '#', '#', '#',' ', ' ', ' ', '#',
    ' ', ' ', ' ', '#','#', '#', '#', '#','#', ' ', ' ', ' ','#', '#', '#', '#',' ', ' ', ' ', '#','#', '#', '#', '#',
    '#', '#', '#', '#','#', ' ', ' ', ' ','#', '#', '#', '#','#', ' ', ' ', '#','#', '#', '#', '#','#', '#', '#', '#',
    ' ', ' ', ' ', '#',' ', ' ', ' ', '#',' ', ' ', ' ', '#',' ', ' ', ' ', '#','#', '#', '#', '#','#', ' ', ' ', '#',
    '#', '#', '#', '#','#', ' ', ' ', '#','#', '#', '#', '#','#', '#', '#', '#','#', ' ', ' ', '#','#', '#', '#', '#',
    ' ', ' ', ' ', '#','#', '#', '#', '#'};


FireEngine::FireEngine(ParticleEngine *part_eng) {
  part_eng_ = part_eng;
  player_ = 0;
  enemy_shots_.resize(MAX_ENEMY_SHOTS);
  player_shots_.resize(MAX_PLAYER_SHOTS);
  last_mov_ = 0;
  score_ = 0;
}

FireEngine::~FireEngine() {
}

void FireEngine::setPlayer(GameObject* player) {
  player_ = player;
}

void FireEngine::playerFires(float px, float py) {
  static const float mi = 1.4142f / (500.0f/SHOT_MOVE_INTERVAL); // o tiro leva 1/2 segundo para cruzar a diagonal da tela
  for (unsigned int i = 0; i < player_shots_.size(); ++i) {
      fire_t & shot = player_shots_[i];
    if (!shot.active) {
      shot.active = true;
      shot.px = px;
      shot.py = py;
      shot.iy = mi;
      shot.ix = 0;
      if (rand() % 4 == 0) {
        shot.ix = (mi / (10.f + 0.01f * (rand() % 100)));
        if (rand() % 2)
          shot.ix = -shot.ix;
      }
      break;
    }
  }
}

void FireEngine::enemyFires(GameObject* enemy) {
  if (!player_) return;
  float px, py, ex, ey;
  player_->getCoordinates(&px, &py);
  enemy->getCoordinates(&ex, &ey);
  fire_t *f = 0;
  for (unsigned int i = 0; i < enemy_shots_.size(); ++i) {
    if (!enemy_shots_[i].active) {
      f = &enemy_shots_[i];
      break;
    }
  }
  if (!f)
    return;
  // inimigo atira na direçao do player
  float ix = px - ex;
  float iy = py - ey;
  static const float mi = 2.0f / (2000.0f/SHOT_MOVE_INTERVAL); // o tiro cruza a diagonal da tela em 2 segundos
  float factor = 1.0f;
  if (fabs(ix) > fabs(iy)) {
    if (fabs(ix) > mi)
      factor = mi / fabs(ix);
  } else {
    if (fabs(iy) > mi)
      factor = mi / fabs(iy);
  }
  ix = ix * factor;
  iy = iy * factor;
  f->active = true;
  f->px = ex;
  f->py = ey;
  f->ix = ix;
  f->iy = iy;

}

void FireEngine::firePaint() {
  float prev_size = 0.1f;
  glGetFloatv(GL_POINT_SIZE, &prev_size);
  glPointSize(7.0f);
  glColor3f(1.0f, 0.7f, 0.2f);
  glBegin(GL_POINTS);
  for (unsigned int i = 0; i < enemy_shots_.size(); ++i) {
    if (enemy_shots_[i].active) {
      glVertex2f(enemy_shots_[i].px, enemy_shots_[i].py);
    }
  }
  glEnd();
  glColor3f(0.4f, 1.0f, 0.4f);
  glPointSize(6.0f);
  glBegin(GL_POINTS);
  for (unsigned int i = 0; i < player_shots_.size(); ++i) {
    if (player_shots_[i].active) {
      glVertex2f(player_shots_[i].px, player_shots_[i].py);
    }
  }
  glEnd();
  glPointSize(prev_size);
  num_right_ = 0.975f;
  unsigned int score = score_;
  if (!score)
    paintNumber(0);
  else {
    int remainder = 0;
    while (score > 0) {
      remainder = score % 10;
      paintNumber(remainder);
      score /= 10;
    }
  }
}

void FireEngine::paintNumber(unsigned char number) {
  num_right_ -= 0.05f; // 0.0125;
  int index = number * 20;
  float px = num_right_;
  float py = 1.0f - 0.0125f;
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
  for (int y = 0; y < 5; ++y) {
    px = num_right_;
    py -= 0.0125;
    for (int x = 0; x < 4; ++x) {
      if (number_font[index++] == '#') {
        glVertex2f(px, py);
        glVertex2f(px+0.0125f, py);
        glVertex2f(px+0.0125f, py+0.0125f);
        glVertex2f(px, py+0.0125f);
      }
      px += 0.0125;
    }
  }
  num_right_ -= 0.0125f; // 0.0125;
  glEnd();
}


void FireEngine::fireAction() {
  int result = 0;
  if (getCurrentTime() - last_mov_ > SHOT_MOVE_INTERVAL)  {
    for (unsigned int i = 0; i < enemy_shots_.size(); ++i) {
      fire_t & shot = enemy_shots_[i];
      if (shot.active) {
        shot.px += shot.ix;
        shot.py += shot.iy;
        if (shot.px > 1.0 || shot.px < -1.0 || shot.py > 1.0 || shot.py < -1.0)
          shot.active = false;
      }
    }
    for (unsigned int i = 0; i < player_shots_.size(); ++i) {
      fire_t & shot = player_shots_[i];
      if (shot.active) {
        shot.px += shot.ix;
        shot.py += shot.iy;
        if (shot.px > 1.0 || shot.px < -1.0 || shot.py > 1.0 || shot.py < -1.0)
          shot.active = false;
      }
    }
    last_mov_ = getCurrentTime();
  }
  if (player_)
    for (unsigned int i = 0; i < enemy_shots_.size(); ++i) {
      fire_t & shot = enemy_shots_[i];
      if (!shot.active)
        continue;
      result = player_->checkColision(shot.px, shot.py, true);
      if (result) {
        shot.active = false;
        if (result == 1)
          score_ = 0;
        part_eng_->shotHits(shot.px, shot.py);
      }
    }
}

void FireEngine::checkShotColision(GameObject* enemy) {
  if (!enemy)
    return;
  for (unsigned int i = 0; i < player_shots_.size(); ++i) {
    fire_t & shot = player_shots_[i];
    if (shot.active && enemy->checkColision(shot.px, shot.py, true)) {
      shot.active = false;
      part_eng_->shotHits(shot.px, shot.py);
      score_ += 10;
    }
  }
}

void FireEngine::resetScore() {
  score_ = 0;
}

void FireEngine::newPlayerLive() {
  for (unsigned int i = 0; i < enemy_shots_.size(); ++i) {
    fire_t & shot = enemy_shots_[i];
    if (shot.active) {
      shot.active =false;
      part_eng_->shotHits(shot.px, shot.py);
    }
  }
}
}
