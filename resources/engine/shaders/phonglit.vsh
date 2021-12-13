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

out VS_OUT {
    vec3 Color;
    vec3 Normal;
    vec2 TexCoord;
    vec3 WorldPosition;
    Light lightOut;
} o;

layout (std140) uniform PV {
    mat4 p;
    mat4 v;
    mat4 pv;
};
uniform mat4 m;

uniform Light light;

void main() {
    gl_Position = pv * m * vec4(iPos, 1.0);
    o.Color = iColor;
    o.Normal = mat3(transpose(inverse(v * m))) * iNormal;
    o.TexCoord = iTexCoord;
    o.WorldPosition = vec3(v * m * vec4(iPos, 1.0));
    o.lightOut = light;
    o.lightOut.position = vec3(v * vec4(light.position, 1.0));
}
