Exercise2-GroupSED

How we solved the exercise:

We first implemented the amient, specular and diffuse contribution.
For this, we set the default-color to the ambient color, and then add up diffuse and specular light.

After this, we implemented shadows.
For the shadows, we created a shadowray and set the center of the shadowray.
Then, for each light, we change the direction of the shadowray and check if
it intersects with the point.
If not, we change the diffuse and specular contribution.

At the end, we implemented the reflections.
For this, we first check if the depth is smaller than max_depth and if the
objects material has the ability to mirror rays.
If so, we create a reflectedRay and set it's direction and origin.
After this, we invoke the method trace recursively with the reflectedRay and with depth+1
This gives us the reflectedColor, with which we partially change the default phong lighting color,
depending on the materials mirror attribute.

Encountered Problems:

 - We had no problems, everthing went fine

Fraction of the total workload:

Sara: 33%
Elio: 33%
Dave: 33%
