#version 420 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D containerTexture;

void main()
{
    FragColor = texture(containerTexture, TexCoord);
}