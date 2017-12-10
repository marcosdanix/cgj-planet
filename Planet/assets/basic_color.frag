#version 330 core

//Fragment shader inputs
//in vec3 ex_Position;
//in vec3 ex_Texcoord;
//in vec3 ex_Normal;
in float ex_Diffuse;

//Fragment shader output
out vec4 out_Color;

const vec4 Color = vec4(1,0,1,1);
const float Ambient = 0.1;

void main()
{
	out_Color = Ambient*Color + ex_Diffuse*Color;
}