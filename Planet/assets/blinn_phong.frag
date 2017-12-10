#version 330 core

//Fragment shader inputs
//in vec3 ex_Position;
//in vec3 ex_Texcoord;
in vec3 ex_Normal;
//in float ex_Diffuse;

//Fragment shader output
out vec4 out_Color;

const vec3 Light = normalize(vec3(1.0, 1.0, 1.0));
const vec4 Color = vec4(1,0,0,1);
const vec4 White = vec4(1);
const float Ambient = 0.1;
const vec3 Viewer = vec3(0,0,1);

vec2 blinnPhong(vec3 normal, vec3 light, float kD, float kS, float shininess)
{
	float diffuse = kD * max(0.0, dot(normal, light));
	vec3 H = normalize(light + Viewer);
	float specular = kS * pow(max(0.0, dot(normal, H)), shininess);
	return vec2(diffuse, specular);
}

void main()
{
	vec3 normal = normalize(ex_Normal);
	vec2 blinnPhong = blinnPhong(normal, Light, 1.0, 1.0, 32.0);
	vec2 backkPhong = blinnPhong(normal, -Light, 0.5, 0.5, 32.0);
	out_Color = Ambient * Color + (blinnPhong.x+backkPhong.x) * Color + (blinnPhong.y+backkPhong.y) * White;
}