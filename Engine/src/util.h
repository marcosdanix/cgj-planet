#pragma once
#include <stack>
#include <glm/fwd.hpp>

//Useful miscellanious header

using namespace glm;

//Matrix stack, representing the composition of transformations in the scene tree
class MatrixStack {
public:
	void push(mat4 matrix);
	void pop();
	mat4 result();
	int size();
private:
	std::stack<mat4> stack_;
	std::stack<mat4> results_;

};