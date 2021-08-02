#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 Color;
    vec3 Normal;
    vec2 TexCoord;
} i;

uniform sampler2D texture0;
uniform vec3 viewerPosition;

void main()
{
    FragColor = texture(texture0, i.TexCoord) * vec4(i.Color, 1.0);
}
