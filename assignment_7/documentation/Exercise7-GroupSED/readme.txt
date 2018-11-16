Exercise5-GroupSED

How we solved the exercise:

 - We first implemented the methods "m_constructLightViewMatrix" and "m_constructLightProjectionMatrix"
 - Then, we implemented "shadowmap_gen.frag" to draw the distances and get a greyscale-shadowmap
   instead of a black-shadowmap
 - Then we tried to implement the BlendFunc with "glEnable(GL_BLEND);" and "glBlendFunc" but it didn't
   work or didn't change anything.
 - After that, we implemented the "phong_shadow.frag" (only diffuse for testing if it works)
 - It didn't change anything and we noticed that we also have to change the method "ShadowViewer::draw".


Problems we encountered:

 - The adding of the diffuse light didn't work, we got an error:
    GL error: invalid operation (not allowed in current state)

   We used the following code in "phong_shadow.frag":
    vec3 color = vec3(0.0f);
    vec3 vecVertexToLight = v2f_ec_vertex - light_position;
    if (length(vecVertexToLight) < 1.01 * texture(shadow_map, vecVertexToLight).r) {
        // Add diffuse light
    	color = color + (diffuse_color * normalize(vecVertexToLight));
    }

   Solution:
   The problem wasn't in "phong_shadow.frag", but in "ShadowViewer.cpp".
   We passed "view_matrix*this->m_light[li].position()" as light, but this is a vec4 and
   "phong_shadow.frag" needs a vec3.
   Interestingly, the "phong_shadow.frag" did work if we changed our Code in "phong_shadow.frag" to:
   (commented out one line)

    vec3 color = vec3(0.0f);
    vec3 vecVertexToLight = v2f_ec_vertex - light_position;
    if (length(vecVertexToLight) < 1.01 * texture(shadow_map, vecVertexToLight).r) {
        // Add diffuse light
    	// color = color + (diffuse_color * normalize(vecVertexToLight));
    }


 - The light on the plane was colored instead of white
   Solution:
    - We calculated for the diffuse light
      "color = color + (diffuse_color * normalize(vecVertexToLight));"
      instead of
      "color = color + (diffuse_color * dot(v2f_ec_vertex, normalize(vecVertexToLight)));"

      Bugs:
       - We didn't compute the dot-product
       - vecVertexToLight was computed as "v2f_ec_vertex - light_position" instead of
         "light_position - v2f_ec_vertex"


 - The light on the plane was too bright
   Solution:
    - There was another bug in the diffuse light:
       "color = color + (diffuse_color * dot(v2f_ec_vertex, normalize(vecVertexToLight)));"
      instead of
       "color = color + (diffuse_color * dot(N, normalize(vecVertexToLight)));"




Fraction of the total workload:

Sara Kraettli: 33%
Elio Fritschi: 33%
Dave Meier: 33%
