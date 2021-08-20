#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera 
{

private:

	glm::vec3 camPosition = glm::vec3(0,0,3);
	glm::vec3 camTarget = glm::vec3(0, 0, 0);
	glm::vec3 camDirection = glm::normalize(camPosition - camTarget);
	glm::vec3 up = glm::vec3(0,1,0);
	glm::vec3 camRight = glm::normalize(glm::cross(up, camDirection));
	glm::vec3 camUp = glm::cross(camDirection, camRight);
	glm::vec3 camFront = glm::vec3(0,0,-1);
	glm::mat4 view;
	glm::mat4 proj;

	float height, width, camSpeed, yaw, pitch;

public:
	Camera(float width, float height);
	~Camera();
	
	//direction: 0 = forward, 1 = backward, 2 = left, 3 = right
	void MoveCamera(float deltaTime, int direction);

	glm::mat4& GetView();
	glm::mat4& GetProj();
	void AddYawAndPitch(float Xoffset, float Yoffset);
	void CalculateMatrix();
};
