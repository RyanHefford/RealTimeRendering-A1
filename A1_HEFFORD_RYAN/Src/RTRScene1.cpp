
#include "RTRScene1.h"

RTRScene1::RTRScene1(){
    sceneName = "1 - Static Immediate Mode";
}

//goes through list and draws each of the vectors
void RTRScene1::DrawSponge(bool lighting, Camera* cam)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam->GetProj()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cam->GetView()));

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < facesSize; i++) {

		//color deciding logic
		if (i % 6 == 0) {
			glm::vec3  normals = { vertex_normals[i], vertex_normals[i + 1], vertex_normals[i + 2] };
			glm::vec3 colors = abs(normals);
			if (lighting) {
				glNormal3f(normals.x, normals.y, normals.z);

				GLfloat color[] = { colors.x, colors.y, colors.z, 1.0 };
				GLfloat specular[] = { 1,1,1,1 };
				GLfloat shine[] = { 60.0 };

				glMaterialfv(GL_FRONT, GL_AMBIENT, color);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, shine);
			}
			else {
				glColor3f(colors.x, colors.y, colors.z);
			}

		}

		glVertex3f(vertex_points[(faces[i] * 3)], vertex_points[(faces[i] * 3) + 1], vertex_points[(faces[i] * 3) + 2]);
	}

	glEnd();


}
