layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec3 iColor;
layout (location = 3) in vec2 iTexCoords;

out VS_OUT {
    vec3 color;
    vec3 normal;
    vec2 texCoords;
    vec3 worldPosition;
    vec3 viewPosition;
    vec3 viewDirection;
    vec3 fragPosition;
} o;

#include shaders/ubo/pv.glsl#
#include shaders/uniform/m.glsl#


void main() {
    o.color = iColor;
    o.normal = mat3(transpose(inverse(m))) * iNormal;
    o.texCoords = iTexCoords;
    o.worldPosition = vec3(v * m * vec4(iPos, 1.0));
    o.viewPosition = viewPosition.xyz;
    o.viewDirection = normalize(viewLookDirection.xyz);
    o.fragPosition = vec3(m * vec4(iPos, 1.0));
    gl_Position = pv * vec4(o.fragPosition, 1.0);
}
