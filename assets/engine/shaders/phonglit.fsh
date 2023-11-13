in VS_OUT {
    vec3 color;
    vec3 normal;
    vec2 texCoords;
    vec3 worldPosition;
    vec3 viewPosition;
    vec3 viewDirection;
    vec3 fragPosition;
} i;

out vec4 FragColor;

struct PhongMaterial {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float lambertFactor; // between 0 and 1
};
uniform PhongMaterial material;

#include shaders/ubo/lights.glsl#

vec3 addDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 addPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 addSpotLight(SpotLight light, vec3 normal, vec3 viewDir);


void main() {
    vec3 normal = normalize(i.normal);
    vec3 viewDir = i.viewPosition - i.fragPosition;

    vec3 result = vec3(0.0);
    for (int i = 0; i < numberOfLights.x; i++) {
        result += addDirectionalLight(directionalLights[i], normal, viewDir);
    }
    for (int i = 0; i < numberOfLights.y; i++) {
        result += addPointLight(pointLights[i], normal, viewDir);
    }
    for (int i = 0; i < numberOfLights.z; i++) {
        result += addSpotLight(spotLights[i], normal, viewDir);
    }
    FragColor = vec4(i.color * result, 1.0);
}

vec3 addDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction.xyz);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0) * material.lambertFactor + (1.0 - material.lambertFactor);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // combine results
    vec4 ambient  = light.ambient  * texture(material.diffuse, i.texCoords);
    vec4 diffuse  = light.diffuse  * diff * texture(material.diffuse, i.texCoords);
    vec4 specular = light.specular * spec * texture(material.specular, i.texCoords);
    return ambient.xyz + diffuse.xyz + specular.xyz;
}

vec3 addPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 distanceVec = vec3(light.position) - i.fragPosition;
    vec3 lightDir = normalize(distanceVec);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0) * material.lambertFactor + (1.0 - material.lambertFactor);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(distanceVec);
    float attenuation = max(1.0 / (light.falloff.x + (light.falloff.y * distance) + (light.falloff.z * (distance * distance))), 0.0);
    // combine results
    vec4 ambient  = light.ambient  * texture(material.diffuse, i.texCoords);
    vec4 diffuse  = light.diffuse  * diff * texture(material.diffuse, i.texCoords);
    vec4 specular = light.specular * spec * texture(material.specular, i.texCoords);
    return (ambient.xyz + diffuse.xyz + specular.xyz) * attenuation;
}

vec3 addSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
    vec3 distanceVec = light.position.xyz - i.fragPosition;
    vec3 lightDir = normalize(distanceVec);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0) * material.lambertFactor + (1.0 - material.lambertFactor);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(distanceVec);
    float attenuation = max(1.0 / (light.falloff.x + (light.falloff.y * distance) + (light.falloff.z * (distance * distance))), 0.0);
    // spotlight cutoff
    vec3 directionToLight = normalize(-light.direction.xyz);
    float angle = acos(dot(lightDir, directionToLight) / (length(lightDir) * length(directionToLight)));
    float intensity = max(light.cutoff.y - angle, 0.0) / (light.cutoff.y - light.cutoff.x);
    // combine results
    vec4 diffuse  = light.diffuse  * diff * texture(material.diffuse, i.texCoords);
    vec4 specular = light.specular * spec * texture(material.specular, i.texCoords);
    return (diffuse.xyz + specular.xyz) * attenuation * intensity;
}
