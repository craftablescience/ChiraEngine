#version 330 core
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iTexCoord;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

out vec3 oColor;
out vec3 oNormal;
out vec2 oTexCoord;
out vec3 oWorldPosition;
out Light oLight;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

uniform Light light;


void main() {
    gl_Position = p * v * m * vec4(iPos, 1.0);
    oColor = iColor;
    oNormal = mat3(transpose(inverse(v * m))) * iNormal;
    oTexCoord = iTexCoord;
    oWorldPosition = vec3(v * m * vec4(iPos, 1.0));
    oLight = light;
    oLight.position = vec3(v * vec4(light.position, 1.0));
}
