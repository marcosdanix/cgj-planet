#version 330 core

//Fragment shader inputs
in vec3 m_Position;
in vec3 ex_Normal;

out vec4 out_Color;

void main(void)
{
	const float pi = 3.14159265359;
	const float maxHeight = 1.155;
	float height = (length(m_Position) - 1.0)/(maxHeight - 1.0); //the distance from the center
	//The sphere has one unit radius, so it has to subract one from the length and maximum height
	
	float latitude = 1.0 - acos(abs(normalize(m_Position).y)) / (0.5*pi); //Higher the latitude, higher the value
	//By doing the dot product with Y, which makes areas in the poles 1 and in the equator 0
	
	
	/** /
	//paint the mountains white
	if (height > 0.4) out_Color = vec4(vec3(height), 1);
	
	//paint the terrain red and it gets progressivly pink as latitude increases
	else out_Color = vec4((height+0.2)*vec3(1,0.5*latitude,latitude), 1);
	/**/
	
	/** /
	//simple height map
	out_Color = vec4(vec3(height), 1);
	/**/
	
	/**/
	//simple latitude map
	out_Color = vec4(vec3(latitude), 1);	
	/**/
	
	/** /
	//let's combine it for lulz
	out_Color = vec4(height, height*latitude, latitude, 1);
	/**/
}