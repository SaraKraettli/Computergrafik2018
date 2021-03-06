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

#include "Mesh.h"
#include "Plane.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <limits>
#include <array>
#include <list>
#include <vector>



//== IMPLEMENTATION ===========================================================


Mesh::Mesh(std::istream &is, const std::string &scenePath)
{
    std::string meshFile, mode;
    is >> meshFile;

    // load mesh from file
    read(scenePath.substr(0, scenePath.find_last_of('/') + 1) + meshFile);

    is >> mode;
    if      (mode ==  "FLAT") draw_mode_ = FLAT;
    else if (mode == "PHONG") draw_mode_ = PHONG;
    else throw std::runtime_error("Invalid draw mode " + mode);

    is >> material;
}


//-----------------------------------------------------------------------------


bool Mesh::read(const std::string &_filename)
{
    // read a mesh in OFF format


    // open file
    std::ifstream ifs(_filename);
    if (!ifs)
    {
        std::cerr << "Can't open " << _filename << "\n";
        return false;
    }


    // read OFF header
    std::string s;
    unsigned int nV, nF, dummy, i;
    ifs >> s;
    if (s != "OFF")
    {
        std::cerr << "No OFF file\n";
        return false;
    }
    ifs >> nV >> nF >> dummy;
    std::cout << "\n  read " << _filename << ": " << nV << " vertices, " << nF << " triangles";


    // read vertices
    Vertex v;
    vertices_.clear();
    vertices_.reserve(nV);
    for (i=0; i<nV; ++i)
    {
        ifs >> v.position;
        vertices_.push_back(v);
    }


    // read triangles
    Triangle t;
    triangles_.clear();
    triangles_.reserve(nF);
    for (i=0; i<nF; ++i)
    {
        ifs >> dummy >> t.i0 >> t.i1 >> t.i2;
        triangles_.push_back(t);
    }


    // close file
    ifs.close();


    // compute face and vertex normals
    compute_normals();

    // compute bounding box
    compute_bounding_box();


    return true;
}


//-----------------------------------------------------------------------------

// Determine the weights by which to scale triangle (p0, p1, p2)'s normal when
// accumulating the vertex normals for vertices 0, 1, and 2.
// (Recall, vertex normals are a weighted average of their incident triangles'
// normals, and in our raytracer we'll use the incident angles as weights.)
// \param[in] p0, p1, p2    triangle vertex positions
// \param[out] w0, w1, w2    weights to be used for vertices 0, 1, and 2
void angleWeights(const vec3 &p0, const vec3 &p1, const vec3 &p2,
                  double &w0, double &w1, double &w2) {
    // compute angle weights
    const vec3 e01 = normalize(p1-p0);
    const vec3 e12 = normalize(p2-p1);
    const vec3 e20 = normalize(p0-p2);
    w0 = acos( std::max(-1.0, std::min(1.0, dot(e01, -e20) )));
    w1 = acos( std::max(-1.0, std::min(1.0, dot(e12, -e01) )));
    w2 = acos( std::max(-1.0, std::min(1.0, dot(e20, -e12) )));
}


//-----------------------------------------------------------------------------

void Mesh::compute_normals()
{
    // compute triangle normals
    for (Triangle& t: triangles_)
    {
        const vec3& p0 = vertices_[t.i0].position;
        const vec3& p1 = vertices_[t.i1].position;
        const vec3& p2 = vertices_[t.i2].position;
        t.normal = normalize(cross(p1-p0, p2-p0));
    }

    // initialize vertex normals to zero
    for (Vertex& v: vertices_)
    {
        v.normal = vec3(0,0,0);
    }

    /** \todo
     * In some scenes (e.g the office scene) some objects should be flat
     * shaded (e.g. the desk) while other objects should be Phong shaded to appear
     * realistic (e.g. chairs). You have to implement the following:
     * - Compute vertex normals by averaging the normals of their incident triangles.
     * - Store the vertex normals in the Vertex::normal member variable.
     * - Weigh the normals by their triangles' angles.
     */
	
	for (Triangle& t : triangles_)
	{
		const vec3& p0 = vertices_[t.i0].position;
        const vec3& p1 = vertices_[t.i1].position;
        const vec3& p2 = vertices_[t.i2].position;
		double w0, w1, w2;
		angleWeights(p0, p1, p2, w0, w1, w2);
		vertices_[t.i0].normal += t.normal*w0;
		vertices_[t.i1].normal += t.normal*w1;
		vertices_[t.i2].normal += t.normal*w2;
	}

	for (Vertex& v : vertices_)
	{
		v.normal = normalize(v.normal);
	}
}


//-----------------------------------------------------------------------------


void Mesh::compute_bounding_box()
{
    bb_min_ = vec3(std::numeric_limits<double>::max());
    bb_max_ = vec3(std::numeric_limits<double>::lowest());

    for (Vertex v: vertices_)
    {
        bb_min_ = min(bb_min_, v.position);
        bb_max_ = max(bb_max_, v.position);
    }
}


//-----------------------------------------------------------------------------


// Calculate, if the ray intersects a finite Plane, given by a center and two points vec1 and vec2
// To avoid a some math-problems we couldn't solve, the method requires some more parameters:
//
// - _eCheck1, _bbCheck1min, _bbCheck1max (note: min/max denotes if the variable is from bb_min_ or bb_max_)
// - _eCheck2, _bbCheck2min, _bbCheck2max
//
bool intersectPlane2Vectors(const Ray& _ray, const vec3& _center, const vec3& _vec1, const vec3& _vec2,
                             const vec3& _eCheck1, const double& _bbCheck1min, const double& _bbCheck1max,
                             const vec3& _eCheck2, const double& _bbCheck2min, const double& _bbCheck2max) {
    vec3 _intersection_point;
    vec3 _intersection_normal;
    double _intersection_t;

    vec3 e1 = vec3(1, 0, 0);
    vec3 e2 = vec3(0, 1, 0);
    vec3 e3 = vec3(0, 0, 1);

    // Create Plane
    Plane planeMinXY_XZ = Plane(_center, cross(operator-(_vec1, _center), operator-(_vec1, _vec2)));

    // If ray intersects plane, check if the intersection point is between center/vec1 AND center/vec2
    if (planeMinXY_XZ.intersect(_ray, _intersection_point, _intersection_normal, _intersection_t)) {

        // Check if x, y or z (depending on _eCheck1) is between _bbCheck1min and _bbCheck1max
        // Note: _bbCheck1min doesn't mean it's smaller than _bbCheck1max, but that it's from bb_min_
        if ((dot(_intersection_point, _eCheck1) <= std::fmax(_bbCheck1min, _bbCheck1max)) && (dot(_intersection_point, _eCheck1) >= std::fmin(_bbCheck1min, _bbCheck1max))) {

            // Check if x, y or z (depending on _eCheck2) is between _bbCheck2min and _bbCheck2max
            if ((dot(_intersection_point, _eCheck2) <= std::fmax(_bbCheck2min, _bbCheck2max)) && (dot(_intersection_point, _eCheck2) >= std::fmin(_bbCheck2min, _bbCheck2max))) {
                return true;
            }
        }
    }
    return false;
}

bool Mesh::intersect_bounding_box(const Ray& _ray) const
{
    /** \todo
    * Intersect the ray `_ray` with the axis-aligned bounding box of the mesh.
    * Note that the minimum and maximum point of the bounding box are stored
    * in the member variables `bb_min_` and `bb_max_`. Return whether the ray
    * intersects the bounding box.
    * This function is ued in `Mesh::intersect()` to avoid the intersection test
    * with all triangles of every mesh in the scene. The bounding boxes are computed
    * in `Mesh::compute_bounding_box()`.
    */

    // To get x, y, and z of a vec3 object with the dot-product
    // Not the best solution, but we found no other one and assumed it's not allowed to change the vec3 class
    // e.g. generate getters or change access of x,y,z to public
    vec3 e1 = vec3(1, 0, 0);
    vec3 e2 = vec3(0, 1, 0);
    vec3 e3 = vec3(0, 0, 1);

    // Get x, y and z of bb_min_ and of bb_max_
    double bbminX = dot(this->bb_min_, e1);
    double bbminY = dot(this->bb_min_, e2);
    double bbminZ = dot(this->bb_min_, e3);

    double bbmaxX = dot(this->bb_max_, e1);
    double bbmaxY = dot(this->bb_max_, e2);
    double bbmaxZ = dot(this->bb_max_, e3);


    // Get the remaining 6 points of the cube
    // Assuming bb_min_ and bb_max_ are diagonal, then
    // it's obvious that 3 points of the remaining 6 points are directly connected to bb_min_ and not directly
    // connected to bb_max_, and the other 3 points are directly connected to bb_max_ and not directly connected
    // to bb_min_.
    // By directly connected, we mean that there is just one edge between the points
    //
    vec3 bbminxy = vec3(bbminX, bbminY, bbmaxZ); //x and y of bbmin, z of bbmax
    vec3 bbminxz = vec3(bbminX, bbmaxY, bbminZ);
    vec3 bbminyz = vec3(bbmaxX, bbminY, bbminZ);

    // first vector: x and y of bbmax, z of bbmin
    vec3 bbmaxxy = vec3(bbmaxX, bbmaxY, bbminZ);
    vec3 bbmaxxz = vec3(bbmaxX, bbminY, bbmaxZ);
    vec3 bbmaxyz = vec3(bbminX, bbmaxY, bbmaxZ);


    // Test for each side of the cube for intersection with the ray
    if (intersectPlane2Vectors(_ray, bb_min_, bbminxy, bbminxz, e3, bbminZ, bbmaxZ, e2, bbminY, bbmaxY)) return true;
    if (intersectPlane2Vectors(_ray, bb_min_, bbminxy, bbminyz, e3, bbminZ, bbmaxZ, e1, bbminX, bbmaxX)) return true;
    if (intersectPlane2Vectors(_ray, bb_min_, bbminxz, bbminyz, e2, bbminY, bbmaxY, e1, bbminX, bbmaxX)) return true;

    if (intersectPlane2Vectors(_ray, bb_max_, bbmaxxy, bbmaxxz, e3, bbminZ, bbmaxZ, e2, bbminY, bbmaxY)) return true;
    if (intersectPlane2Vectors(_ray, bb_max_, bbmaxxy, bbmaxyz, e3, bbminZ, bbmaxZ, e1, bbminX, bbmaxX)) return true;
    if (intersectPlane2Vectors(_ray, bb_max_, bbmaxxz, bbmaxyz, e2, bbminY, bbmaxY, e1, bbminX, bbmaxX)) return true;

    return false;
}






//-----------------------------------------------------------------------------


bool Mesh::intersect(const Ray& _ray,
                     vec3&      _intersection_point,
                     vec3&      _intersection_normal,
                     double&    _intersection_t ) const
{
    // check bounding box intersection
    if (!intersect_bounding_box(_ray))
    {
        return false;
    }

    vec3   p, n;
    double t;

    _intersection_t = NO_INTERSECTION;

    // for each triangle
    for (const Triangle& triangle : triangles_)
    {
        // does ray intersect triangle?
        if (intersect_triangle(triangle, _ray, p, n, t))
        {
            // is intersection closer than previous intersections?
            if (t < _intersection_t)
            {
                // store data of this intersection
                _intersection_t      = t;
                _intersection_point  = p;
                _intersection_normal = n;
            }
        }
    }

    return (_intersection_t != NO_INTERSECTION);
}


//-----------------------------------------------------------------------------


bool
Mesh::
intersect_triangle(const Triangle&  _triangle,
                   const Ray&       _ray,
                   vec3&            _intersection_point,
                   vec3&            _intersection_normal,
                   double&          _intersection_t) const
{
    const vec3& p0 = vertices_[_triangle.i0].position;
    const vec3& p1 = vertices_[_triangle.i1].position;
    const vec3& p2 = vertices_[_triangle.i2].position;

    /** \todo
    * - intersect _ray with _triangle
    * - store intersection point in `_intersection_point`
    * - store ray parameter in `_intersection_t`
    * - store normal at intersection point in `_intersection_normal`.
    * - Depending on the member variable `draw_mode_`, use either the triangle
    *  normal (`Triangle::normal`) or interpolate the vertex normals (`Vertex::normal`).
    * - return `true` if there is an intersection with t > 0 (in front of the viewer)
    *
    * Hint: Rearrange `ray.origin + t*ray.dir = a*p0 + b*p1 + (1-a-b)*p2` to obtain a solvable
    * system for a, b and t.
    * Refer to [Cramer's Rule](https://en.wikipedia.org/wiki/Cramer%27s_rule) to easily solve it.
     */

    // Matrix A has column vectors v0, v1, v2
    vec3 v0 = operator-(p0, p2);
    vec3 v1 = operator-(p1, p2);
    vec3 v2 = operator-(_ray.direction);

    // Solution-vector b of Cramer's Rule
    vec3 b = operator-(_ray.origin, p2);

    double matrixADeterminant = dot(cross(v0, v1), v2);

    // Calculating alpha, beta and gamma by applying Cramer's Rule
    double alpha = dot(cross(b, v1), v2) / matrixADeterminant; // Det(A_0) / Det(A)
    double beta = dot(cross(v0, b), v2) / matrixADeterminant;  // A_0 means the matrix (b | v1 | v2)
    double gamma = 1 - alpha - beta;
    _intersection_t = dot(cross(v0, v1), b) / matrixADeterminant;

    // If point is inside the triangle
    if (alpha >= 0 && beta >= 0 && gamma >= 0) {

        // Calculate intersection point
        _intersection_point = operator+(operator+(operator*(alpha, p0), operator*(beta, p1)), operator*(gamma, p2));

        if (draw_mode_ ==  FLAT) {
            _intersection_normal = _triangle.normal;
        } else {
            _intersection_normal = operator+(
                    operator+(alpha*vertices_[_triangle.i0].normal, beta*vertices_[_triangle.i1].normal),
                    gamma*vertices_[_triangle.i2].normal);
        }

        return _intersection_t > 0 ? true : false;
    } else return false;
}

//=============================================================================
