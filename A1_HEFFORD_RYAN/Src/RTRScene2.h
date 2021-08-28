#pragma once
#include "RTRSceneBase.h"

class RTRScene2 : public RTRSceneBase {
public:

	RTRScene2();
	~RTRScene2();

	int DrawSponge(bool lighting, Camera* cam, Light* lightModel);
	void Init(Camera* cam);
	void ResetShaders();
	void CreateSponge(int subDivisions);

	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_FaceElementBuffer;

	Shader* spongeShader;

};
