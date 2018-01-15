#version 330 core

in vec3 in_Position;

out vec3 ex_Position;

uniform mat3 Normal;

void main(void)
{
	vec3 pos =  Normal * in_Position;
	//vec3 pos = normalize(in_Position);
	ex_Position = pos;
	gl_Position = vec4(pos.xy, 0.999, 1);
}