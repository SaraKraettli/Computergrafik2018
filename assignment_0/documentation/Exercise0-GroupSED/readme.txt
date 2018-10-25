Exercise0-GroupSED

How we solved the exercise:
We downloaded the files "assignment_0.zip" and "assignment_0.pdf".
We extracted the zip-folder "assignment_0.zip" to the folder "assignment_0".
Then we opened the file "assignment_0/scenes/solid_color/solid_color.sce" and modified the second line to our favorite color.
We assumed that the color is encoded as RGB.

Then we followed the instructions in "assignment_0/readme.md":

We opened a cmd in the folder "assignment_0" and typed
 mkdir build
 cd build
 cmake ..
 make

Then we executed the program "raytrace" by opening a cmd in "assignment_0/build" and typing "./raytrace ../scenes/solid_color/solid_color.sce solid_color.tga"





Encountered Problems:

 - make didn't work, because it's not installed on the computer

 - Executing "raytrace" didn't work, because the project was compiled with visual studio (make didn't work),
   which created the executable "raytrace" in the subfolder "Debug"

 - Errormessage "abort() has been called". The cmd-command was:
    "assignment_0\build>    Debug/raytrace.exe ../../scenes/spheres/spheres.sce output.tga"

   Reason was the wrong path ../../scenes/spheres/spheres.sce, which should have been ../scenes/spheres/spheres.sce

 - output-file "solid_color.tga" was white instead of the favorite color.
   Reason was the encoding of the color, we assumed integer-values between 1 and 255 instead of float/double-values between 0 and 1.


Fraction of the total workload:

Sara: 33%
Elio: 33%
Dave: 33%
