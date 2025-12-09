#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 out_Color;

uniform vec3 uLightPos = vec3(0.0, 0.0, 0.0);

uniform vec3 uCameraPos;

void main(void)
{
    vec3 N = normalize(worldNormal);
    vec3 L = normalize(uLightPos - worldPosition);
    vec3 V = normalize(uCameraPos - worldPosition);

    float diff = max(dot(N, L), 0.0);

    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), 24.0);

    float glow = pow(diff, 6.0);

    vec3 baseColor    = vec3(0.05);
    vec3 diffuseColor = vec3(0.25);
    vec3 glowColor    = vec3(0.4, 0.7, 1.0);

    vec3 ambient  = baseColor * 0.4;
    vec3 diffuse  = diffuseColor * diff;
    vec3 highlight = glowColor * (glow * 1.8 + spec * 1.2);

    vec3 color = ambient + diffuse + highlight;

    color = pow(color, vec3(1.0 / 2.2));

    out_Color = vec4(color, 1.0);
}
