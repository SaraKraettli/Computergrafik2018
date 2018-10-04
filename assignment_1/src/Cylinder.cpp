//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

//== INCLUDES =================================================================

#include "Cylinder.h"
#include "SolveQuadratic.h"

#include <array>
#include <cmath>

//== IMPLEMENTATION =========================================================

bool
Cylinder::
intersect(const Ray&  _ray,
          vec3&       _intersection_point,
          vec3&       _intersection_normal,
          double&     _intersection_t) const
{


    const vec3 &dir = _ray.direction;
    const vec3   oc = _ray.origin - center;

    std::array<double, 2> t;
    size_t nsol = solveQuadratic(dot(dir, dir) - (dot(dir, this->axis)*dot(dir, this->axis)),
                                 2 * (dot(dir, oc) - dot(dir, this->axis)*dot(oc, this->axis)),
                                 dot(oc, oc) - (dot(oc, this->axis)*dot(oc, this->axis)) - (radius*radius), t);

    _intersection_t = NO_INTERSECTION;
    bool inside = false;

    // Find the closest valid solution (in front of the viewer)
    for (size_t i = 0; i < nsol; ++i) {

        if (t[i] > 0) {
            double memory = _intersection_t;
            _intersection_t = std::min(_intersection_t, t[i]);
            _intersection_point  = _ray(_intersection_t);
            vec3 pointX = operator+(this->center, operator*(dot(operator-(_intersection_point, this->center), this->axis), this->axis));
            if (norm(operator-(this->center, pointX)) > (this->height/2)) {
                _intersection_t = memory;
                inside = true;
            }

            /* double memory = _intersection_t;
             _intersection_t = std::min(_intersection_t, t[i]);
             _intersection_point  = _ray(_intersection_t);


             vec3 pointX = operator+(this->center, operator*(dot(operator-(_intersection_point, this->center), this->axis), this->axis));
             if (norm(operator-(this->center, pointX)) > (this->height/2)) {
                 _intersection_t = memory;
                 _intersection_point  = _ray(_intersection_t);
                 vec3 pointX = operator+(this->center, operator*(dot(operator-(_intersection_point, this->center), this->axis), this->axis));
                 _intersection_normal = operator-(_intersection_point, pointX);//(_intersection_point - center) / radius;  // TODO: Falsche normale, es sollte der Abstand zur center-linie, nicht zum center-punkt berechnen


             } else {
                 _intersection_normal = operator-(_intersection_point, pointX);//(_intersection_point - center) / radius;  // TODO: Falsche normale, es sollte der Abstand zur center-linie, nicht zum center-punkt berechnen

             }*/



        }



    }



    if (_intersection_t == NO_INTERSECTION) return false;
    _intersection_point  = _ray(_intersection_t);
    vec3 pointX = operator+(this->center, operator*(dot(operator-(_intersection_point, this->center), this->axis), this->axis));
    _intersection_normal = operator-(_intersection_point, pointX);




    /*operator<<(std::cout, _ray.direction);
    std::printf(" #### ");
    operator<<(std::cout, _intersection_normal);
    std::printf(" #### ");
   double winkel = dot(_ray.direction, _intersection_normal) / (norm(_ray.direction)*norm(_intersection_normal));

   std::printf("%d\n\n\n\n", winkel);*/





    double dot2 = dot(_ray.direction, _intersection_normal);
    double len1 = norm(_ray.direction);
    double len2 = norm(_intersection_normal);

    double a = dot2 / (len1 * len2);
    std::printf("%d ## %d ## %d ## %d\n", dot2, len1, len2, a);

    /*if(winkel < 0) {
        _intersection_normal = operator-(pointX, _intersection_point);//(_intersection_point - center) / radius;  // TODO: Falsche normale, es sollte der Abstand zur center-linie, nicht zum center-punkt berechnen
        //std::cout << "hier";
    }*/

   // _intersection_normal = norm(_intersection_normal);


    /** \todo
     * - compute the first valid intersection `_ray` with the cylinder
     *   (valid means in front of the viewer: t > 0)
     * - store intersection point in `_intersection_point`
     * - store ray parameter in `_intersection_t`
     * - store normal at _intersection_point in `_intersection_normal`.
     * - return whether there is an intersection with t > 0
    */
    return true;
}
