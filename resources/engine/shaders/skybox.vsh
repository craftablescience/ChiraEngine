layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iTexCoord;

out vec3 TexCoords;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

void main() {
    TexCoords = iPos;
    gl_Position = p * v * vec4(iPos, 1.0);
}
