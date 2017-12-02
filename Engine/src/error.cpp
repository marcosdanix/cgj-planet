#include <iostream>
#include "error.h"
#include <GL/glew.h>


void printPosition(unsigned int line, std::string file)
{
	std::cerr << file << ":" << line << "  ";
}

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}


void cgj::checkOpenglError(std::string message, unsigned int line, std::string file)
{
	if (isOpenGLError()) {
		printPosition(line, file);
		std::cerr << message << std::endl;
		exit(EXIT_FAILURE);
	}
}

void cgj::peekOpenglError(std::string message, unsigned int line, std::string file)
{
	if (isOpenGLError()) {
		printPosition(line, file);
		std::cerr << message << std::endl;
		//Do not exit the program
	}
}

void cgj::checkAssert(bool condition, std::string expression, unsigned int line, std::string file)
{
	if (!condition) {
		printPosition(line, file);
		std::cerr << "Assertion error: " << expression << std::endl;
		exit(EXIT_FAILURE);
	}
}

void cgj::peekAssert(bool condition, std::string expression, unsigned int line, std::string file)
{
	if (!condition) {
		printPosition(line, file);
		std::cerr << "Assertion error: " << expression << std::endl;
		//Do not exit the program
	}
}
