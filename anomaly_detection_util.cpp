//
// Created by dor on 10/14/21.
//

#include <cmath>
#include "anomaly_detection_util.h"
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

// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size) {
    float x [size], y [size];
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
  float xCoefficient = cov(x,y,size)/ var(x,size), avgX = avg(x,size), avgY = avg(y,size),
  numCoefficient = avgY -(xCoefficient * avgX);
   Line line(xCoefficient,numCoefficient);
   return line;
}
float lineY(Line line,float x){
    return (line.a*x) + line.b;
}
// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
  Line line = linear_reg(points,size);
  float pointLineY = lineY(line,p.x);
  float res = (pointLineY - p.y);
  if(res >= 0)
      return res;
  return res*-1;
}

// returns the deviation between point p and the line
float dev(Point p,Line l) {
  float pointLineY = lineY(l,p.x);
  float res = (pointLineY - p.y);
  if(res >= 0)
      return res;
  return res*-1;
}