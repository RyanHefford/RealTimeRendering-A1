#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"

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

struct RTRMaterial {
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shininess;
};

class Light
{

private:

	#define MAX_LIGHTS 10 
	#define MAX_MATERIALS 3 
	struct RTRLight lightArray[MAX_LIGHTS];
	int numActiveLights = 1;

	std::string lightAttributes[9] = {
		"].Type",
		"].Ambient",
		"].Diffuse",
		"].Specular",
		"].Direction",
		"].Position",
		"].Constant",
		"].Linear",
		"].Quadratic"
	};

	std::string materialAttributes[4] = {
		"].Ambient",
		"].Diffuse",
		"].Specular",
		"].Shininess"
	};

public:
	struct RTRMaterial materialArray[MAX_MATERIALS];

	Light();
	~Light();

	void CreateLights();
	RTRLight* GetLight(int index);
	int AddUniforms(Shader &shader);
	int GetActiveNumLights();
	void UpdateDirectional(glm::vec3 newDirection);
	void ChangeLightNum(int change);

};
