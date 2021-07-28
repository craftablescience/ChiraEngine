#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

in vec3 oColor;
in vec3 oNormal;
in vec2 oTexCoord;
in vec3 oWorldPosition;
in Light oLight;

out vec4 FragColor;

uniform Material material;


void main() {
    vec4 ambientResult = vec4(oLight.ambient, 1.0) * texture(material.diffuse, oTexCoord);

    vec3 normal = normalize(oNormal);
    vec3 lightDirection = normalize(oLight.position - oWorldPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec4 diffuseResult = vec4(oLight.diffuse, 1.0) * (diff * texture(material.diffuse, oTexCoord));

    vec3 viewDirection = normalize(-oWorldPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec4 specular = vec4(oLight.specular, 1.0) * (spec * texture(material.specular, oTexCoord));

    FragColor = vec4(oColor, 1.0) * (ambientResult + diffuseResult + specular);
}
