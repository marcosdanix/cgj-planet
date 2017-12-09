#include "transform.h"
#include <glm/gtc/matrix_transform.hpp>

cgj::Transform::Transform(): 
	scale_(1.0f), 
	orientation_(1.0f, 0.0f, 0.0f, 0.0f), 
	translation_(0.0f)
{
}

cgj::Transform::Transform(vec3 & scale, quat & orientation, vec3 & translation):
	scale_(scale),
	orientation_(orientation),
	translation_(translation)
{
}

cgj::Transform::Transform(float scale, quat & orientation, vec3 & translation):
	scale_(scale),
	orientation_(orientation),
	translation_(translation)
{
}

vec3 cgj::Transform::scale() const
{
	return scale_;
}

void cgj::Transform::scale(vec3 & s)
{
	scale_ = s;
}

quat cgj::Transform::orientation() const
{
	return orientation_;
}

void cgj::Transform::orientation(quat & q)
{
	orientation_ = q;
}

vec3 cgj::Transform::translation() const
{
	return translation_;
}

void cgj::Transform::translation(vec3 & t)
{
	translation_ = t;
}

void cgj::Transform::addScale(vec3& add)
{
	scale_ += add;
}

void cgj::Transform::addScale(float add)
{
	scale_ += vec3(add);
}

void cgj::Transform::mulScale(vec3& mul)
{
	scale_ *= mul;
}

void cgj::Transform::mulScale(float mul)
{
	scale_ *= vec3(mul);
}

void cgj::Transform::translate(vec3& t)
{
	translation_ += t;
}

void cgj::Transform::rotate(quat & r)
{
	orientation_ = r * orientation_;
}

void cgj::Transform::rotateAxis(vec3& axis, float radians)
{
	auto rotation = angleAxis(radians, axis);
	orientation_ = rotation * orientation_;
}

void cgj::Transform::rotateX(float radians)
{
	rotateAxis(vec3(1, 0, 0), radians);
}

void cgj::Transform::rotateY(float radians)
{
	rotateAxis(vec3(0, 1, 0), radians);
}

void cgj::Transform::rotateZ(float radians)
{
	rotateAxis(vec3(0, 0, 1), radians);
}

mat4 cgj::Transform::matrix()
{
	//Scale then Rotate then Translate
	//OR
	//Translation after Rotation after Scale
	//this order applies when right-multiplying vectors
	
	return glm::translate(mat4(1.0f), translation_) *
		mat4_cast(orientation_) *
		glm::scale(mat4(1.0f), scale_);
}

//plox don't use any 0 scale component
mat4 cgj::Transform::inverse()
{
	return glm::scale(mat4(1.0), 1.0f / scale_) *
		transpose(mat4_cast(orientation_)) *
		glm::translate(mat4(1.0f), -translation_);
}
