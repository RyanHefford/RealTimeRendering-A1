#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

enum class RTRLightType: int{ DirectionalLight = 0, PointLight };

struct RTRLight {
	RTRLightType Type;
	glm::vec3 Ambient;      //   Directional, Point
	glm::vec3 Diffuse;      //   Directional, Point
	glm::vec3 Specular;     //   Directional, Point
	glm::vec3 Direction;    //   Directional
	glm::vec3 Position;     //   Point
	float Constant;         //   Point
	float Linear;           //   Point
	float Quadratic;        //   Point
};

#define MAX_LIGHTS 10 
struct RTRLight lightArray[MAX_LIGHTS]; 
int numLights;

class Light
{

private:

public:
	Light();
	~Light();

	void AddLight(RTRLight newLight);

};
