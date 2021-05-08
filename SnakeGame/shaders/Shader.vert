#version 330 core

layout(location = 0) in vec2 pos;

uniform mat4 mp;

void main()
{
	gl_Position = mp * vec4(pos, 0.0, 1.0);
}