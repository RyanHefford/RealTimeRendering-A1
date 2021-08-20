#include "Camera.h"



Camera::Camera(float _width, float _height) {
	width = _width;
	height = _height;
	camSpeed = 0.5;
	yaw = -90;
	pitch = 0;
	CalculateMatrix();
}

void Camera::MoveCamera(float deltaTime, int direction) {
	switch (direction) {
		//Move Forward
	case 0:
		camPosition += camSpeed * camFront * deltaTime;
		break;
		//Move Backward
	case 1:
		camPosition -= camSpeed * camFront * deltaTime;
		break;
		//Move Left
	case 2:
		camPosition -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed * deltaTime;
		break;
		//Move Right
	case 3:
		camPosition += glm::normalize(glm::cross(camFront, camUp)) * camSpeed * deltaTime;
		break;
	default:
		break;
	}

}

glm::mat4& Camera::GetView() {
	return view;
}

glm::mat4& Camera::GetProj() {
	return proj;
}

void Camera::AddYawAndPitch(float Xoffset, float Yoffset)
{
	yaw += Xoffset;
	pitch += Yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
		
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
		
	camDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	camDirection.y = sin(glm::radians(pitch));
	camDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront = glm::normalize(camDirection);

}

void Camera::CalculateMatrix()
{
	proj = glm::perspective(glm::radians(45.0f), width / height, 0.01f, 100.0f);
	view = glm::lookAt(camPosition, camPosition + camFront, camUp);


	float position[] = { camPosition.x, camPosition.y, camPosition.z };
	float direction[] = { camFront.x, camFront.y, camFront.z }; 
	glLightfv(GL_LIGHT1, GL_POSITION, position);
}
