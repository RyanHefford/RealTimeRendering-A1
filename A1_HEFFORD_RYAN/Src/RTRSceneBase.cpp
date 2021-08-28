#include "RTRSceneBase.h"
#include <iostream>

RTRSceneBase::RTRSceneBase()
{
}

RTRSceneBase::~RTRSceneBase()
{
}

void RTRSceneBase::Init(Camera* cam)
{
}

void RTRSceneBase::CreateSponge(int subDivitions) {
	ResetArrays();
	DrawCube(0, 0, 0, 0.5, subDivitions);
}

void RTRSceneBase::DrawCube(float x, float y, float z, float size, int recursionsLeft) {

	if (recursionsLeft > 1) {
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				for (int k = -1; k < 2; k++) {

					//filter center cubes
					if (abs(i) + abs(j) + abs(k) > 1) {

						float newSize = size / 3;

						DrawCube(x + i * 2 * newSize, y + j * 2 * newSize, z + k * 2 * newSize, newSize, recursionsLeft - 1);
					}
				}
			}
		}
	}
	else {

		AddToList(
			glm::vec3{ x - size, y + size, z + size },
			glm::vec3{ x + size, y + size, z + size },
			glm::vec3{ x - size, y - size, z + size },
			glm::vec3{ x + size, y - size, z + size },
			glm::vec3{ x - size, y + size, z - size },
			glm::vec3{ x + size, y + size, z - size },
			glm::vec3{ x - size, y - size, z - size },
			glm::vec3{ x + size, y - size, z - size }
		);

	}

}


//goes through list and draws each of the vectors
int RTRSceneBase::DrawSponge(bool lighting, Camera* cam, Light *lightModel)
{
	return 0;
	
}


void RTRSceneBase::AddToList(glm::vec3 FTL, glm::vec3 FTR, glm::vec3 FBL, glm::vec3 FBR, glm::vec3 BTL, glm::vec3 BTR, glm::vec3 BBL, glm::vec3 BBR) {

	glm::vec3 tempArray[] = { FTL,FTR,FBL,FBR,BTL,BTR,BBL,BBR };

	for (glm::vec3 vector3 : tempArray) {
		vertex_points[vertexSize] = vector3.x;
		vertexSize += 1;
		vertex_points[vertexSize] = vector3.y;
		vertexSize += 1;
		vertex_points[vertexSize] = vector3.z;
		vertexSize += 1;
	}

	AddFaces();

}

void RTRSceneBase::AddFaces() {

	int adjustedIndex = (vertexSize/3) - 9;
	int numNormalRepeat = 2;

	//Front face triangles

	faces[facesSize] = adjustedIndex + 1;
	facesSize++;
	faces[facesSize] = adjustedIndex + 3;
	facesSize++;
	faces[facesSize] = adjustedIndex + 2;
	facesSize++;
	faces[facesSize] = adjustedIndex + 2;
	facesSize++;
	faces[facesSize] = adjustedIndex + 3;
	facesSize++;
	faces[facesSize] = adjustedIndex + 4;
	facesSize++;

	//Back face triangles

	faces[facesSize] = adjustedIndex + 6;
	facesSize++;
	faces[facesSize] = adjustedIndex + 8;
	facesSize++;
	faces[facesSize] = adjustedIndex + 5;
	facesSize++;
	faces[facesSize] = adjustedIndex + 5;
	facesSize++;
	faces[facesSize] = adjustedIndex + 8;
	facesSize++;
	faces[facesSize] = adjustedIndex + 7;
	facesSize++;

	//Top face triangles

	faces[facesSize] = adjustedIndex + 5;
	facesSize++;
	faces[facesSize] = adjustedIndex + 1;
	facesSize++;
	faces[facesSize] = adjustedIndex + 2;
	facesSize++;
	faces[facesSize] = adjustedIndex + 2;
	facesSize++;
	faces[facesSize] = adjustedIndex + 6;
	facesSize++;
	faces[facesSize] = adjustedIndex + 5;
	facesSize++;

	//Bottom face triangles

	faces[facesSize] = adjustedIndex + 3;
	facesSize++;
	faces[facesSize] = adjustedIndex + 8;
	facesSize++;
	faces[facesSize] = adjustedIndex + 4;
	facesSize++;
	faces[facesSize] = adjustedIndex + 3;
	facesSize++;
	faces[facesSize] = adjustedIndex + 7;
	facesSize++;
	faces[facesSize] = adjustedIndex + 8;
	facesSize++;

	//Right face triangles

	faces[facesSize] = adjustedIndex + 2;
	facesSize++;
	faces[facesSize] = adjustedIndex + 4;
	facesSize++;
	faces[facesSize] = adjustedIndex + 8;
	facesSize++;
	faces[facesSize] = adjustedIndex + 8;
	facesSize++;
	faces[facesSize] = adjustedIndex + 6;
	facesSize++;
	faces[facesSize] = adjustedIndex + 2;
	facesSize++;

	//Left face triangles

	faces[facesSize] = adjustedIndex + 5;
	facesSize++;
	faces[facesSize] = adjustedIndex + 7;
	facesSize++;
	faces[facesSize] = adjustedIndex + 3;
	facesSize++;
	faces[facesSize] = adjustedIndex + 3;
	facesSize++;
	faces[facesSize] = adjustedIndex + 1;
	facesSize++;
	faces[facesSize] = adjustedIndex + 5;
	facesSize++;
}

void RTRSceneBase::ResetArrays()
{
	//empty arrays
	std::fill(std::begin(vertex_points), std::end(vertex_points), 0);
	std::fill(std::begin(faces), std::end(faces), 0);

	//reset sizes to 0
	vertexSize = 0;
	facesSize = 0;

}
