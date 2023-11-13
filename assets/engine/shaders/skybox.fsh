out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube cubemap;


void main() {
    FragColor = texture(cubemap, TexCoords);
}
