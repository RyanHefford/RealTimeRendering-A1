#pragma once
#include "RTRSceneBase.h"

class RTRScene3 : public RTRSceneBase {
public:

	#define MAX_INSTANCES 9


	RTRScene3();
	~RTRScene3();

	int DrawSponge(bool lighting, Camera* cam, Light* lightModel);
	void Init(Camera* cam);
	void ResetShaders();
	void CreateSponge(int subDivisions);
	void RepositionModels();
	int CalculateDelta();
	void UpdateRotations(int td_milli);

	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_FaceElementBuffer;

	glm::mat4 modelList[MAX_INSTANCES];

	Shader* spongeShader;

};
