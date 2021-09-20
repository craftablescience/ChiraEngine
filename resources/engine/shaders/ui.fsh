out vec4 FragColor;

in VS_OUT {
    vec3 Color;
    vec3 Normal;
    vec2 TexCoord;
} i;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, i.TexCoord);
}
