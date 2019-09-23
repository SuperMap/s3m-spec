#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertextColor;

flat out vec4 regionColor;

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
	regionColor = vertextColor;
        gl_Position = proj * view * model * vec4(mp2dp(position), 1.0);
}
