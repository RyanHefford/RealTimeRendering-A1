#include "RTRScene2.h"

RTRScene2::RTRScene2()
{
	sceneName = "2 - Static Modern Shaders";
    model = glm::mat4(1.0f);
}

void RTRScene2::Init(Camera* cam) {
    cam->ResetPosition();

    spongeShader = nullptr;
    spongeShader = new Shader("Src/RTR.vert", "Src/RTR.frag", "Src/RTR.geom");

    ResetShaders();
}

void RTRScene2::ResetShaders()
{
    attributeByteSize = 0;
    glDeleteVertexArrays(1, &m_VertexArray);
    glDeleteBuffers(1, &m_VertexBuffer);
    glDeleteBuffers(1, &m_FaceElementBuffer);

    m_VertexBuffer = 0;

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize, vertex_points, GL_STATIC_DRAW);

    attributeByteSize += sizeof(float) * vertexSize;

    m_VertexArray = 0;

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    m_FaceElementBuffer = 0;

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

    attributeByteSize += sizeof(unsigned int) * facesSize;
}

void RTRScene2::CreateSponge(int subDivitions) {
    ResetArrays();
    DrawCube(0, 0, 0, 0.5, subDivitions);
    ResetShaders();
}

int RTRScene2::DrawSponge(bool lighting, Camera* cam, Light* lightModel)
{
    int bytesUsed = 0;
    bytesUsed += attributeByteSize;

    glUseProgram(spongeShader->ID);

    GLint model_loc = glGetUniformLocation(spongeShader->ID, "u_model");
    GLint proj_loc = glGetUniformLocation(spongeShader->ID, "u_proj");
    GLint view_loc = glGetUniformLocation(spongeShader->ID, "u_view");
  
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(cam->GetProj()));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(cam->GetView()));
    bytesUsed += sizeof(glm::mat4) * 3;

    spongeShader->setBool("u_lightsEnabled", lighting);
    bytesUsed += sizeof(bool);
    spongeShader->setVec3("u_camPosition", cam->GetPosition());
    bytesUsed += sizeof(glm::vec3);

    bytesUsed += lightModel->AddUniforms(*spongeShader);


    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES, facesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return bytesUsed;
}
