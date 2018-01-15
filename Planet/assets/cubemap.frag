#version 330 core

in vec3 ex_Position;

out vec4 out_Color;

void main(void) {
	vec3 pos = 0.5 * normalize(ex_Position) + vec3(0.5);
	out_Color = vec4(pos, 1);
	//out_Color = vec4(1);
}

