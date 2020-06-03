#include "utils.h"

namespace sopa {

bool checkDistance(float px, float py, float tx, float ty, float dist_square) {
  float dx = px > tx ? px - tx : tx - px;
  float dy = py > ty ? py - ty : ty - py;
  dx *= dx;
  dy *= dy;
  return dx + dy < dist_square;
}

bool checkLineColision(float p0_x, float p0_y, float p1_x, float p1_y,
                       float p2_x, float p2_y, float p3_x, float p3_y) {
  float denominator = ((p1_x - p0_x) * (p3_y - p2_y)) - ((p1_y - p0_y) * (p3_x - p2_x));
  float numerator1 = ((p0_y - p2_y) * (p3_x - p2_x)) - ((p0_x - p2_x) * (p3_y - p2_y));
  float numerator2 = ((p0_y - p2_y) * (p1_x - p0_x)) - ((p0_x - p2_x) * (p1_y - p0_y));

  if (denominator == 0.0f) return numerator1 == 0.0f && numerator2 == 0.0f;

  float r = numerator1 / denominator;
  float s = numerator2 / denominator;

  return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

float sign(const fPoint& p1, const fPoint& p2, const fPoint& p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool checkPointColision(const fPoint& pt, const fPoint& v1, const fPoint& v2, const fPoint& v3) {
    bool b1, b2, b3;

    b1 = sign(pt, v1, v2) < 0.0f;
    b2 = sign(pt, v2, v3) < 0.0f;
    b3 = sign(pt, v3, v1) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

}
