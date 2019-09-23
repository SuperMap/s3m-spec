#version 330 core
flat in vec4 regionColor;

out vec4 color;

void main()
{
    color = regionColor;
}