#include "Camera.h"

#include <shared/System.h>
#include <client/components/Input.h>

class Camera *pCamera;
Camera *g_Camera() { return pCamera ? pCamera : new Camera(); }

Camera::Camera() : ClientComponent() {
	pCamera = this;
	pos = glm::vec3(0.0, 0.0, 0.0);
	rot = rot3(0.0, 0.0, 0.0);
	UpdateVectors();
}
Camera::~Camera() { pCamera = NULL; }
void Camera::Tick() {
	RotateX(-g_Input()->mouseY * 0.01);
	RotateZ(-g_Input()->mouseX * 0.01);
	UpdateVectors();
	glm::vec3 look2 = look;
	look2 *= 512.0;
	look2 *= g_System()->tickCoeff;
	glm::vec3 right2 = right;
	right2 *= 512.0;
	right2 *= g_System()->tickCoeff;
	glm::vec3 up2 = up;
	up2 *= 512.0;
	up2 *= g_System()->tickCoeff;
}
void Camera::RotateX(float rad) { rot.x += rad; }
void Camera::RotateY(float rad) { rot.y += rad; }
void Camera::RotateZ(float rad) { rot.z += rad; }
void Camera::UpdateVectors() {
	up = glm::rotate(glm::vec3(0.0, 0.0, 1.0), rot);
	look = glm::rotate(glm::vec3(0.0, 1.0, 0.0), rot);
	right = glm::rotate(glm::vec3(1.0, 0.0, 0.0), rot);
}
void Camera::StateChange(const STATE &lastState) {
	ClientComponent::StateChange(lastState);
	if (!lastState.ingame && state.ingame) {
		rot = rot3(0.0, 0.0, 0.0);
		pos = glm::vec3(0.0, 0.0, 0.0);
		UpdateVectors();
	}
}
