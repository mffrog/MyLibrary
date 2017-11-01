
#version 410 
layout(location=0) in vec3 vPosition;
layout(location=1) in vec4 vColor;
layout(location=2) in vec2 vTexCoord;
layout(location=3) in vec3 vNormal;

layout(location=0) out vec4 inColor;
layout(location=1) out vec2 inTexCoord;
layout(location=2) out vec3 inWorldPosition;
layout(location=3) out vec3 inWorldNormal;

/**
 * 頂点シェーダのパラメータ
 */
layout (std140) uniform VertexData{
    mat4 matMVP;
    vec4 matPos;
} vertexData;

void main() {
    inColor = vColor;
    inTexCoord = vTexCoord;
    inWorldPosition = vPosition + vertexData.matPos.xyz;
    inWorldNormal = vNormal;
    gl_Position = vertexData.matMVP * vec4(vPosition + vertexData.matPos.xyz,1.0);
}
