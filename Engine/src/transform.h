#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// Scale, Quaternion, Translation (SQT) Transform class

using namespace glm;

namespace cgj {

	class Transform {
		Transform();
		Transform(vec3& scale, quat& orientation, vec3& translation);
		Transform(float scale, quat& orientation, vec3& translation);
		
		//Setters and Getters
		vec3 scale() const;
		void scale(vec3& s);
		quat orientation() const;
		void orientation(quat& q);
		vec3 translation() const;
		void translation(vec3& t);

		//Transformers

		void addScale(vec3& add);
		void addScale(float add);
		void mulScale(vec3& mul);
		void mulScale(float mul);
		void translate(vec3& t);
		void rotate(quat& r);
		void rotateAxis(vec3& axis, float radians);
		void rotateX(float radians);
		void rotateY(float radians);
		void rotateZ(float radians);

		mat4 matrix();
	private:
		vec3 scale_;
		quat orientation_;
		vec3 translation_;
	};

}