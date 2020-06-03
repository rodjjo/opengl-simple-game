#ifndef EVENT_LINSTENER_H_INCLUDED
#define EVENT_LINSTENER_H_INCLUDED

namespace sopa {

class EventListener {
 public:
  virtual ~EventListener(){};
  virtual void fireKeyDown(unsigned char key) {};
  virtual void fireKeyUp(unsigned char key) {};
  virtual void firePaint() {};
  virtual void fireAction() {};
};

}


#endif // EVENT_LINSTENER_H_INCLUDED
