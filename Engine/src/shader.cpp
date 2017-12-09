#include <fstream>
#include "shader.h"
#include "error.h"
#include <glm/gtc/type_ptr.hpp>

using namespace cgj;

/////////////////////////////////////////////////////////////////////// Shader

Shader::Shader() :	loaded_(false),	compiled_(false)
{
}

Shader::~Shader()
{
	glDeleteShader(id_);
}

void Shader::load(std::string filename)
{
	auto input = std::ifstream();
	input.exceptions(std::ifstream::failbit);
	try {
		input.open(filename);
		load(input);
		input.close();
	}
	catch (std::ifstream::failure e) {
		std::cerr << e.what() << std::endl;
		throw std::exception(e);
	}
}

void Shader::load(std::istream & stream)
{
	if (loaded_) throw std::exception("Already loaded the shader");

	id_ = createShader();

	std::string contents((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());

	string_ = contents;
	const GLchar* source_ = string_.c_str();

	glShaderSource(id_, 1, &source_, 0);
	PEEK_OPENGL_ERROR("Failed Generating Shader")

	loaded_ = true;
}

void Shader::compile(std::ostream & output)
{
	if (compiled_) return;

	glCompileShader(id_);

	//check status
	GLint status;
	glGetShaderiv(id_, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		char buffer[65536];
		GLsizei length;
		glGetShaderInfoLog(id_, 65536, &length, buffer);
		output.write(buffer, length);
		throw std::exception("Shader compile exception");
	}
	else {
		compiled_ = true;
	}
}

GLuint Shader::id()
{
	return id_;
}

GLuint VertexShader::createShader()
{
	return glCreateShader(GL_VERTEX_SHADER);
	PEEK_OPENGL_ERROR("Failed Creating Vertex Shader")
}

GLuint FragmentShader::createShader()
{
	return glCreateShader(GL_FRAGMENT_SHADER);
	PEEK_OPENGL_ERROR("Failed Creating Fragment Shader")
}

/////////////////////////////////////////////////////////////////////// ShaderProgram

ShaderProgram::ShaderProgram() : id_(0), linked_(false)
{
}

ShaderProgram::~ShaderProgram()
{
	glUseProgram(0);

	for (Shader* shader : shaders_) {
		glDetachShader(id_, shader->id());
	}

	glDeleteProgram(id_);
}

ShaderProgram & ShaderProgram::create()
{
	if (id_ != 0) return *this;

	id_ = glCreateProgram();
	PEEK_OPENGL_ERROR("Failed Creating Program")

	if (id_ == 0) {
		throw std::exception("Failed creating program");
	}

	return *this;
}

ShaderProgram & ShaderProgram::attach(Shader * shader)
{
	shader->compile();
	shaders_.push_back(shader);
	glAttachShader(id_, shader->id());
	PEEK_OPENGL_ERROR("Failed Attaching Shader")
	return *this;
}

ShaderProgram & ShaderProgram::bindAttribute(GLuint index, std::string attribute)
{
	glBindAttribLocation(id_, index, attribute.c_str());
	PEEK_OPENGL_ERROR("Failed Binding Attribute")
	return *this;
}

void ShaderProgram::link(std::ostream & output)
{
	glLinkProgram(id_);

	//check status
	GLint status;
	glGetProgramiv(id_, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		char buffer[65536];
		GLsizei length;
		glGetProgramInfoLog(id_, 65536, &length, buffer);
		output.write(buffer, length);
		throw std::exception("Program link exception");
		linked_ = false;
	}
	else {
		linked_ = true;
	}
}


GLint ShaderProgram::getUniform(std::string id)
{
	if (linked_) {
		const char* idstr = id.c_str();
		return glGetUniformLocation(id_, idstr);
	}
	else throw std::exception("getUniformLocation before linking successfuly the program");
}

void ShaderProgram::use()
{
	glUseProgram(id_);
}

void cgj::ShaderProgram::stop() //good idea prof
{
	glUseProgram(0);
}

void ShaderProgram::deleteShaders()
{
	for (Shader* shader : shaders_) {
		glDetachShader(id_, shader->id());
		delete shader;
	}
}

bool cgj::ShaderProgram::empty()
{
	return shaders_.empty();
}

ShaderProgram & cgj::ShaderProgram::uniform(std::string id, GLfloat item)
{
	GLint uniformId = getUniform(id);
	if (uniformId != -1) glUniform1f(uniformId, item);
	return *this;
}

ShaderProgram & cgj::ShaderProgram::uniform(std::string id, GLint item)
{
	GLint uniformId = getUniform(id);
	if (uniformId != -1) glUniform1i(uniformId, item);
	return *this;
}

ShaderProgram & cgj::ShaderProgram::uniform(std::string id, vec2 item)
{
	GLint uniformId = getUniform(id);
	if (uniformId != -1) glUniform2f(uniformId, item.x, item.y);
	return *this;
}

ShaderProgram & cgj::ShaderProgram::uniform(std::string id, vec3 item)
{
	GLint uniformId = getUniform(id);
	if (uniformId != -1) glUniform3f(uniformId, item.x, item.y, item.z);
	return *this;
}

ShaderProgram & cgj::ShaderProgram::uniform(std::string id, vec4 item)
{
	GLint uniformId = getUniform(id);
	if (uniformId != -1) glUniform4f(uniformId, item.x, item.y, item.z, item.w);
	return *this;
}

ShaderProgram & cgj::ShaderProgram::uniform(std::string id, mat2 item)
{
	GLint uniformId = getUniform(id);
	if (uniformId != -1) glUniformMatrix2fv(uniformId, 1, GL_FALSE, value_ptr(item));
	return *this;
}

ShaderProgram & cgj::ShaderProgram::uniform(std::string id, mat3 item)
{
	GLint uniformId = getUniform(id);
	if (uniformId != -1) glUniformMatrix3fv(uniformId, 1, GL_FALSE, value_ptr(item));
	return *this;
}

ShaderProgram & cgj::ShaderProgram::uniform(std::string id, mat4 item)
{
	GLint uniformId = getUniform(id);
	if (uniformId != -1) glUniformMatrix4fv(uniformId, 1, GL_FALSE, value_ptr(item));
	return *this;
}
