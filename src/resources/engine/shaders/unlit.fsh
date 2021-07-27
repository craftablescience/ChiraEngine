#version 330 core
out vec4 FragColor;

in vec3 oColor;
in vec3 oNormal;
in vec2 oTexCoord;

uniform sampler2D texture0;
uniform vec3 viewerPosition;

void main()
{
    FragColor = texture(texture0, oTexCoord) * vec4(oColor, 1.0);
}
