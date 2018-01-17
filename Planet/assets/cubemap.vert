#version 330 core

in vec3 in_Position;

out vec3 ex_Position;

uniform mat4 View;
uniform mat4 Projection;


void main(void)
{
	vec3 pos =  mat3(View) * 2.6 * in_Position;
	ex_Position = in_Position;
	gl_Position = vec4(pos.xy, 1, 1);
}