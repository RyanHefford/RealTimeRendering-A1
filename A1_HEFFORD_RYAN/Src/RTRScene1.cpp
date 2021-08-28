
#include "RTRScene1.h"

RTRScene1::RTRScene1(Light* lightModel){
    sceneName = "1 - Static Immediate Mode";

    lightModelPtr = lightModel;
}

void RTRScene1::Init(Camera* cam) {
	cam->ResetPosition();
    InitaliseLights();
}

//goes through list and draws each of the vectors
int RTRScene1::DrawSponge(bool lighting, Camera* cam, Light* lightModel)
{
	int bytesUsed = 0;

	bytesUsed += lightModelPtr->GetActiveNumLights() * sizeof(RTRLight);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam->GetProj()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cam->GetView()));
	bytesUsed += sizeof(glm::mat4) * 2;

	glBegin(GL_TRIANGLES);
	int currentNormalIndex = 0;
	for (int i = 0; i < facesSize; i++) {

		//color deciding logic
		if (i % 6 == 0) {
			int currentMaterialIndex = currentNormalIndex / 2;

			glm::vec3 ambient = lightModel->materialArray[currentMaterialIndex].Ambient;
			glm::vec3 diffuse = lightModel->materialArray[currentMaterialIndex].Diffuse;
			GLfloat currentAmbient[] = { ambient.x, ambient.y, ambient.z, 1.0 };
			GLfloat currentDiffuse[] = { diffuse.x, diffuse.y, diffuse.z, 1.0 };

			glNormal3f(vertex_normals[currentNormalIndex].x, vertex_normals[currentNormalIndex].y, vertex_normals[currentNormalIndex].z);
			bytesUsed += sizeof(float) * 3;
			//check for lighting
			if (lighting) {

				glm::vec3 specular = lightModel->materialArray[currentMaterialIndex].Specular;
				GLfloat shine[] = { lightModel->materialArray[currentMaterialIndex].Shininess };
				GLfloat currentSpecular[] = { specular.x, specular.y, specular.z, 1.0 };

				glMaterialfv(GL_FRONT, GL_AMBIENT, currentAmbient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, currentDiffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, currentSpecular);
				bytesUsed += sizeof(float) * 4 * 3;
				glMaterialfv(GL_FRONT, GL_SHININESS, shine);
				bytesUsed += sizeof(float);
			}
			else {
				GLfloat shine[] = { 0 };
				GLfloat currentSpecular[] = { 0, 0, 0, 0 };

				glMaterialfv(GL_FRONT, GL_AMBIENT, currentAmbient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, currentDiffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, currentSpecular);
				bytesUsed += sizeof(float) * 4 * 3;
				glMaterialfv(GL_FRONT, GL_SHININESS, shine);
				bytesUsed += sizeof(float);
			}
			

			currentNormalIndex = (currentNormalIndex + 1) % 6;
			
		}

		glVertex3f(vertex_points[(faces[i] * 3)], vertex_points[(faces[i] * 3) + 1], vertex_points[(faces[i] * 3) + 2]);
		bytesUsed += sizeof(float) * 3;
	}

	glEnd();

	return bytesUsed;
}

void RTRScene1::InitaliseLights()
{
    glEnable(GL_NORMALIZE);
    int currGlLight = GL_LIGHT0;
    for (int i = 0; i < MAX_LIGHTS - 2; i++) {


        RTRLight* currLight = lightModelPtr->GetLight(i);

		int type = currLight->Type == RTRLightType::DirectionalLight ? 1.0 : 0.0;

        //create camera light
        GLfloat lightPosition[4];
        GLfloat ambientLight[] = { currLight->Ambient.x, currLight->Ambient.y, currLight->Ambient.z, type };
        GLfloat diffuseLight[] = { currLight->Diffuse.x, currLight->Diffuse.y, currLight->Diffuse.z, type };
        GLfloat specularLight[] = { currLight->Specular.x, currLight->Specular.y, currLight->Specular.z, type };

        glLightfv(currGlLight, GL_AMBIENT, ambientLight);
        glLightfv(currGlLight, GL_DIFFUSE, diffuseLight);
        glLightfv(currGlLight, GL_SPECULAR, specularLight);

        if (i != 0) {
            lightPosition[0] =  currLight->Position.x;
            lightPosition[1] = currLight->Position.y;
            lightPosition[2] = currLight->Position.z;
            lightPosition[3] = type;

			GLfloat constantAtt[] = { currLight->Constant };
			GLfloat linearAtt[] = { currLight->Linear };
			GLfloat quadraticAtt[] = { currLight->Quadratic };

			glLightfv(currGlLight, GL_CONSTANT_ATTENUATION, constantAtt);
			glLightfv(currGlLight, GL_LINEAR_ATTENUATION, linearAtt);
			glLightfv(currGlLight, GL_QUADRATIC_ATTENUATION, quadraticAtt);
        }
        else {
            lightPosition[0] = 0.0;
            lightPosition[1] = 0.0;
            lightPosition[2] = 3.0;
            lightPosition[3] = 0.0;
        }

        glLightfv(currGlLight, GL_POSITION, lightPosition);

		currGlLight++;
    }

    //enable the lighting
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
