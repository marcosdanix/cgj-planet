#include "util.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/exponential.hpp>

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

/////////////////////////////////////////////////////////////////////// Perlin

//Based on the published Java reference implementation by Ken Perlin
//Copyright 2002 Ken Perlin

const int p[] = { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

vec3 fade(vec3 t) { return t * t * t * (t * (6.0f * t - vec3(15.0f)) + vec3(10.0f)); }

float lerp(float t, float a, float b) { return a + t * (b - a); }
float grad(int hash, vec3 pos) {
	int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
	float  u = h<8 ? pos.x : pos.y,         // INTO 12 GRADIENT DIRECTIONS.
		   v = h<4 ? pos.y : h == 12 || h == 14 ? pos.x : pos.z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float noise(vec3 pos)
{
	vec3 cube = floor(pos);			//FIND UNIT CUBE THAT CONTAINS POINT.
	vec3 P = pos - cube;			//FIND RELATIVE X,Y,Z OF POINT IN CUBE
	vec3 f = fade(P);				//COMPUTE FADE CURVES FOR EACH OF X,Y,Z
	ivec3 c = ivec3(cube) & 255;
	int A = p[c.x] + c.y, AA = p[A] + c.z, AB = p[A + 1] + c.z,      // HASH COORDINATES OF
		B = p[c.x + 1] + c.y, BA = p[B] + c.z, BB = p[B + 1] + c.z;  // THE 8 CUBE CORNERS,

	return lerp(f.z, lerp(f.y, lerp(f.x, grad(p[AA],P),	                            // AND ADD
										 grad(p[BA],    P - vec3(1.0,0.0,0.0))),	// BLENDED
							   lerp(f.x, grad(p[AB],    P - vec3(0.0, 1.0, 0.0)),	// RESULTS
										 grad(p[BB],    P - vec3(1.0, 1.0, 0.0)))),	// FROM  8
				     lerp(f.y, lerp(f.x, grad(p[AA + 1],P - vec3(0.0, 0.0, 1.0)),	// CORNERS
										 grad(p[BA + 1],P - vec3(1.0, 0.0, 1.0))),  // OF CUBE
							   lerp(f.x, grad(p[AB + 1],P - vec3(0.0, 1.0, 1.0)),
										 grad(p[BB + 1],P - vec3(1.0, 1.0, 1.0)))));
}

float cgj::perlin(vec3 pos, float freq, int iterations, float decay, vec3 add)
{
	float acc = 0.0f;
	float fmul = 1.0f;
	float nmul = 1.0f;

	for (int i = 0; i < iterations; ++i) {
		acc += nmul*noise(fmul*freq*(pos + add));
		nmul = pow(1.0f / decay, (float)(i + 1));
		fmul *= 2.0;
	}

	return acc;
}
