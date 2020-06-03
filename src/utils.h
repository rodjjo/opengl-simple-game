#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

namespace sopa {

struct fPoint {
  fPoint(){x = 0; y = 0;};
  fPoint(float px, float py){x = px; y = py;};
  float x;
  float y;
};

bool checkDistance(float px, float py, float tx, float ty, float dist_square);
bool checkLineColision(float p0_x, float p0_y, float p1_x, float p1_y,
                       float p2_x, float p2_y, float p3_x, float p3_y);
bool checkPointColision(const fPoint& pt, const fPoint& v1, const fPoint& v2, const fPoint& v3);


}


#endif // UTILS_H_INCLUDED
