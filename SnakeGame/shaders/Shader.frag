#version 330 core

out vec4 colour;

uniform vec3 col;

void main()
{
	colour = vec4(col, 1.0);
}
