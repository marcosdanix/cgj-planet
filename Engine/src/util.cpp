#include "util.h"
#include <glm/mat4x4.hpp>


/////////////////////////////////////////////////////////////////////// MatrixStack

void MatrixStack::push(mat4 matrix)
{
	if (!stack_.empty())
	{
		mat4 result;
		
		if (!results_.empty())
			result = results_.top();
		else
			result = mat4(1.0f);

		results_.push(result * stack_.top());
	}

	stack_.push(matrix);
}

void MatrixStack::pop()
{
	stack_.pop();
	if (results_.size() > 0) results_.pop();
}

mat4 MatrixStack::result()
{
	if (stack_.empty())
		return mat4(1.0f);

	if (results_.empty())
		return stack_.top();
	else
		return results_.top() * stack_.top();
}

int MatrixStack::size()
{
	return stack_.size();
}
