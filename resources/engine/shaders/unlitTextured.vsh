layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iTexCoord;

out VS_OUT {
   vec3 Color;
   vec3 Normal;
   vec2 TexCoord;
} o;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main() {
   gl_Position = p * v * m * vec4(iPos, 1.0);
   o.Color = iColor;
   o.Normal = iNormal;
   o.TexCoord = iTexCoord;
}
