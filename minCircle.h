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
float distance(Point a, Point b){
    return sqrt(pow(a.x- b.x,2)+ pow(a.y- b.y,2) );
}
/*
* the function check if a point is inside the circle
*/
bool inside_circle(Circle& c, Point p){
    if (distance(c.center,p) <= c.radius){
        return true;
    }
    return false;
}
/*
* the function check if all points are valid for the gva circle
*/
bool is_valid(Circle& c,vector<Point>& r){
    //lopping trow point in r
    for(const Point&p: r){
        //checking if point is not inside the circle
        if (!inside_circle(c,p)){
            return false;
        }
    }
    return true;
}
/*
* the function return a very trivial min circle from 2 points
*/
Circle circle_from_2points(Point x1,Point x2){
    //middle point of 2 point
    Point center = Point((x1.x+x2.x)/2,(x1.y+x2.y)/2);
    //the radius is the distance between 2 point divide by 2
    float r = distance(x1,x2)/2;
    //returning circle
    return Circle(center, r);
}
/*
 * the function return the unique circle from 3 point using the
 * line equation of the medians(the 3 point create a triangle so, we use there median)
 */
Circle circle_from_3points(Point a,Point b,Point c){
    //calculating slop of the median line of ab segment
    //slop of AB is m= (y2-y1)/(x2-x1)
    float slop_AB = (b.y-a.y)/(b.x-a.x);
    float mid_p_slop_AB = -1/slop_AB;
    Point mid_point_AB = Point((b.x+a.x)/2,(b.y+a.y)/2);
    //calculating slop of the median line of bc segment
    float slop_BC = (c.y-b.y)/(c.x-b.x);
    float mid_p_slop_BC = -1/slop_BC;
    Point mid_point_BC = Point((b.x+c.x)/2,(b.y+c.y)/2);
    /*
    now we know line equation is y-y1=m(x-x1) so we can place the mid-point as x1,y1 for each segment,
    so we could compare the 2 line equation and get the intersection point which is the center of circle
    mid_p_slop_AB(x- mid_point_AB.x) + mid_p_slop_BC.y = mid_p_slop_BC(x- mid_point_BC.x) + mid_p_slop_BC.y
    and after simplification we get:
    x = (mid_point_BC.y -mid_p_slop_BC*mid_point_BC.x + mid_p_slop_AB*mid_point_AB.x - mid_point_AB.y)/
     mid_p_slop_AB -mid_p_slop_BC)
     */
   float x =  (mid_point_BC.y -mid_p_slop_BC*mid_point_BC.x + mid_p_slop_AB*mid_point_AB.x - mid_point_AB.y)/
           (mid_p_slop_AB -mid_p_slop_BC);
   float y = mid_p_slop_AB*(x- mid_point_AB.x) + mid_point_AB.y;
   Point center = Point(x,y);
   //calculating the circle radius and returning min circle
   float r = distance(center,a);
    return Circle(center,r);
}
/*
 * the function return the simple min circle in case when we get
 * to the base condition in our recursion
 */
Circle simple_min_circle(vector<Point>& p){
    //for circle with size 0
    if (p.size() == 0){
        return Circle(Point(0,0),0);
    }
    //for circle with size 1
    if (p.size() == 1){
        return Circle(p[0],0);
    }
    //for circle with size 2
    if(p.size() == 2){
        return circle_from_2points(p[0],p[1]);
    }
    //checking if for every circle that 2 points create the 3rd point contain inside this circle
    for (int i = 0; i < 3; ++i) {
        for (int j = i+1; j < 3; ++j) {
            //creating circle for the 2 points
            Circle circle = circle_from_2points(p[i],p[j]);
            //checking if points inside circle
            if (is_valid(circle,p)){
                return circle;
            }
        }
    }
    //otherwise, there is no such circle so, we return the unique circle of 3 points
    return circle_from_3points(p[0],p[1],p[2]);
}
/*
 * this func implement welzl algorithm and
 * all rest of function are help method for this func
 */
Circle welzl_Algo(Point** p,vector<Point> r,size_t size){
    //checking for base case
    if (size == 0 || r.size() == 3){
        //returning the simple circle
        return simple_min_circle(r);
    }
    //picking random point
    int index = rand()% size;
    Point point= Point(p[index]->x,p[index]->y);
    //swapping the point with the last point in p
    swap(p[index],p[size-1]);
    //getting minimal circle from after we remove the last point
    Circle c = welzl_Algo(p,r,size-1);
    //if point is inside the circle we return c
    if (inside_circle(c,point)){
        return c;
    }
    //we add to r the point we can't add to c
    r.push_back(point);
    //calling the recursion
    return welzl_Algo(p,r,size-1);
}
/*
 * the function that get the pointer to array of point
 * and his size and returns the minimal circle
 */
Circle findMinCircle(Point** points,size_t size){
    //sending the points to the welzl algorithm
    return welzl_Algo(points,{},size);
}
#endif /* MINCIRCLE_H_ */
