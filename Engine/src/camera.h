#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

namespace cgj {

	class CameraControl {
	public:
		virtual ~CameraControl() {}
		virtual mat4 view() = 0;
		virtual mat4 inverse() = 0;
	};

	class Camera {
	public:
		Camera() : control_(nullptr), projection_(mat4(1.0)) {}
		Camera(CameraControl* control, mat4& projection) : control_(control), projection_(projection) {}
		mat4 view() { return control_->view(); }
		mat4 projection() { return projection_; }
		mat4 inverseView() { return control_->inverse(); }
	private:
		CameraControl* control_;
		mat4 projection_;

	};

	class OrbitControl : public CameraControl {
	public:
		OrbitControl();
		OrbitControl(float distance, float rot_speed, float speed);
		mat4 view();
		mat4 inverse();
		void rotate(float rotx, float roty);
		void move(float direction);
		void reset();
	private:
		quat orientation_;
		float distance_;		
		float rotspeed_;
		float speed_;
		float offset_;

	};

}