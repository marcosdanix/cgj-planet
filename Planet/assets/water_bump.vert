#version 330 core

//Vertex shader inputs
in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Tangent;

//Outputs to the fragment shader
out vec3 m_Position; //model space
out vec3 e_Position; //eye space
out vec3 m_Normal;
out vec3 m_Tangent;

//Include at least these 4 uniforms
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;




void main(void)
{
	vec4 pos = View * Model * vec4(in_Position, 1.0);
	m_Position = vec3(in_Position);
	e_Position = vec3(pos);
	m_Normal = in_Normal;
	m_Tangent = in_Tangent;
	gl_Position = Projection * pos;
}