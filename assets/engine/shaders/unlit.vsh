layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iTexCoord;

out VS_OUT {
   vec3 Color;
   vec3 Normal;
   vec2 TexCoord;
} o;

#include shaders/ubo/pv.glsl#
#include shaders/uniform/m.glsl#


void main() {
   gl_Position = pv * m * vec4(iPos, 1.0);
   o.Color = iColor;
   o.Normal = iNormal;
   o.TexCoord = iTexCoord;
}
