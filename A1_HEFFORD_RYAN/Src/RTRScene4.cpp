#include "RTRScene4.h"
#include "glm/gtx/string_cast.hpp"
RTRScene4::RTRScene4()
{
    sceneName = "4 - Instanced Geometry 2";
}

void RTRScene4::Init(Camera* cam) {
    //set cam position
    cam->ResetPosition(glm::vec3(0, 0, 7));

    spongeShader = nullptr;
    spongeShader = new Shader("Src/RTR.vert", "Src/RTR.frag", "Src/RTR.geom");

    for (int i = 0; i < MAX_INSTANCES; i++) {
        instanceModelMatrices[i] = glm::mat4(1);
    }
    RepositionModels();
    ResetShaders();
}

void RTRScene4::ResetShaders()
{
    attributeByteSize = 0;

    glDeleteVertexArrays(1, &m_VertexArray);
    glDeleteBuffers(1, &m_InstanceModelBuffer);
    glDeleteBuffers(1, &m_VertexBuffer);
    glDeleteBuffers(1, &m_FaceElementBuffer);

    m_VertexBuffer = 0;
    m_InstanceModelBuffer = 0;
    m_VertexArray = 0;

    glGenVertexArrays(1, &m_VertexArray);
    glGenBuffers(1, &m_VertexBuffer);
    glGenBuffers(1, &m_InstanceModelBuffer);
    
    glBindVertexArray(m_VertexArray);

    
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize, vertex_points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    attributeByteSize += sizeof(float) * vertexSize;

    attributeByteSize += UpdateMatrixVBO();

    m_FaceElementBuffer = 0;

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    attributeByteSize += sizeof(unsigned int) * facesSize;
}

void RTRScene4::CreateSponge(int subDivitions) {
    ResetArrays();
    DrawCube(0, 0, 0, 0.5, subDivitions);
    ResetShaders();
}

void RTRScene4::RepositionModels()
{
    int currModel = 0;
    float spacing = 1.5;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            instanceModelMatrices[currModel] = glm::translate(instanceModelMatrices[currModel], glm::vec3(i * spacing, j * spacing, 0));
            currModel++;
        }
    }
}

int RTRScene4::CalculateDelta()
{
    static unsigned int last_time = 0;
    unsigned int cur_time = SDL_GetTicks();
    unsigned int td_milli = cur_time - last_time;
    last_time = cur_time;
    return td_milli;
}

void RTRScene4::UpdateRotations(int td_milli)
{
    int currModel = 0;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (abs(i) + abs(j) != 0) {
                instanceModelMatrices[currModel] = glm::rotate(instanceModelMatrices[currModel], glm::radians(60 * ((float)td_milli / 1000)), glm::vec3(i, j, 0));
            }
            else {
                instanceModelMatrices[currModel] = glm::rotate(instanceModelMatrices[currModel], (float)td_milli / 1000, glm::vec3(0, 0, 1));
            }
            currModel++;
        }
    }

    UpdateMatrixVBO();
}

//call every frame to update rotations
int RTRScene4::UpdateMatrixVBO()
{
    int matrixLocation = glGetAttribLocation(spongeShader->ID, "model_Instance");

    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceModelBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MAX_INSTANCES, &instanceModelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < 4; i++)
    {
        int currIndex = matrixLocation + i;

        glEnableVertexAttribArray(currIndex);

        glVertexAttribPointer(currIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(GLfloat) * i * 4));

        glVertexAttribDivisor(currIndex, 1);
    }

    return sizeof(glm::mat4) * MAX_INSTANCES;
}

int RTRScene4::DrawSponge(bool lighting, Camera* cam, Light* lightModel)
{
    int bytesUsed = 0;
    bytesUsed += attributeByteSize;

    UpdateRotations(CalculateDelta());

    glUseProgram(spongeShader->ID);

    GLint model_loc = glGetUniformLocation(spongeShader->ID, "u_model");
    GLint proj_loc = glGetUniformLocation(spongeShader->ID, "u_proj");
    GLint view_loc = glGetUniformLocation(spongeShader->ID, "u_view");

    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(cam->GetProj()));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(cam->GetView()));
    bytesUsed += sizeof(glm::mat4) * 2;

    spongeShader->setVec3("u_camPosition", cam->GetPosition());
    bytesUsed += sizeof(glm::vec3);
    spongeShader->setBool("u_lightsEnabled", lighting);
    bytesUsed += sizeof(bool);
    bytesUsed += lightModel->AddUniforms(*spongeShader);

    glBindVertexArray(m_VertexArray);

    glDrawElementsInstanced(GL_TRIANGLES, facesSize, GL_UNSIGNED_INT, 0, MAX_INSTANCES);

    glBindVertexArray(0);

    return bytesUsed;
}
