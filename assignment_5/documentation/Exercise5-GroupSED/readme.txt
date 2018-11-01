Exercise5-GroupSED

How we solved the exercise:

 - First, we implemented the positioning of all planets (update_body_positions)
 - Then, we implemented the paint-method, which sets the camera to the
   right position
 - After that, we implemented draw_scene for rendering of the planets
 - Finally, we fixed all bugs in the code and implemented
   the spaceship

Problems we encountered:

 - The planets were accelerating instead of continuously move around the sun
    -> Fix: update_body_positions set the position of the planets
            by using the current position and the angle_orbit.
            Instead, it should have used angle_orbit_step.
            But we needed to change this method anyway, because we didn't use
            the distance and angle_orbit, but the current position of the planet.

 - Mercury and venus moved too slow around the sun
    - Fix: There were two bugs:
            1. Planet-positions were false, because the angle_orbit is a very small number.
               We needed to multiply it by 360 to move the planets faster
            2. The draw_scene translated the planets first and then rotated them with angle_self.
               This let the planets look like there were moving around the sun, but in fact they were
               in a line (z-axis), moving very slow.

 - The camera-rotation around the sun was correct, but on other planets, it moved back and forth
    Fix: The eye was rotated after translating it to the planet
         Instead, it should have been rotated and then translated to the planet

 - The screen was sometimes white, when the eye was directly above the planet
    Fix: Added a float "hack" to change the x-rotation a little bit


Fraction of the total workload:

Sara Kraettli: 33%
Elio Fritschi: 33%
Dave Meier: 33%
