struct DirectionalLight {
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
#define DIRECTIONAL_LIGHT_MAX #DIRECTIONAL_LIGHT_MAX#

struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 falloff; // x is constant, y is linear, z is quadratic
};
#define POINT_LIGHT_MAX #POINT_LIGHT_MAX#

struct SpotLight {
    vec4 position;
    vec4 direction;
    vec4 diffuse;
    vec4 specular;
    vec4 falloff; // x is constant, y is linear, z is quadratic
    vec4 cutoff; // x is cutoff inner angle, y is cutoff outer angle
};
#define SPOT_LIGHT_MAX #SPOT_LIGHT_MAX#

layout (std140) uniform LIGHTS {
    DirectionalLight directionalLights[DIRECTIONAL_LIGHT_MAX];
    PointLight pointLights[POINT_LIGHT_MAX];
    SpotLight spotLights[SPOT_LIGHT_MAX];
    vec4 numberOfLights; // x is directional lights, y is point lights, z is spot lights
};
