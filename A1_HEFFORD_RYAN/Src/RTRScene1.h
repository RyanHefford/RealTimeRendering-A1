#pragma once
#include "RTRSceneBase.h"

class RTRScene1 : public RTRSceneBase {
public:

	RTRScene1(Light* lightModel);
	void Init(Camera* cam);
	~RTRScene1();

	int DrawSponge(bool lighting, Camera* cam, Light *lightModel);
	void InitaliseLights();
	Light* lightModelPtr;

	glm::vec3 vertex_normals[6] = { glm::vec3(0,0,1),glm::vec3(0,0,-1),glm::vec3(0,1,0),glm::vec3(0,-1,0), glm::vec3(1,0,0),glm::vec3(-1,0,0), };
};
