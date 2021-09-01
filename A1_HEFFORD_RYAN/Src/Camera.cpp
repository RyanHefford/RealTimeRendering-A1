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

glm::vec3& Camera::GetPosition() {
	return camPosition;
}

glm::vec3 Camera::GetDirection() {
	return camDirection;
}

glm::mat4& Camera::GetView() {
	return view;
}

glm::mat4& Camera::GetProj() {
	return proj;
}

//camera position reset for scene init
void Camera::ResetPosition(glm::vec3 newPosition) {

	camSpeed = newPosition.z / 4;
	camPosition = newPosition;
	
	camTarget = glm::vec3(0, 0, 0);
	camDirection = glm::normalize(camPosition - camTarget);
	up = glm::vec3(0, 1, 0);
	camRight = glm::normalize(glm::cross(up, camDirection));
	camUp = glm::cross(camDirection, camRight);
	camFront = glm::vec3(0, 0, -1);
	view = glm::mat4(1);
	proj = glm::mat4(1);
	yaw = -90;
	pitch = 0;
	CalculateMatrix();
	AddYawAndPitch(0, 0);
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
	proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.01f, 100.0f);
	view = glm::lookAt(camPosition, camPosition + camFront, camUp);


	float position[] = { camPosition.x, camPosition.y, camPosition.z };
	float direction[] = { camFront.x, camFront.y, camFront.z }; 
	glLightfv(GL_LIGHT1, GL_POSITION, position);
}
