#include "RTRScene5.h"

RTRScene5::RTRScene5()
{
    sceneName = "5 - CPU Animated Geometry";
    model = glm::mat4(1.0f);
}

void RTRScene5::Init(Camera* cam) {
    cam->ResetPosition();

    spongeShader = nullptr;
    spongeShader = new Shader("Src/RTR.vert", "Src/RTR.frag", "Src/RTR.geom");



    ResetShaders();
}

void RTRScene5::ResetShaders()
{
    attributeByteSize = 0;

    glDeleteVertexArrays(1, &m_VertexArray);
    glDeleteBuffers(1, &m_VertexBuffer);
    glDeleteBuffers(1, &m_FaceElementBuffer);

    m_VertexBuffer = 0;
    m_VertexArray = 0;

    glGenBuffers(1, &m_VertexBuffer);
    glGenVertexArrays(1, &m_VertexArray);

    glBindVertexArray(m_VertexArray);

    CalculateVertexEndpoints();
    attributeByteSize += sizeof(float) * vertexSize;

    m_FaceElementBuffer = 0;

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
    attributeByteSize += sizeof(unsigned int) * facesSize;
}

void RTRScene5::CreateSponge(int subDivitions) {
    ResetArrays();
    DrawCube(0, 0, 0, 0.5, subDivitions);
    ResetShaders();
}


void RTRScene5::CalculateCurrentLerp()
{

    if (lerpForward) {
        currentLerpTime += (float)CalculateDelta() / 1000;
    }
    else {
        currentLerpTime -= (float)CalculateDelta() / 1000;
    }

    if (currentLerpTime >= lerpDuration) {
        currentLerpTime = lerpDuration;
        lerpForward = false;
    }
    else if (currentLerpTime <= 0) {
        currentLerpTime = 0;
        lerpForward = true;
    }

    float percentageCompleted = currentLerpTime / lerpDuration;

    percentageCompleted = 0.5 * glm::cos(glm::pi<float>() * percentageCompleted) + 0.5;

    //empty array
    std::fill(std::begin(current_vertex_points), std::end(current_vertex_points), 0);

    for (int i = 0; i < vertexSize; i+=3) {
        glm::vec3 currentV0 = glm::vec3(vertex_points[i], vertex_points[i+1], vertex_points[i+2]);
        glm::vec3 currentV1 = glm::vec3(end_point_vertex_points[i], end_point_vertex_points[i + 1], end_point_vertex_points[i + 2]);

        glm::vec3 currentLerp = (1.0f - percentageCompleted) * currentV0 + percentageCompleted * currentV1;

        current_vertex_points[i] = currentLerp.x;
        current_vertex_points[i+1] = currentLerp.y;
        current_vertex_points[i+2] = currentLerp.z;

    }
    UpdateVBO();
}

void RTRScene5::CalculateVertexEndpoints()
{
    std::fill(std::begin(end_point_vertex_points), std::end(end_point_vertex_points), 0);

    for (int i = 0; i < vertexSize; i += 3) {
        float currX = vertex_points[i];
        float currY = vertex_points[i+1];
        float currZ = vertex_points[i+2];


        float radius = (abs(currX) > abs(currY) ? currX : currY);
        radius = abs(radius) > abs(currZ) ? radius : currZ;
        radius = abs(radius);

        glm::vec3 endPoint = glm::vec3(currX, currY, currZ);
        endPoint = glm::normalize(endPoint);

        endPoint *= radius;

        end_point_vertex_points[i] = endPoint.x;
        end_point_vertex_points[i + 1] = endPoint.y;
        end_point_vertex_points[i + 2] = endPoint.z;

    }

    CalculateCurrentLerp();
}

int RTRScene5::CalculateDelta()
{
    static unsigned int last_time = 0;
    unsigned int cur_time = SDL_GetTicks();
    unsigned int td_milli = cur_time - last_time;
    last_time = cur_time;
    return td_milli;
}

void RTRScene5::UpdateVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize, current_vertex_points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}


int RTRScene5::DrawSponge(bool lighting, Camera* cam, Light* lightModel)
{
    int bytesUsed = 0;
    bytesUsed += attributeByteSize;
    
    CalculateCurrentLerp();

    glUseProgram(spongeShader->ID);

    GLint model_loc = glGetUniformLocation(spongeShader->ID, "u_model");
    GLint proj_loc = glGetUniformLocation(spongeShader->ID, "u_proj");
    GLint view_loc = glGetUniformLocation(spongeShader->ID, "u_view");

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(cam->GetProj()));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(cam->GetView()));
    bytesUsed += sizeof(glm::mat4) * 3;

    spongeShader->setVec3("u_camPosition", cam->camPosition);
    bytesUsed += sizeof(glm::vec3);
    spongeShader->setBool("u_lightsEnabled", lighting);
    bytesUsed += sizeof(bool);
    bytesUsed += lightModel->AddUniforms(*spongeShader);


    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES, facesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return bytesUsed;
}
