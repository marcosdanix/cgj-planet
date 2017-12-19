#version 330 core

//Vertex shader inputs
in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Tangent;

//Outputs to the fragment shader
out vec3 m_Position;
out vec3 ex_Position;
out vec3 m_Normal;
out mat3 ex_NormalMatrix;
out vec3 ex_Tangent;

//Include at least these 4 uniforms
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 Normal; //convert to eye space



void main(void)
{
	vec4 pos = View * Model * vec4(in_Position, 1.0);
	m_Position = vec3(in_Position);
	ex_Position = vec3(pos);
	m_Normal = in_Normal;
	ex_NormalMatrix = Normal;
	ex_Tangent = in_Tangent;
	gl_Position = Projection * pos;
}