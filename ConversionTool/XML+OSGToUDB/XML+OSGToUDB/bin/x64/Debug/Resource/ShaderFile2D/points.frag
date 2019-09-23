#version 330 core
#extension GL_EXT_texture_array : enable
in mat2 rotationMat;
flat in int symbolID;
flat in vec2 texCoord;

out vec4 color;

uniform sampler2DArray ourTexture;
void main()
{

    vec2 centerFix = vec2(texCoord.s/2, texCoord.t/2);

    vec2 pointCoord = gl_PointCoord * texCoord - centerFix;
    pointCoord = rotationMat * pointCoord + centerFix;
    vec3 pt = vec3(pointCoord, symbolID);

    color = texture2DArray(ourTexture, pt);
}
