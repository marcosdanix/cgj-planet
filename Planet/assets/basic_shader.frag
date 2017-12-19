#version 330 core

//Fragment shader inputs
//in vec3 ex_Position;
//in vec3 ex_Texcoord;
//in vec3 ex_Normal;
in vec3 ex_Tangent;

//Fragment shader output
out vec4 out_Color;

void main()
{
	//vec3 normal = 0.5 * (1.0 + normalize(ex_Normal));
	
	//it's the tangent but I don't feel changing the name
	vec3 normal = 0.5 * (1.0 + normalize(ex_Tangent)); 
	out_Color = vec4(normal, 1);
}