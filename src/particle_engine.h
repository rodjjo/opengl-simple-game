#ifndef PARTICLE_ENGINE_H_INCLUDED
#define PARTICLE_ENGINE_H_INCLUDED

#include "event_linstener.h"
#include <vector>

namespace sopa {

struct particle_t {
  bool active;
  float size;
  char color;
  float a;
  float px;
  float py;
  float ix;
  float iy;
  float ia;
};

class ParticleEngine : public EventListener {
 public:
  ParticleEngine();
  virtual ~ParticleEngine();
  virtual void firePaint() ;
  virtual void fireAction();
  virtual void explosion(float px, float py);
  virtual void emission(float px, float py, bool up);
  virtual void shotHits(float px, float py);

 private:
  std::vector<particle_t> particles_;
  unsigned int last_part_movs_;
  unsigned int last_part_alfa_;
};

}

#endif // PARTICLE_ENGINE_H_INCLUDED
