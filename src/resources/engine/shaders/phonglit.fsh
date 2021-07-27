#version 330 core
out vec4 FragColor;

in vec3 oColor;
in vec3 oNormal;
in vec2 oTexCoord;
in vec3 oWorldPos;

uniform sampler2D texture0;
uniform vec3 viewerPosition;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    vec4 ambientResult = vec4(ambient, 1.0) * texture(texture0, oTexCoord);

    vec3 normal = normalize(oNormal);
    vec3 lightDirection = normalize(lightPosition - oWorldPos);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec4 diffuseResult = vec4(diff * lightColor, 1.0);

    float specularStrength = 0.4;
    vec3 viewDirection = normalize(viewerPosition - oWorldPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec4 specular = vec4(specularStrength * spec * lightColor, 1.0);

    FragColor = vec4(oColor, 1.0) * (ambientResult + diffuseResult + specular);
}
