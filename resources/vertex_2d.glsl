#version 330 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_Center;
layout (location = 3) in vec2 a_Size;
layout (location = 4) in vec4 a_Radius;
layout (location = 5) in vec2 a_TextureCoords;
layout (location = 6) in float a_TextureIndex;
layout (location = 7) in float a_StrokeWidth;

flat out vec4 v_Color;
flat out vec2 v_Center;
flat out vec2 v_Size;
flat out vec4 v_Radius;
out vec2 v_TextureCoords;
flat out float v_TextureIndex;
flat out float v_StrokeWidth;

uniform vec2 u_WindowSize;

void main()
{
    vec2 p = a_Position;
    gl_Position = vec4(vec2(2.0 * p / u_WindowSize - 1.0), 0.0, 1.0);

    v_Color = a_Color;
    v_Center = a_Center;
    v_Size = a_Size;
    v_Radius = a_Radius;
    v_TextureCoords = a_TextureCoords;
    v_TextureIndex = a_TextureIndex;
    v_StrokeWidth = a_StrokeWidth;
}
