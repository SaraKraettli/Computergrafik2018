Exercise5-GroupSED

How we solved the exercise:

 - We first implemented the billboard
 - Then we tried to implement the phong shading



Problems we encountered:
   
s
 - We didn't have enough knowledge to implement the phong-shading.
   We got stuck with the result in image "bild1.png".
   Calculations:

    - Normal-Matrix:    mat3(transpose(inverse(m_matrix)))    //solar_viewer.cpp
    - light_position:   mvp_matrix*sun_.pos_                  //solar_viewer.cpp
    - v2f_light:        vec3(light_position)                  //phong.vert
    - v2f_normal:       normal_matrix*v_normal                //phong.vert

    - Adding ambient:   color += (0.2f * sunlight * texture(tex, v2f_texcoord.st).rgb)                        //phong.frag
    - Adding diffuse:   color += sunlight * texture(tex, v2f_texcoord.st).rgb * dot(v2f_normal, v2f_light);   //phong.frag


   Solution:
   We asked another team for little help and changed
   the following 3 varibles according to their implementation:
    - light_position:   light
    - v2f_normal:       normalize(normal_matrix*v_normal)
    - v2f_light:        normalize(vec3(light_position - (modelview_matrix * v_position)))

   After that, the code didn't work yet, but we could fix it by changing the Normal-Matrix:
    - Normal-Matrix:    mat3(transpose(inverse(mv_matrix)))

  Note: Now, we have ambient-light and diffuse-light. Specular-light isn't implemented yet.
        The result is shown in "bild2.png"



 - 



Fraction of the total workload:

Sara Kraettli: 33%
Elio Fritschi: 33%
Dave Meier: 33%
