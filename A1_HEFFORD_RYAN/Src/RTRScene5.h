#pragma once
#include "RTRSceneBase.h"

class RTRScene5 : public RTRSceneBase {
public:

	RTRScene5();
	~RTRScene5();

	int DrawSponge(bool lighting, Camera* cam, Light* lightModel);
	void Init(Camera* cam);
	void ResetShaders();
	void CreateSponge(int subDivisions);
	void CalculateCurrentLerp();
	void CalculateVertexEndpoints();
	int CalculateDelta();
	void UpdateVBO();

	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_FaceElementBuffer;

	//contains current vertex positions after lerp
	float current_vertex_points[40000000];
	float end_point_vertex_points[40000000];

	float currentLerpTime = 0;
	float lerpDuration = 2.0f;
	bool lerpForward = true;

	Shader* spongeShader;

};
