#pragma once
#include <GL/glew.h>
#include <glm/fwd.hpp>

using namespace glm;

namespace cgj {
	class Texture {
	public:
		virtual ~Texture() {}
		Texture() : id_(-1) {}
		GLint id() { return id_; }
	protected:
		GLuint id_;
	};

	//add more Texture classes later
	class TextureCubemap : public Texture {
	public:
		~TextureCubemap() { delete[] data; }
		//right now it assumes 8bit RGB data
		void create(GLubyte **data, GLsizei width, GLsizei height, GLsizei levels = 1);
	private:
		GLubyte **data;
	};

	class StarmapGenerator {
	public:
		//generates an array of 6 images
		GLubyte** generate(GLsizei resolution);	
	private:
		vec3 getColor(vec3 pos);

	};
}