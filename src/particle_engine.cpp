#include "particle_engine.h"
#include "accurate_timer.h"
#include <gl/gl.h>
#include <cstdlib>
#define MAX_PARTICLES 2000
#define MIN_PART_DURATION 250
#define MAX_PART_DURATION 500
#define MAX_EXP_DURATION 1500
#define PARTICLE_ALFA_INTERVAL 33
#define PARTICLE_MOV_INTERVAL 15

namespace sopa {

struct color_t {
  float r;
  float g;
  float b;
};

static const color_t colors[] = {
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.5f, 0.5f},
    {0.5f, 1.0f, 0.5f},
    {0.5f, 0.5f, 1.0f}
  };

ParticleEngine::ParticleEngine() {
  particles_.resize(MAX_PARTICLES);
  last_part_alfa_ = 0;
  last_part_movs_ = 0;
}

ParticleEngine::~ParticleEngine() {
}

void ParticleEngine::firePaint() {
  float prev_size = 0.1f;
  glGetFloatv(GL_POINT_SIZE, &prev_size);
  glPointSize(3.0f);
  glBegin(GL_POINTS);
  for (unsigned int i = 0; i < particles_.size(); ++i) {
    particle_t & p = particles_[i];
    if (p.active) {
      const color_t & c = colors[p.color];
      glColor4f(c.r, c.g, c.b, p.a);
      glVertex2f(p.px , p.py);
    }
  }
  glEnd();
  glPointSize(prev_size);
}

void ParticleEngine::fireAction() {
  if (getCurrentTime() - last_part_alfa_ > PARTICLE_ALFA_INTERVAL) {
    for (unsigned int i = 0; i < particles_.size(); ++i) {
      particle_t & p = particles_[i];
      if (p.active) {
        p.a -= p.ia;
        if (p.a < 0.0f) {
          p.active = false;
        }
      }
    }
    last_part_alfa_ = getCurrentTime();
  }
  if (getCurrentTime() - last_part_movs_ > PARTICLE_MOV_INTERVAL) {
    for (unsigned int i = 0; i < particles_.size(); ++i) {
      particle_t & p = particles_[i];
      if (p.active) {
        p.px += p.ix;
        p.py += p.iy;
      }
    }
    last_part_movs_ = getCurrentTime();
  }
}

void ParticleEngine::explosion(float px, float py) {
  int amount = 200;
  static float velocity = 2.0f / (1500.0f/PARTICLE_MOV_INTERVAL); // particula leva 2.5 segundos para cruzar a tela
  int rand_num ;
  for (unsigned int i = 0; i < particles_.size(); ++i) {
    particle_t & p = particles_[i];
    if (!p.active) {
      p.active = true;
      p.px = px + (rand() % 100) * 0.0001;
      p.py = py + (rand() % 100) * 0.0001;
      p.iy = p.px - px;
      p.ix = p.py - py;
      p.a = 1.0f;
      p.ia = 1.0f / ((float)(MIN_PART_DURATION + rand() % (MAX_EXP_DURATION - MIN_PART_DURATION)) / (float)PARTICLE_ALFA_INTERVAL);
      if (rand() % 2)
        p.ix = -p.ix;
      if (rand() % 2)
        p.iy = -p.iy;
      if (rand() % 2)
        p.color = 0;
      else
        p.color = 3;
      if (--amount < 1)
        break;
    }
  }
}

void ParticleEngine::emission(float px, float py, bool up) {
  int amount = 15;
  static float velocity = 2.0f / (1500.0f/PARTICLE_MOV_INTERVAL); // particula leva 2.5 segundos para cruzar a tela
  float iy = up ? velocity : -velocity;
  int rand_num ;
  for (unsigned int i = 0; i < particles_.size(); ++i) {
    particle_t & p = particles_[i];
    if (!p.active) {
      p.active = true;
      p.px = px;
      p.py = py;
      p.iy = iy;
      p.ix = (rand() % 100) * 0.00005;
      p.a = 1.0f;
      p.ia = 1.0f / ((float)(MIN_PART_DURATION + rand() % (MAX_PART_DURATION - MIN_PART_DURATION)) / (float)PARTICLE_ALFA_INTERVAL);
      if (rand() % 2)
        p.ix = -p.ix;
      if (rand() % 2)
        p.color = 2;
      else
        p.color = 5;
      if (--amount < 1)
        break;
    }
  }
}

void ParticleEngine::shotHits(float px, float py) {
  int amount = 15;
  static float velocity = 2.0f / (1500.0f/PARTICLE_MOV_INTERVAL); // particula leva 2.5 segundos para cruzar a tela
  int rand_num ;
  for (unsigned int i = 0; i < particles_.size(); ++i) {
    particle_t & p = particles_[i];
    if (!p.active) {
      p.active = true;
      p.px = px + (rand() % 100) * 0.00005;
      p.py = py + (rand() % 100) * 0.00005;
      p.iy = p.px - px;
      p.ix = p.py - py;
      p.a = 1.0f;
      p.ia = 1.0f / ((float)(MIN_PART_DURATION + rand() % (MAX_EXP_DURATION - MIN_PART_DURATION)) / (float)PARTICLE_ALFA_INTERVAL);
      if (rand() % 2)
        p.ix = -p.ix;
      if (rand() % 2)
        p.iy = -p.iy;
      if (rand() % 2)
        p.color = 1;
      else
        p.color = 4;
      if (--amount < 1)
        break;
    }
  }
}

}


