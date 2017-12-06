#pragma once
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>



//Shader and ShaderProgram related code
using namespace glm;

namespace cgj {
	
	class Shader {
	public:
		Shader();
		~Shader();

		void load(std::string filename);
		void load(std::istream &stream);
		void compile(std::ostream &output = std::cerr);
		GLuint id();

	protected:
		virtual GLuint createShader() = 0;
	private:
		bool loaded_;
		bool compiled_;
		GLuint id_;
		std::string string_;
	};

	class VertexShader : public Shader {
	protected:
		GLuint createShader();
	};

	class FragmentShader : public Shader {
	protected:
		GLuint createShader();
	};
	
	class ShaderProgram {
	public:
		ShaderProgram();
		~ShaderProgram();
		ShaderProgram& create();
		ShaderProgram& attach(Shader* shader);
		ShaderProgram& bindAttribute(GLuint index, std::string attribute);
		void link(std::ostream &output = std::cerr);
		GLint getUniform(std::string id);
		void use();
		void stop();
		void deleteShaders();
		
		//if ya need anymore uniform types, add some here
		//Like array uniforms for example
		ShaderProgram& uniform(std::string id, GLfloat item);
		ShaderProgram& uniform(std::string id, GLint item);	
		ShaderProgram& uniform(std::string id, vec2 item);	
		ShaderProgram& uniform(std::string id, vec3 item);
		ShaderProgram& uniform(std::string id, vec4 item);	
		ShaderProgram& uniform(std::string id, mat2 item);
		ShaderProgram& uniform(std::string id, mat3 item);
		ShaderProgram& uniform(std::string id, mat4 item);


	private:
		GLuint id_;
		bool linked_;
		std::vector<Shader*> shaders_;
	};
}