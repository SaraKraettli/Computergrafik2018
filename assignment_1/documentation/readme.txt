Exercise1-GroupSED

How we solved the exercise:

We first implemented the ray-plane-intersection.
For this, we changed the class "Plane.cpp".
To test if the implementation works, we executed raytrace.exe with the cylinder-scene as input.
The output should be an image with a green background

Then we made the ray-cylinder intersection and normal derivations, because this is
needed for the remaining part of the exercise.

After this, we implemented the ray-cylinder intersection.
For this, we needed to change the file "Cylinder.cpp".




Encountered Problems:

 - How to test if ray-plane-intersection works

 - bug: only half cylinder was visible
   fix: do not return if the nearer intersection_t is too height,
        but test the second intersection_t if it's valid

 - bug: Colors inside the cylinder were wrong
   fix: Calculate other _intersection_normal if the point is inside the cylinder
        (if intersection-point is inside cylinder, the ray has to reflect to the center-line, unlike if the
        intersection-point is outside the cylinder (then is has to go away from the center-line))



Fraction of the total workload:

Sara: 33%
Elio: 33%
Dave: 33%
