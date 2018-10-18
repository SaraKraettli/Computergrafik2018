Exercise2-GroupSED

How we solved the exercise:

We first implemented the bounding-box, because this is independent from the other tasks and after this,
the image generation will be faster.

Then, we implemented the calculation of the normals of the vertices.

At the end, we implemented the ray-triangle-intersection.



Encountered Problems:

Problems in intersect_bounding_box:

 - Getting x, y and z of a vec3-object
   Problem: For our solution, we needed x, y and z of a vec3-object.
   Solve:   use e1 (1, 0, 0), e2 (0, 1, 0) and e3 (0, 0, 1) and the dot-product

Problems in intersect_triangle

 - Create a matrix of 3 vec3-objects and calculate determinant
   Problem: To calculate the determinant of a matrix, we first tried to create a matrix3-class, which can
            compute determinants.
            Another option was to create a function, which can calculate the determinant by passing 3 vec3-objects as clumns-vectors.
            But for both options, we would have needed e1, e2 and e3 to get x, y, z of the vec3-object.
            And this would have decreased the performance very drastic (maybe, we didn't test).

   Solution: Use v2 . (v0 x v1), the dot-product with v2 and (v0 CROSS v1), to calculate the determinant.
             v0, v1 and v2 are vec3's and represent the matrix A = (v0 | v1 | v2)

 - Problem: Mask was first very brown and had no shadows and no bright parts
   Solution: We didn't test if the _intersection_t is positive,
             We used "return true;" instead of "return _intersection_t > 0 ? true : false;"

Problems in compute_normals

 - Problem: Chose the wrong/too complicated initial idea in order to find neigboring  triangles. Triangles were sorted first.
   Solution: Directly calculate weights and modify vertex normals while iterating over triangles.


Fraction of the total workload:

Sara: 33%
Elio: 33%
Dave: 33%