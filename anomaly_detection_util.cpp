//
// Created by dor on 10/14/21.
//

#include "anomaly_detection_util.h"
#include <math.h>

float var(float* x, int size) {
  float temp = 0, ev = 0;

  // Calc the Expected value
  for (int i=0; i< size; i++) {
    temp += x[i];
  }
  ev = (float)(1.0 / size) * temp;

  // Calc the variance
  temp = 0;
  for (int i=0; i< size; i++) {
    temp += pow(x[i] - ev, 2);
  }
  temp = (float)(1.0 / size) * temp;

  return temp;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
  float e1 = 0, e2 = 0, e3 = 0;

  for (int i=0; i< size; i++) {
    e1 += (x[i] * y[i]);
  }
  e1 = (float)(1.0 / size) * e1;

  for (int i=0; i< size; i++) {
    e2 += (x[i]);
  }
  e3 = (float)(1.0 / size) * e2;

  for (int i=0; i< size; i++) {
    e3 += (y[i]);
  }
  e3 = (float)(1.0 / size) * e3;

  return e1 - (e2 * e3);
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
  return 0;
}
class Line{
 public:
  float a,b;
  Line():a(0),b(0){}
  Line(float a, float b):a(a),b(b){}
  float f(float x) {
    return a*x+b;
  }
};

class Point{
 public:
  float x,y;
  Point(float x, float y):x(x),y(x){}
};

// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size) {
  Line ln;
  return ln;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
  return 0;
}

// returns the deviation between point p and the line
float dev(Point p,Line l) {
  return 0;
}