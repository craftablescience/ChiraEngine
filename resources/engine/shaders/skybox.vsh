layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iTexCoord;

out vec3 TexCoords;

uniform mat4 p;
uniform mat4 v;

void main() {
    TexCoords = iPos;
    vec4 pos = p * v * vec4(iPos, 1.0);
    gl_Position = pos.xyww;
}
