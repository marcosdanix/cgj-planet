#version 330 core

//Vertex shader inputs
in vec3 in_Position;
//in vec3 in_Texcoord;
in vec3 in_Normal;

//Outputs to the fragment shader
out vec3 ex_Position;
//out vec3 ex_Texcoord;
out vec3 ex_Normal;

//Include at least these 4 uniforms
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 Normal; //convert to eye space



void main(void)
{
	//ex_Normal = vec3(Normal * vec4(in_Normal, 0.0));
	vec4 pos = View * Model * vec4(in_Position, 1.0);
	ex_Position = vec3(pos);
	ex_Normal = Normal * in_Normal;
	gl_Position = Projection * pos;
}
