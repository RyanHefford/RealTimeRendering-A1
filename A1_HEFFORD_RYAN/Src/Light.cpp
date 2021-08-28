#include "Light.h"

Light::Light()
{

	//First Material
	materialArray[0] = { 
		{0.2, 0.2, 0.2},	//ambient
		{0.5, 0.5, 0.5},	//diffuse
		{0.7, 0.7, 0.7},	//specular
		32.0f				//shininess
	};
	//Second Material
	materialArray[1] = {
		{0.2, 0.2, 0.7},	//ambient
		{0.2, 0.2, 0.7},	//diffuse
		{0.7, 0.7, 0.7},	//specular
		32.0f				//shininess
	};
	//Third Material
	materialArray[2] = {
		{0.9, 0.2, 0.2},	//ambient
		{0.9, 0.2, 0.2},	//diffuse
		{0.7, 0.7, 0.7},	//specular
		32.0f				//shininess
	};

	CreateLights();
}

void Light::CreateLights()
{
	int numLights = 0;
	//light 0 (Directional)
	lightArray[numLights] = RTRLight{
		RTRLightType::DirectionalLight,
		glm::vec3(0.1, 0.1, 0.1),
		glm::vec3(0.5, 0.5, 0.5),
		glm::vec3(0.6, 0.6, 0.6),
		glm::vec3(0.0, 0.0, 1.0)
	};
	//point light 1
	numLights++;
	lightArray[numLights] = RTRLight{
		RTRLightType::PointLight,
		glm::vec3(0.0, 0.0, 0.2),
		glm::vec3(0.0, 0.0, 0.5),
		glm::vec3(0.0, 0.0, 1.0),
		glm::vec3(0),
		glm::vec3(0,0,0),
		0.4,0,0.001
	};
	//point light 2
	numLights++;
	lightArray[numLights] = RTRLight{
		RTRLightType::PointLight,
		glm::vec3(0.0, 0.2, 0.0),
		glm::vec3(0.0, 0.5, 0.0),
		glm::vec3(0.0, 1.0, 0.0),
		glm::vec3(0),
		glm::vec3(1,1,1),
		0.4,0,0.001
	};
	//point light 3
	numLights++;
	lightArray[numLights] = RTRLight{
		RTRLightType::PointLight,
		glm::vec3(0.2, 0.0, 0.0),
		glm::vec3(0.5, 0.0, 0.0),
		glm::vec3(1.0, 0.0, 0.0),
		glm::vec3(0),
		glm::vec3(1,1,-1),
		0.4,0,0.001
	};
	//point light 4
	numLights++;
	lightArray[numLights] = RTRLight{
		RTRLightType::PointLight,
		glm::vec3(0.0, 0.2, 0.2),
		glm::vec3(0.0, 0.5, 0.5),
		glm::vec3(0.0, 0.5, 0.5),
		glm::vec3(0),
		glm::vec3(-1,1,-1),
		0.4,0,0.001
	};
	//point light 5
	numLights++;
	lightArray[numLights] = RTRLight{
		RTRLightType::PointLight,
		glm::vec3(0.2, 0.0, 0.2),
		glm::vec3(0.5, 0.0, 0.5),		
		glm::vec3(0.5, 0.0, 0.5),
		glm::vec3(0),
		glm::vec3(-1,1,1),
		1,0.045,0.0075
	};
	//point light 6
	numLights++;
	lightArray[numLights] = RTRLight{
		RTRLightType::PointLight,
		glm::vec3(0.2, 0.2, 0.0),
		glm::vec3(0.5, 0.5, 0.0),
		glm::vec3(0.5, 0.5, 0.0),
		glm::vec3(0),
		glm::vec3(0,1,0),
		1,0.045,0.0075
	};
	//point light 7
	numLights++;
	lightArray[numLights] = RTRLight{
		RTRLightType::PointLight,
		glm::vec3(0.1, 0.2, 0.1),
		glm::vec3(0.25, 0.5, 0.25),
		glm::vec3(0.25, 0.5, 0.25),
		glm::vec3(0),
		glm::vec3(1,0,0),
		1,0.045,0.0075
	};
	//point light 8
	numLights++;
	lightArray[numLights] = RTRLight{
		RTRLightType::PointLight,
		glm::vec3(0.4, 0.0, 0.0),
		glm::vec3(0.6, 0.0, 0.0),
		glm::vec3(0.6, 0.0, 0.0),
		glm::vec3(0),
		glm::vec3(0,-1,0),
		1,0.045,0.0075
	};
	//point light 9
	numLights++;
	lightArray[numLights] = RTRLight{
		RTRLightType::PointLight,
		glm::vec3(0.0, 0.0, 0.4),
		glm::vec3(0.0, 0.0, 0.6),
		glm::vec3(0.0, 0.0, 0.6),
		glm::vec3(0),
		glm::vec3(-1,0,0),
		1,0.045,0.0075
	};
}

RTRLight* Light::GetLight(int index) {
	return &lightArray[index];
}

void Light::ChangeLightNum(int change) {
	if (numActiveLights + change > 0 && numActiveLights + change < MAX_LIGHTS) {
		numActiveLights += change;
	}
}

int Light::AddUniforms(Shader& shader)
{
	int bytesUsed = 0;
	shader.setInt("u_numLights", numActiveLights);
	bytesUsed += sizeof(int);
	for (int i = 0; i < numActiveLights; i++) {
		RTRLight currentLight = lightArray[i];
		std::string index = std::to_string(i);

		std::string nextAttribute = "u_lights[" + index + lightAttributes[0];
		shader.setInt(nextAttribute, (int)currentLight.Type);

		nextAttribute = "u_lights[" + index + lightAttributes[1];
		shader.setVec3(nextAttribute, currentLight.Ambient);
		nextAttribute = "u_lights[" + index + lightAttributes[2];
		shader.setVec3(nextAttribute, currentLight.Diffuse);
		nextAttribute = "u_lights[" + index + lightAttributes[3];
		shader.setVec3(nextAttribute, currentLight.Specular);
		nextAttribute = "u_lights[" + index + lightAttributes[4];
		shader.setVec3(nextAttribute, currentLight.Direction);
		nextAttribute = "u_lights[" + index + lightAttributes[5];
		shader.setVec3(nextAttribute, currentLight.Position);

		nextAttribute = "u_lights[" + index + lightAttributes[6];
		shader.setFloat(nextAttribute, currentLight.Constant);
		nextAttribute = "u_lights[" + index + lightAttributes[7];
		shader.setFloat(nextAttribute, currentLight.Linear);
		nextAttribute = "u_lights[" + index + lightAttributes[8];
		shader.setFloat(nextAttribute, currentLight.Quadratic);

		bytesUsed += sizeof(currentLight);
	}


	for (int i = 0; i < 3; i++) {
		RTRMaterial currentMaterial = materialArray[i];
		std::string index = std::to_string(i);
		std::string nextAttribute = "u_materials[" + index + materialAttributes[0];
		shader.setVec3(nextAttribute, currentMaterial.Ambient);
		nextAttribute = "u_materials[" + index + materialAttributes[1];
		shader.setVec3(nextAttribute, currentMaterial.Diffuse);
		nextAttribute = "u_materials[" + index + materialAttributes[2];
		shader.setVec3(nextAttribute, currentMaterial.Specular);
		nextAttribute = "u_materials[" + index + materialAttributes[3];
		shader.setFloat(nextAttribute, currentMaterial.Shininess);

		bytesUsed += sizeof(currentMaterial);
	}

	return bytesUsed;
}

int Light::GetActiveNumLights()
{
	return numActiveLights;
}

void Light::UpdateDirectional(glm::vec3 newDirection)
{
	lightArray[0].Direction = newDirection;
}
