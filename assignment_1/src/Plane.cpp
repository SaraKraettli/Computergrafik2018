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

#include "Plane.h"
#include <limits>


//== CLASS DEFINITION =========================================================



Plane::Plane(const vec3& _center, const vec3& _normal)
: center(_center), normal(_normal)
{
}


//-----------------------------------------------------------------------------


bool
Plane::
intersect(const Ray& _ray,
          vec3&      _intersection_point,
          vec3&      _intersection_normal,
          double&    _intersection_t ) const
{

    /** \todo
 * - compute the intersection of the plane with `_ray`
 * - if ray and plane are parallel there is no intersection
 * - otherwise compute intersection data and store it in `_intersection_point`, `_intersection_normal`, and `_intersection_t`.
 * - return whether there is an intersection in front of the viewer (t > 0)
*/

    double dividor = dot(this->normal, _ray.direction);

    if (dividor == 0) {
        return false;
    }

    _intersection_t = (dot(this->normal, operator-(this->center, _ray.origin))) / dividor;
    _intersection_normal = this->normal;
    _intersection_point = _ray(_intersection_t);

    double a = dot(_ray.direction, _intersection_normal) / (norm(_ray.direction) * norm(_intersection_normal));

    if (a > 0) {
        _intersection_normal = operator-(_intersection_normal);//(_intersection_point - center) / radius;  // TODO: Falsche normale, es sollte der Abstand zur center-linie, nicht zum center-punkt berechnen
    }

    return _intersection_t > 0 ? true : false;
}


//=============================================================================
