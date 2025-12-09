#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 vTexDir;

uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    mat4 viewNoTranslate = mat4(mat3(uView));

    vec4 pos = uProj * viewNoTranslate * vec4(aPos, 1.0);
    gl_Position = pos.xyww;  // force depth = 1.0

    vTexDir = aPos;
}
