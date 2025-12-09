#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D uTexture;
uniform int uHasTexture;

void main()
{
    vec3 base = (uHasTexture == 1) ? texture(uTexture, TexCoord).rgb
                                   : uColor;

    FragColor = vec4(base, 1.0);
}
