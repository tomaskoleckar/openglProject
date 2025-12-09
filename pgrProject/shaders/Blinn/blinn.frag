#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform int uObjectID;
uniform int uSelectedID;
uniform int  uShadingMode;

uniform vec3 uColor;

struct Material {
    vec3 ra;
    vec3 rd;
    vec3 rs;
    float h;
};
uniform Material material;

struct Light {
    int type;
    vec3 pos;
    vec3 dir;
    vec3 color;
    float intensity;
    float kc;
    float kl;
    float kq;
    float cutOff;
    float outerCutOff;
};

const int MAX_LIGHTS = 50;
uniform int   uLightCount;
uniform Light uLights[MAX_LIGHTS];

uniform vec3 uViewPos;
uniform sampler2D uTexture;
uniform int uHasTexture;

vec3 computeBlinn(Light L, vec3 N, vec3 V, vec3 baseColor)
{
    if (L.type == 0)
        return material.ra * L.color * L.intensity * baseColor;

    vec3 Ldir = (L.type == 1)
        ? normalize(-L.dir)
        : normalize(L.pos - FragPos);

    float diff = max(dot(N, Ldir), 0.0);

    vec3 H = normalize(V + Ldir);
    float spec = pow(max(dot(N, H), 0.0), material.h);

    float attenuation = 1.0;
    if (L.type >= 2) {
        float dist = length(L.pos - FragPos);
        attenuation = 1.0 / (L.kc + L.kl * dist + L.kq * dist * dist);
    }

    float spotFactor = 1.0;
    if (L.type == 3) {
        float theta = dot(Ldir, normalize(-L.dir));
        float eps = L.cutOff - L.outerCutOff;
        spotFactor = clamp((theta - L.outerCutOff) / eps, 0.0, 1.0);
    }

    vec3 amb = material.ra * L.color * L.intensity * baseColor;
    vec3 dif = material.rd * diff * L.color * L.intensity * baseColor;
    vec3 spe = material.rs * spec * L.color * L.intensity;

    return (amb + dif + spe) * attenuation * spotFactor;
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(uViewPos - FragPos);
    vec3 baseColor = uHasTexture == 1 ? texture(uTexture, TexCoord).rgb : uColor;

    vec3 result = vec3(0.0);
    for (int i=0; i<uLightCount; i++)
        result += computeBlinn(uLights[i], N, V, baseColor);

    if (uObjectID == uSelectedID && uSelectedID > 0) {
        result += vec3(0.2, 0.8, 0.2);
    }

    FragColor = vec4(result / (result + vec3(1.0)), 1.0);
}
