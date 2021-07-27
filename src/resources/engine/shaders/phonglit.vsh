#version 330 core
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iTexCoord;

out vec3 oColor;
out vec3 oNormal;
out vec2 oTexCoord;
out vec3 oWorldPos;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    gl_Position = p * v * m * vec4(iPos, 1.0);
    oColor = iColor;
    oNormal = mat3(transpose(inverse(m))) * iNormal;
    oTexCoord = iTexCoord;
    oWorldPos = vec3(m * vec4(iPos, 1.0));
}
