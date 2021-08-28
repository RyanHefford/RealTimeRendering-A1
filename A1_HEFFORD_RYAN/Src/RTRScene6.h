#pragma once
#include "RTRSceneBase.h"

class RTRScene6 : public RTRSceneBase {
public:

	RTRScene6();
	~RTRScene6();

	int DrawSponge(bool lighting, Camera* cam, Light* lightModel);
	void Init(Camera* cam);
	void ResetShaders();
	void CreateSponge(int subDivisions);

	int CalculateDelta();
	float CalculateLerpPercentage();

	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_FaceElementBuffer;

	float currentLerpTime = 0;
	float lerpDuration = 2.0f;
	bool lerpForward = true;

	Shader* spongeShader;

};
