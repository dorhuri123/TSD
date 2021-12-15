
//Dor Huri 209409218
//Aviya Hadad 314802075
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"
#include <cmath>

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------
/*
* the function return the distance between 2 points
*/
float distance(Point a, Point b);
/*
* the function check if a point is inside the circle
*/
bool inside_circle(Circle& c, Point p);
/*
* the function check if all points are valid for the gva circle
*/
bool is_valid(Circle& c,vector<Point>& r);
/*
* the function return a very trivial min circle from 2 points
*/
Circle circle_from_2points(Point x1,Point x2);
/*
 * the function return the unique circle from 3 point using the
 * line equation of the medians(the 3 point create a triangle so, we use there median)
 */
Circle circle_from_3points(Point a,Point b,Point c);
/*
 * the function return the simple min circle in case when we get
 * to the base condition in our recursion
 */
Circle simple_min_circle(vector<Point>& p);
/*
 * this func implement welzl algorithm and
 * all rest of function are help method for this func
 */
Circle welzl_Algo(Point** p,vector<Point> r,size_t size);
/*
 * the function that get the pointer to array of point
 * and his size and returns the minimal circle
 */
Circle findMinCircle(Point** points,size_t size);
#endif /* MINCIRCLE_H_ */