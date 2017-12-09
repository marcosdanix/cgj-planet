#include "util.h"
#include <glm/mat4x4.hpp>

using namespace cgj;

/////////////////////////////////////////////////////////////////////// MatrixStack

void MatrixStack::push(mat4& matrix)
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
size_t MatrixStack::size()
{
	return stack_.size();
}

bool MatrixStack::empty()
{
	return stack_.empty();
}

/////////////////////////////////////////////////////////////////////// MatrixStack

void TransformStack::push(Transform & transform)
{
	if (!stack_.empty())
	{
		mat4 result;
		mat4 inverse;

		if (!results_.empty()) {
			result = results_.top();
			inverse = inverse_.top();
		}
		else {
			result = inverse = mat4(1.0f);
		}

		results_.push(result * stack_.top().matrix());
		inverse_.push(stack_.top().inverse() * inverse);
	}

	stack_.push(transform);

}

void TransformStack::pop()
{
	stack_.pop();
	if (results_.size() > 0) {
		results_.pop();
		inverse_.pop();
	}
}

mat4 TransformStack::result()
{
	if (stack_.empty())
		return mat4(1.0f);

	if (results_.empty())
		return stack_.top().matrix();
	else
		return results_.top() * stack_.top().matrix();
}

mat4 TransformStack::inverse()
{
	if (stack_.empty())
		return mat4(1.0f);

	if (inverse_.empty())
		return stack_.top().inverse();
	else
		return stack_.top().inverse() * inverse_.top();
}

size_t TransformStack::size()
{
	return stack_.size();
}

bool TransformStack::empty()
{
	return stack_.empty();
}
