struct DirectionalLight {
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
#define DIRECTIONAL_LIGHT_COUNT #DIRECTIONAL_LIGHT_COUNT#

struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 falloff; // x is constant, y is linear, z is quadratic
};
#define POINT_LIGHT_COUNT #POINT_LIGHT_COUNT#

struct SpotLight {
    vec4 position;
    vec4 direction;
    vec4 diffuse;
    vec4 specular;
    vec4 falloff; // x is constant, y is linear, z is quadratic
    vec4 cutoff; // x is cutoff inner angle, y is cutoff outer angle
};
#define SPOT_LIGHT_COUNT #SPOT_LIGHT_COUNT#

layout (std140) uniform LIGHTS {
    DirectionalLight directionalLights[DIRECTIONAL_LIGHT_COUNT];
    PointLight pointLights[POINT_LIGHT_COUNT];
    SpotLight spotLights[SPOT_LIGHT_COUNT];
    vec4 numberOfLights; // x is directional lights, y is point lights, z is spot lights
};
