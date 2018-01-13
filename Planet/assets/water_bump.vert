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
out vec3 Light;

//Include at least these 4 uniforms
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 Normal;

const vec3 light = normalize(vec3(1,0,0));
const vec3 light_pos = vec3(0,0,100);


void main(void)
{
	vec4 pos = View * Model * vec4(in_Position, 1.0);
	m_Position = vec3(in_Position);
	e_Position = vec3(pos);
	m_Normal = in_Normal;
	m_Tangent = in_Tangent;
	Light = normalize(vec3(View * vec4(light,0)));
	gl_Position = Projection * pos;
}