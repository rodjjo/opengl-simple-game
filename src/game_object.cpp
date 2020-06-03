#include "game_object.h"
#include "utils.h"
#include "accurate_timer.h"
#include "particle_engine.h"
#include <gl/gl.h>

#define EMISSION_INTERVAL 33

namespace sopa {

GameObject::GameObject(ParticleEngine *part_eng)  {
  part_eng_ = part_eng;
  py_ = 0.0f;
  px_ = 0.0f;
  active_ = false;
  destroyed_ = false;
  cr_ = 1.0f;
  cg_ = 1.0f;
  cb_ = 1.0f;
  min_x_ = 1.0f;
  max_x_ = -1.0f;
  min_y_ = 1.0f;
  max_y_ = -1.0f;
  width_ = 0.0f;
  height_ = 0.0f;
  destroyed_at_ = 0;
  last_emission_ = 0;
}

GameObject::~GameObject() {
}

void GameObject::emit(bool up) {
  if (getCurrentTime() - last_emission_ > EMISSION_INTERVAL) {
    for (unsigned int i = 0; i < emission_point_.size(); ++i)
      part_eng_->emission(emission_point_[i].first+px_, emission_point_[i].second+py_, up);
    last_emission_ = getCurrentTime();
  }
}

bool GameObject::getActive() {
  return active_;
}

void GameObject::deactivate() {
  active_ = false;
}

void GameObject::paint() {
  if (!active_)
    return;
  if (destroyed_) {
    // pintar objeto destruido
    return;
  }
  glBegin(GL_TRIANGLES);
  for (iter_ = vertexList_.begin(); iter_ != vertexList_.end(); ++iter_) {
    glColor3f(iter_->cr,iter_->cg, iter_->cb);
    glVertex2f(px_ + iter_->px, py_ + iter_->py);
  }
  glEnd();
}

bool GameObject::getCoordinates(float *cx, float *cy) {
  *cx = px_;
  *cy = py_;
}

float GameObject::getWidth() {
  return width_;
}

float GameObject::getHeight() {
  return height_;
}

void GameObject::move(float px, float py) {
  if (px < -1.0f)
    px_ = -1.0f;
  else if (px > 1.0f)
    px_ = 1.0f;
  else
    px_ = px;
  if (py < -1.0f)
    py_ = -1.0f;
  else if (py > 1.0f)
    py_ = 1.0f;
  else
    py_ = py;
}

void GameObject::destroy() {
  if (destroyed_)
    return;
  part_eng_->explosion(px_, py_);
  destroyed_at_ = getCurrentTime();
  destroyed_ = true;
}

bool GameObject::getDestroyed() {
  return destroyed_;
}

unsigned int GameObject::getDestroyedTime() {
  return destroyed_at_;
}

void GameObject::activate(float wx, float wy) {
  destroyed_ = false;
  destroyed_at_ = getCurrentTime();
  active_ = true;
  px_ = wx;
  py_ = wy;
}

void GameObject::addEmissionPoint(float px, float py) {
  emission_point_.push_back(std::make_pair(px, py));
}

void GameObject::colorVertex(float r, float g, float b) {
  cr_ = r;
  cg_ = g;
  cb_ = b;
}

void GameObject::nextVertex(float px, float py) {
  vertexList_.push_back(vertex_t(px, py, cr_, cg_, cb_));
  if (px > max_x_)
    max_x_ = px;
  if (py > max_y_)
    max_y_ = py;
  if (px < min_x_)
    min_x_ = px;
  if (py < min_y_)
    min_y_ = py;
  width_ = max_x_ - min_x_;
  height_ = max_y_ - min_y_;
  col_dist_ = (width_ > height_ ? width_ : height_) * 0.5f;
  col_dist_ *= col_dist_; // quadrado da distancia para evitar extrair raiz
}

bool GameObject::checkLineIntersection(float x1, float y1, float x2, float y2) {
  char count = 0;
  unsigned int index = 0;
  while (index + 3 <= vertexList_.size()) {
    if (checkLineColision(vertexList_[index].px + px_, vertexList_[index].py + py_,
                        vertexList_[index+1].px + px_, vertexList_[index+1].py + py_,
                        x1, y1, x2, y2)) {
      destroy();
      return true;
    }
    if (checkLineColision(vertexList_[index].px + px_, vertexList_[index].py + py_,
                        vertexList_[index+2].px + px_, vertexList_[index+2].py + py_,
                        x1, y1, x2, y2)) {
      destroy();
      return true;
    }
    if (checkLineColision(vertexList_[index+2].px + px_, vertexList_[index+2].py + py_,
                        vertexList_[index+1].px + px_, vertexList_[index+1].py + py_,
                        x1, y1, x2, y2)) {
      destroy();
      return true;
    }
    index += 3;
  }
  return false;
}

int GameObject::checkColision(float px, float py, bool should_kill) {
  if (!active_ || destroyed_)
    return 0;
  unsigned int index = 0;
  fPoint p(px, py);
  while (index + 3 <= vertexList_.size()) {
    if(checkPointColision(p,
    fPoint(vertexList_[index].px + px_, vertexList_[index].py + py_),
    fPoint(vertexList_[index+1].px + px_, vertexList_[index+1].py + py_),
    fPoint(vertexList_[index+2].px + px_, vertexList_[index+2].py + py_))) {
      if (should_kill)
        destroy();
      return should_kill ? 1 : 2;
    }
    index += 3;
  }
  return 0;
}

int GameObject::checkColision(GameObject* object, bool should_kill) {
  if (!object)
    return 0;
  if (!active_ || !object->active_ || destroyed_ || object->destroyed_)
    return 0;
  unsigned int index = 0;
  while (index + 3 <= vertexList_.size()) {
    if (object->checkLineIntersection(vertexList_[index].px + px_, vertexList_[index].py + py_,
                                      vertexList_[index+1].px + px_, vertexList_[index+1].py + py_)) {
      if (should_kill)
        destroy();
      return should_kill ? 1 : 2;
    }
    if (object->checkLineIntersection(vertexList_[index].px + px_, vertexList_[index].py + py_,
                                      vertexList_[index+2].px + px_, vertexList_[index+2].py + py_)) {
      if (should_kill)
        destroy();
      return should_kill ? 1 : 2;
    }
    if (object->checkLineIntersection(vertexList_[index+1].px + px_, vertexList_[index+1].py + py_,
                                      vertexList_[index+2].px + px_, vertexList_[index+2].py + py_)) {
      if (should_kill)
        destroy();
      return should_kill ? 1 : 2;
    }
    index += 3;
  }
  return 0;
}

}
