#pragma once
#include <string>

#ifdef CGJ_DEBUG
//Assert OpenGL errors
#define ASSERT_OPENGL_ERROR(message) checkOpenglError((message), __LINE__, __FILE__);
#define PEEK_OPENGL_ERROR(message) peekOpenglError((message), __LINE__, __FILE__);
//There is an assert function in <cassert>, but the engine uses NDEBUG to include freeglut
//A hack, but it works
#define ASSERT_CONDITION(condition) checkAssert((condition), #condition, __LINE__, __FILE__);
#define PEEK_CONDITION(condition) peekAssert((condition), #condition, __LINE__, __FILE__);

#else 
#define ASSERT_OPENGL_ERROR(message) 
#define PEEK_OPENGL_ERROR(message) 
#define ASSERT_CONDITION(condition) 
#define PEEK_CONDITION(condition)
#endif

namespace cgj {
	void checkOpenglError(std::string message, unsigned int line, std::string file);
	void peekOpenglError(std::string message, unsigned int line, std::string file);
	void checkAssert(bool condition, std::string expression, unsigned int line, std::string file);
	void peekAssert(bool condition, std::string expression, unsigned int line, std::string file);
}