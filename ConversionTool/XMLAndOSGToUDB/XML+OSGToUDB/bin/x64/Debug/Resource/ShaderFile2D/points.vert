#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertextColor;
layout (location = 2) in int symbolID_in;
layout (location = 3) in vec2 texCoord_in;
layout (location = 4) in float angle_in;
layout (location = 5) in float size;

out mat2 rotationMat;
flat out int symbolID;
flat out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec4 coorPara;

vec3 mp2dp(vec3 mPosition)
{
    float mapLeft = coorPara.x;
    float mapBottom = coorPara.y;
    float M2LRatio = coorPara.z;
    float L2DRatio = coorPara.w;

    mPosition.x = (mPosition.x - mapLeft) * M2LRatio * L2DRatio;
    mPosition.y = (mapBottom - mPosition.y) * M2LRatio * L2DRatio;

    return mPosition;
}

void main()
{
    // texture coord compute
    symbolID = symbolID_in;
    texCoord.xy = texCoord_in.xy;

    // compute rorarionMat
    float angle = radians(angle_in);
    float sin_t = sin(angle);
    float cos_t = cos(angle);
    //Counterclockwise
    rotationMat = mat2(cos_t, sin_t, -sin_t, cos_t);

    // Point Size
    gl_PointSize = size * coorPara.w * 10.0f;
    gl_Position = proj * view * model * vec4(mp2dp(position), 1.0);
}
