#include "camera.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>



#define DEG2RAD 0.01745329252f
#define QUAT_ID (quat(1.0f, 0.0f, 0.0f, 0.0f))
using namespace cgj;

cgj::OrbitControl::OrbitControl():
	orientation_(QUAT_ID), //no rotation
	distance_(1.0),
	rotspeed_(0.0),
	speed_(0.0),
	offset_(0.0f)
{
}

OrbitControl::OrbitControl(float distance, float rot_speed, float speed):
	orientation_(QUAT_ID), //no rotation
	distance_(distance),
	rotspeed_(rot_speed),
	speed_(speed),
	offset_(0.0f)
{
}

mat4 OrbitControl::view()
{
	mat4 translation = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, -(distance_+offset_)));
	mat4 orientation = mat4_cast(orientation_);
	return translation * orientation;
}

mat4 OrbitControl::inverse()
{
	mat4 translation = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, distance_ + offset_));
	mat4 orientation = mat4_cast(conjugate(orientation_));
	return orientation * translation;
}

void OrbitControl::rotate(float rotx, float roty)
{
	float rx = DEG2RAD * rotx * rotspeed_;
	float ry = DEG2RAD * roty * rotspeed_;

	//screen x corresponds to y-axis rotation and vice-versa
	float dx = ry;
	float dy = rx;

	quat rotationX = angleAxis(dx, vec3(1.0f, 0.0f, 0.0f));
	quat rotationY = angleAxis(dy, vec3(0.0f, 1.0f, 0.0f));
	orientation_ = rotationX * rotationY * orientation_;
}

void OrbitControl::move(float direction)
{
	offset_ += direction * speed_;
}

void OrbitControl::reset()
{
	offset_ = 0.0f;
	orientation_ = QUAT_ID;
}
