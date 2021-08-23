struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float lambertFactor; // between 0 and 1
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

in VS_OUT {
    vec3 Color;
    vec3 Normal;
    vec2 TexCoord;
    vec3 WorldPosition;
    Light lightOut;
} i;

out vec4 FragColor;

uniform Material material;


void main() {
    vec4 ambientResult = vec4(i.lightOut.ambient, 1.0) * texture(material.diffuse, i.TexCoord);

    vec3 normal = normalize(i.Normal);
    vec3 lightDirection = normalize(i.lightOut.position - i.WorldPosition);
    float diff = max(dot(normal, lightDirection), 0.0) * material.lambertFactor + (1.0 - material.lambertFactor);
    vec4 diffuseResult = vec4(i.lightOut.diffuse, 1.0) * (diff * texture(material.diffuse, i.TexCoord));

    vec3 viewDirection = normalize(-i.WorldPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec4 specular = vec4(i.lightOut.specular, 1.0) * (spec * texture(material.specular, i.TexCoord));

    FragColor = vec4(i.Color, 1.0) * (ambientResult + diffuseResult + specular);
}
