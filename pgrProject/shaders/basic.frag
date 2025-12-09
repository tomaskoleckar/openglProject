#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 uColor;

struct Material {
    vec3 ra;
    vec3 rd;
    vec3 rs;
    float h;
};

uniform Material material;

struct Light {
    int   type;        // 0=AMBIENT, 1=DIRECTIONAL, 2=POINT, 3=SPOT
    vec3  pos;         // for POINT, SPOT
    vec3  dir;         // for DIRECTIONAL, SPOT
    vec3  color;
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
uniform int  uShadingMode; // 0=Constant, 1=Lambert, 2=Phong, 3=Blinn
uniform int  uIsSun;
uniform int  uIsFirefly;
uniform int  uBugMode;

uniform int uSelectedID;
uniform int uObjectID;

uniform sampler2D uTexture;
uniform int uHasTexture;

vec3 computeLight(Light L, vec3 N, vec3 V, vec3 baseColor)
{
    if (L.type == 0) {
        return material.ra * L.color * L.intensity * baseColor;
    }

    vec3 Ldir = (L.type == 1)
        ? normalize(-L.dir)
        : normalize(L.pos - FragPos);

    float diff = max(dot(N, Ldir), 0.0);
    float spec = 0.0;

    if (diff > 0.0) {
        if (uShadingMode == 2) { // Phong
            vec3 R = reflect(-Ldir, N);
            spec = pow(max(dot(V, R), 0.0), material.h);
        } else if (uShadingMode == 3) { // Blinn
            vec3 H = normalize(V + Ldir);
            spec = pow(max(dot(N, H), 0.0), material.h);
        }
    }

    float attenuation = 1.0;
    if (L.type == 2 || L.type == 3) {
        float dist = length(L.pos - FragPos);
        attenuation = 1.0 / (L.kc + L.kl * dist + L.kq * dist * dist);
    }

    float spotFactor = 1.0;
    if (L.type == 3) {
        float theta = dot(Ldir, normalize(-L.dir));
        float epsilon = L.cutOff - L.outerCutOff;
        spotFactor = clamp((theta - L.outerCutOff) / epsilon, 0.0, 1.0);
    }

    vec3 ambient  = material.ra * L.color * L.intensity * baseColor;
    vec3 diffuse  = material.rd * diff * L.color * L.intensity * baseColor;
    vec3 specular = material.rs * spec * L.color * L.intensity;

    return (ambient + diffuse + specular) * attenuation * spotFactor;
}

void main()
{
    if (uIsFirefly == 1) {
        FragColor = vec4(1.0, 1.0, 0.8, 1.0);
        return;
    }

    vec3 N = normalize(Normal);
    if (uBugMode == 1) N = -N;

    vec3 V = normalize(uViewPos - FragPos);

    vec3 baseColor = uColor;
    if (uHasTexture == 1) {
        baseColor = texture(uTexture, TexCoord).rgb;
    }

    if (uShadingMode == 0) {
        FragColor = vec4(baseColor, 1.0);
        return;
    }

    vec3 finalColor = vec3(0.0);
    for (int i = 0; i < uLightCount; ++i) {
        finalColor += computeLight(uLights[i], N, V, baseColor);
    }

    vec3 mapped = finalColor / (finalColor + vec3(1.0));

    if (uObjectID == uSelectedID && uSelectedID > 0) {
        mapped += vec3(0.2, 0.8, 0.2);
    }

    FragColor = vec4(mapped, 1.0);
}
