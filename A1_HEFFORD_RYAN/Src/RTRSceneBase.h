#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <sdl/SDL.h>
#include <stb/stb_image.h>
#include "shader.h"
#include "Camera.h"


class RTRSceneBase{
public:
	
	RTRSceneBase();
	~RTRSceneBase();
	virtual void Init();
	virtual void CreateSponge(int subDivitions);
	void DrawCube(float x, float y, float z, float size, int recursionsLeft);
	virtual void DrawSponge(bool lighting, Camera* cam);
	//Naming Convention 1st letter:F/B (Front/Back), 2nd letter:T/B (Top/Bottom), 3rd letter:L/R (Left/Right)
	void AddToList(glm::vec3 FTL, glm::vec3 FTR, glm::vec3 FBL, glm::vec3 FBR, glm::vec3 BTL, glm::vec3 BTR, glm::vec3 BBL, glm::vec3 BBR);
	void AddFaces();
	void ResetArrays();
	void AddNormals(float x, float y, float z);

	

	const char* sceneName;

	unsigned int vertexSize = 0;
	unsigned int facesSize = 0;
	unsigned int normalSize = 0;

	float vertex_points[40000000];
	float vertex_normals[40000000];
	unsigned int faces[40000000];

};