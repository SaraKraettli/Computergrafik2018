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
    /** \todo
        * - compute the first valid intersection `_ray` with the cylinder
        *   (valid means in front of the viewer: t > 0)
        * - store intersection point in `_intersection_point`
        * - store ray parameter in `_intersection_t`
        * - store normal at _intersection_point in `_intersection_normal`.
        * - return whether there is an intersection with t > 0
       */

    const vec3 &dir = _ray.direction;
    const vec3   oc = _ray.origin - center;

    std::array<double, 2> t;
    size_t nsol = solveQuadratic(dot(dir, dir) - (dot(dir, this->axis)*dot(dir, this->axis)),
                                 2 * (dot(dir, oc) - dot(dir, this->axis)*dot(oc, this->axis)),
                                 dot(oc, oc) - (dot(oc, this->axis)*dot(oc, this->axis)) - (radius*radius), t);

    _intersection_t = NO_INTERSECTION;

    // Find the closest valid solution (in front of the viewer)
    for (size_t i = 0; i < nsol; ++i) {

        if (t[i] > 0) {
            double memory = _intersection_t;
            _intersection_t = std::min(_intersection_t, t[i]);


            // If the zylinder is less height than the point, go back to previous _intersection_t
            _intersection_point  = _ray(_intersection_t);
            vec3 pointX = operator+(this->center, operator*(dot(operator-(_intersection_point, this->center), this->axis), this->axis));

            if (norm(operator-(this->center, pointX)) > (this->height/2)) {
                _intersection_t = memory;
            }
        }
    }

    if (_intersection_t == NO_INTERSECTION) return false;

    // Calculate _intersection_point and pointX (point on center-axis on same height as _intersection_point)
    _intersection_point  = _ray(_intersection_t);
    vec3 pointX = operator+(this->center, operator*(dot(operator-(_intersection_point, this->center), this->axis), this->axis));

    // Assume intersection is outside the cylinder
    _intersection_normal = operator-(_intersection_point, pointX);

    double a = dot(_ray.direction, _intersection_normal) / (norm(_ray.direction) * norm(_intersection_normal));

    if (a > 0) {
        _intersection_normal = operator-(pointX, _intersection_point);//(_intersection_point - center) / radius;  // TODO: Falsche normale, es sollte der Abstand zur center-linie, nicht zum center-punkt berechnen
    }

    // Normalize _intersection_normal
   _intersection_normal = normalize(_intersection_normal);

    return true;
}
