#version 330 core

//Fragment shader inputs
//in vec3 ex_Position;
//in vec3 ex_Texcoord;
in vec3 ex_Normal;

//Fragment shader output
out vec4 out_Color;

void main()
{
	vec3 normal = normalize(ex_Normal);
	normal += 1.0;
	normal /= 2.0;
	out_Color = vec4(normal, 1);
}