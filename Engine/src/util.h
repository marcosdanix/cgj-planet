#pragma once
#include <stack>
#include <glm/fwd.hpp>
#include "transform.h"

//Useful miscellanious header

using namespace glm;


namespace cgj {
	//Matrix stack, representing the composition of transformations in the scene tree
	class MatrixStack {
	public:
		void push(mat4& matrix);
		void pop();
		mat4 result();
		size_t size();
		bool empty();
	private:
		std::stack<mat4> stack_;
		std::stack<mat4> results_;
	};

	//More useful TransformStack, the matrix stack is kinda obsolete
	//The advantage is that it stores the inverses of each transform
	class TransformStack {
	public:
		void push(Transform& transform);
		void pop();
		mat4 result();
		mat4 inverse();
		size_t size();
		bool empty();
	private:
		std::stack<Transform> stack_;
		std::stack<mat4> results_;
		std::stack<mat4> inverse_;
	};

	float perlin(vec3 pos, float freq, int iterations = 1, float decay = 2.0f, vec3 add = vec3(0.0));
}