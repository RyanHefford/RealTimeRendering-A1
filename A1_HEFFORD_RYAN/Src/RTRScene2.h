#pragma once
#include "RTRSceneBase.h"

class RTRScene2 : public RTRSceneBase {
public:

	RTRScene2();
	~RTRScene2();

	virtual void CreateSponge(int subDivitions);
	void DrawCube(float x, float y, float z, float size, int recursionsLeft);

};
