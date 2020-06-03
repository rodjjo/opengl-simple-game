#ifndef GAME_OBJECT_H_INCLUDED
#define GAME_OBJECT_H_INCLUDED

#include <vector>

namespace sopa {

struct vertex_t {
  float cr;
  float cg;
  float cb;
  float px;
  float py;
  vertex_t(float x, float y, float r, float g, float b)
    : cr(r), cg(g), cb(b), px(x), py(y) {
  }
};

class ParticleEngine;

class GameObject {
 public:
  GameObject(ParticleEngine *part_eng);
  virtual ~GameObject();
  virtual bool getActive();
  virtual void paint();
  void move(float px, float py);
  virtual bool getCoordinates(float *cx, float *cy);
  virtual void destroy();
  bool getDestroyed();
  virtual unsigned int getDestroyedTime();
  virtual void activate(float wx, float wy);
  virtual int checkColision(GameObject* object, bool should_kill);
  virtual int checkColision(float px, float py, bool should_kill);
  float getWidth();
  float getHeight();
  void deactivate();
  void addEmissionPoint(float px, float py);
  virtual void emit(bool up);

 protected:
  void colorVertex(float r, float g, float b);
  void nextVertex(float px, float py);
 private:
  bool checkLineIntersection(float x1, float y1, float x2, float y2);
  float py_;
  float px_;
  bool active_;
  bool destroyed_;
  float cr_;
  float cg_;
  float cb_;
  float min_x_;
  float max_x_;
  float min_y_;
  float max_y_;
  float width_;
  float height_;
  float col_dist_;
  std::vector<std::pair<float,float> > emission_point_;
  std::vector<vertex_t>::iterator iter_;
  std::vector<vertex_t> vertexList_;
  unsigned long destroyed_at_;
  unsigned long last_emission_;
  ParticleEngine *part_eng_;
};

}



#endif // GAME_OBJECT_H_INCLUDED
