//
// Created by dor on 10/14/21.
//Dor Huri 209409218
//Aviya Hadad 314802075
//

#include <cmath>
#include "anomaly_detection_util.h"
/*
 * the function get a pointer to float array
 * and his size and return the average value
 */
float avg(float* x, int size){
    float res = 0;
    for(int i = 0; i < size; i++){
        //summing value
        res += x[i];
    }
    //divide in size and return the average
    return res / (float)size;
}
/*
 * the function get a pointer to float array
 * and his size and return the variance value for x sample
 */
float var(float* x, int size) {
    float res = 0, ev = 0;
    //getting the average
    ev = avg(x, size);
    for (int i=0; i< size; i++) {
        //summing all the value of x in square
        res += x[i]*x[i];
    }
    //return variance according to formula
    return (res / size) - ev * ev;
}
/*
 * the function get 2 pointers to float arrays
 * and his size and return the covariance of X and Y
 */
float cov(float* x, float* y, int size) {
    float sum = 0,avgX = avg(x,size), avgY = avg(y,size);
    for (int i=0; i < size; i++){
        //summing according to formula
        sum += (x[i]- avgX)*(y[i] - avgY);
    }
    //dividing by size for average then we return the covariance of X and Y
    return sum / size;
}
/*
 * the function get 2 pointers to float arrays
 * and his size and return the Pearson correlation coefficient of X and Y
 */
float pearson(float* x, float* y, int size) {
    float covRes, varX, varY;
    //assigning value for covariance and variance
    covRes = cov(x,y,size);
    varX = var(x, size);
    varY = var(y, size);
    //returning correlation coefficient of X and Y according to formula
    return covRes / (sqrt(varX) * sqrt(varY));
}
/*
 * the function get pointer to point pointer and his size and
 * performs a linear regression and return s the line equation
 */
Line linear_reg(Point** points, int size) {
    float x [size], y [size];
    for (int i = 0; i < size; ++i) {
        //assigning the values of the points
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    //getting value of x coefficient for line equation and x,y average
    float xCoefficient = cov(x,y,size)/ var(x,size), avgX = avg(x,size), avgY = avg(y,size),
    //calculating the free number in line equation
    numCoefficient = avgY -(xCoefficient * avgX);
    //creating line object
    Line line(xCoefficient,numCoefficient);
    //returning the line linear regression
    return line;
}
/*
 * the function return the line equation y value
 */
float lineY(Line line,float x){
    return (line.a*x) + line.b;
}
/*
 * the function get pointer to point pointer and his size and
 * returns the deviation between point p and the line equation of the points
 */
float dev(Point p,Point** points, int size) {
    //getting the line equation from point pointer array with linear regression
    Line line = linear_reg(points,size);
    //getting y value in x coordinate of point
    float pointLineY = lineY(line,p.x);
    //subtraction between point y value and line y value according to formula
    float res = (pointLineY - p.y);
    //returning result in absolute value
    if(res >= 0)
        return res;
    return res*-1;
}
/*
 * the function get pointer to point pointer and his size and
 * returns the deviation between point p and the line
 */
float dev(Point p,Line l) {
    //getting y value in x coordinate of point
    float pointLineY = lineY(l,p.x);
    //subtraction between point y value and line y value according to formula
    float res = (pointLineY - p.y);
    //returning result in absolute value
    if(res >= 0)
        return res;
    return res*-1;
}