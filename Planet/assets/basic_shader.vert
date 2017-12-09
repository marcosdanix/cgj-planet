#version 330 core

//Vertex shader inputs
in vec3 in_Position;
//in vec3 in_Texcoord;
in vec3 in_Normal;

//Outputs to the fragment shader
//out vec3 ex_Position;
//out vec3 ex_Texcoord;
out vec3 ex_Normal;

//Include at least these 4 uniforms
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Normal; //convert to eye space

void main(void)
{
	ex_Normal = vec3(Normal * vec4(in_Normal, 0.0));
	gl_Position = Projection * View * Model * vec4(in_Position, 1.0);
}