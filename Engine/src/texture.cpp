#include "texture.h"
#include "util.h"
#include "error.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

//https://www.khronos.org/opengl/wiki/Cubemap_Texture
//+X -X +Y -Y +Z -Z
const static vec3 fixed[] = {
	vec3(1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0), vec3(0.0,-1.0, 0.0),
	vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0,-1.0)
};
const static vec3 u[] = {
	vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, -1.0),
	vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0),
	vec3(-1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0)
};
const static vec3 v[] = {
	vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, -1.0),
	vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0)
};

GLubyte ** cgj::StarmapGenerator::generate(GLsizei resolution)
{
	GLubyte** result = new GLubyte*[6];
	int rr = resolution * resolution;

	for (int i = 0; i < 6; ++i) {
		result[i] = new GLubyte[3*rr];
		
		for (int j = 0; j < 3*rr; j += 3 ) {
			int ix = (j/3) % resolution;
			int iy = (j/3) / resolution;
			float x = 2.0 * (float(ix) / float(resolution)) - 1.0;
			float y = -2.0 * (float(iy) / float(resolution)) + 1.0;
			vec3 pos = fixed[i] + x*u[i] + y*v[i];
			vec3 color = getColor(pos);
			result[i][j]   = (GLubyte)(color.x * 256.0);
			result[i][j+1] = (GLubyte)(color.y * 256.0);
			result[i][j+2] = (GLubyte)(color.z * 256.0);
		}
	}

	return result;

}

vec3 cgj::StarmapGenerator::getColor(vec3 pos)
{
	const static vec3 ADD = vec3(7, 13, 19);
	const static vec3 ADD2 = vec3(23, -7, 113);
	pos = normalize(pos);
	
	float noise = perlin(pos, 64.0, 4, 1.5);
	float noise2 = 0.5 * perlin(pos, 10.0, 8, 2.0, ADD) + 0.5;
	float noise3 = 0.5 * perlin(pos, 10.0, 8, 2.0, ADD2) + 0.5;

	float yy = pos.x*(1.0 - sqrt(abs(pos.y)));
	noise = noise > (0.4 - 0.1*yy) ? noise : 0.0;

	return vec3(noise + yy * 0.5f * (noise2 - 0.25f * noise3));
}

void cgj::TextureCubemap::create(GLubyte ** data, GLsizei width, GLsizei height, GLsizei levels)
{
	//Most of this code was copied from the OpenGL programming guide 8th edition

	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, levels, GL_RGB8, width, height);

	for (int face = 0; face < 6; ++face) {
		GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
		glTexSubImage2D(target, // Face
			0,					// Level
			0, 0,				// X, Y offset
			width, height,		// Size of face
			GL_RGB,				// Format
			GL_UNSIGNED_BYTE,	// Type
			data[face]);		// Data
	}
	PEEK_OPENGL_ERROR("Failed creating texture")
}
