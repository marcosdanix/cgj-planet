#version 330 core

//Fragment shader inputs
in vec3 ex_Position;
//in vec3 ex_Texcoord;
in vec3 ex_Normal;
//in float ex_Diffuse;
in vec3 ex_Tangent;

//Fragment shader output
out vec4 out_Color;

const vec3 Light = normalize(vec3(1, 1, 1));
const vec4 Color = vec4(0,0.31,0.9,1);
//const vec4 White = vec4(0.5,0.5,0,1);
const vec4 White = vec4(1);

const float Ambient = 0.1;

const float w = 0.21;

vec2 blinnPhong(vec3 normal, vec3 light, float kD, float kS, float shininess)
{
	vec3 v = normalize(-ex_Position);
	float diffuse = kD * max(0.0, dot(normal, light));
	vec3 H = normalize(light + v);
	float specular = kS * pow(max(0.0, dot(normal, H)), shininess);
	return vec2(diffuse, specular);
}

float OrenNayarDir(vec3 lightDir, vec3 normal, float exponent)
{
  vec3 viewDir = normalize(-ex_Position);
  float LdotN = dot(lightDir,normal);
  float VdotN = dot(viewDir,normal);
  float result = clamp( LdotN, 0. , 1.);
  float soft_rim = clamp( 1. - VdotN/2., 0. , 1.);
  float fakey = pow(1. - result * soft_rim , 2.);
  float fakey_magic = 0.62;
  fakey = fakey_magic - fakey*fakey_magic;
  float K =  mix(result, fakey, exponent) ;
  return K;
}

void main()
{
	vec3 normal = normalize(ex_Normal);
	vec2 blinnPhong = blinnPhong(normal, Light, 1.0, 1.0, 72.0);
	//vec2 backkPhong = blinnPhong(normal, -Light, 0.5, 0.5, 32.0);
	//out_Color = Ambient * Color + (blinnPhong.x+backkPhong.x) * Color + (blinnPhong.y+backkPhong.y) * White;
	//out_Color = Ambient * Color + blinnPhong.x * Color + blinnPhong.y * White;
	out_Color = Ambient * Color + OrenNayarDir(Light, normal, 0.5) * (Color+vec4(w,w,w,0)) + blinnPhong.y * White;
}