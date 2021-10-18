//
// Created by dor on 10/14/21.
//

#include <math.h>
float avg(float* x, int size){
  float res = 0;
  for(int i = 0; i < size; i++){
    res += x[i];
  }

  return res / size;
}

float var(float* x, int size) {
  float res = 0, ev = 0;

  ev = avg(x, size);
  for (int i=0; i< size; i++) {
    res += x[i]*x[i];
  }
  res = (res / size) - ev * ev;
  return res;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    float sum = 0,avgX = avg(x,size), avgY = avg(y,size);
    for (int i=0; i < size; i++){
        sum += (x[i]- avgX)*(y[i] - avgY);
    }
    return sum / size;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
  float covRes, varX, varY;
  covRes = cov(x,y,size);
  varX = var(x, size);
  varY = var(y, size);
  return covRes / sqrt(varX * varY);
}

class Line {
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
  Point(float x, float y):x(x),y(y){}
};

// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size) {
    float x [size], y [size];
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
  float xCoefficient = cov(x,y,size), avgX = avg(x,size), avgY = avg(y,size),
  numCoefficient = avgY -(xCoefficient * avgX);
   Line line(xCoefficient,numCoefficient);
   return line;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
  return 0;
}

// returns the deviation between point p and the line
float dev(Point p,Line l) {
  return 0;
}