#pragma once
#include "RTRSceneBase.h"

class RTRScene4 : public RTRSceneBase {
public:

#define MAX_INSTANCES 9


	RTRScene4();
	~RTRScene4();

	int DrawSponge(bool lighting, Camera* cam, Light* lightModel);
	void Init(Camera* cam);
	void ResetShaders();
	void CreateSponge(int subDivisions);
	void RepositionModels();
	int CalculateDelta();
	void UpdateRotations(int td_milli);
	int UpdateMatrixVBO();

	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_FaceElementBuffer;
	unsigned int m_InstanceModelBuffer;

	glm::mat4 instanceModelMatrices[MAX_INSTANCES];

	Shader* spongeShader;

};
