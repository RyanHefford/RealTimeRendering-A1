#include "Light.h"

Light::Light()
{

}

void Light::AddLight(RTRLight newLight)
{
	if (numLights < MAX_LIGHTS) {
		lightArray[numLights] = newLight;
	}
}
