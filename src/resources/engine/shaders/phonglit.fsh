#version 330 core
out vec4 FragColor;

in vec3 oColor;
in vec3 oNormal;
in vec2 oTexCoord;
in vec3 oWorldPosition;
in vec3 oLightPosition;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform sampler2D texture0;
uniform vec3 lightColor;
uniform Material material;


void main() {
    vec3 ambient = material.ambient * lightColor;
    vec4 ambientResult = vec4(ambient, 1.0) * texture(texture0, oTexCoord);

    vec3 normal = normalize(oNormal);
    vec3 lightDirection = normalize(oLightPosition - oWorldPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec4 diffuseResult = vec4(lightColor * (diff * material.diffuse), 1.0);

    float specularStrength = 0.4;
    vec3 viewDirection = normalize(-oWorldPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec4 specular = vec4(lightColor * (spec * material.specular), 1.0);

    FragColor = vec4(oColor, 1.0) * (ambientResult + diffuseResult + specular);
}
