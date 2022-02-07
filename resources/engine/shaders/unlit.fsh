out vec4 FragColor;

in VS_OUT {
    vec3 Color;
    vec3 Normal;
    vec2 TexCoord;
} i;


void main() {
    FragColor = vec4(i.Color, 1.0);
}
