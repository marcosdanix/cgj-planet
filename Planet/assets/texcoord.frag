#version 330 core

in vec3 ex_Position;

out vec4 out_Color;

uniform samplerCube Texture;

void main()
{
	//out_Color = texture(Texture, ex_Position);
	out_Color = vec4(ex_Position,1);
}