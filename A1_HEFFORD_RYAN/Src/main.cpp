//-----------------------------------------------------------------------------
// main.cpp
//-----------------------------------------------------------------------------
// Assignment 1 Starter Code
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include "RTRApp.h"

#define GLT_IMPLEMENTATION
#include <gltext/gltext.h>

//stores all time related values
typedef struct { float frameRate, td_milliTotal; int frames, numberOfLights; bool isImmidiateMode; } Global;

typedef struct { bool QuitApp, BackFace, DepthBuffer, Lighting, WireFrame, fullDisplay; } Toggles;

typedef struct { bool moveLeft, moveRight, moveForwards, moveBackwards; float mouseLastX, mouseLastY, mouseSensitivity; } Movement;


class AssignmentApp : public RTRApp
{
public:
    AssignmentApp(const char* title, bool fullscreen = false, int width = 1024, int height = 768)
        : RTRApp(title, fullscreen, width, height) {}
    int Init();
    void Done();
    bool Tick(unsigned int td_milli);

private:

    Global g = { 0,0,0,1, true };
    Toggles tog = { false, true, true, true, false, true };
    Movement move = { false, false, false, false, (float)m_WindowWidth/2, (float)m_WindowHeight / 2, 0.075f };

    void CheckInput();
    void UpdateState(unsigned int td_milli);
    void RenderFrame();
    void RenderOSD();
    void CheckToggles();
    void InitializeLights();
    void ChangeNumLights(int change);
    void ChangeRecursions(int change);
};

bool AssignmentApp::Tick(unsigned int td_milli)
{
    tog.QuitApp = false;
    CheckInput();
    UpdateState(td_milli);
    RenderFrame();
    return tog.QuitApp;
}

void AssignmentApp::CheckInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        
        if (event.type == SDL_KEYDOWN) {

            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                tog.QuitApp = true;
                break;

            case SDLK_w:
                move.moveForwards = true;
                break;

            case SDLK_a:
                move.moveLeft = true;
                break;

            case SDLK_s:
                move.moveBackwards = true;
                break;

            case SDLK_d:
                move.moveRight = true;
                break;

            case SDLK_KP_PLUS:
                ChangeRecursions(1);
                break;

            case SDLK_EQUALS:
                ChangeRecursions(1);
                break;

            case SDLK_KP_MINUS:
                ChangeRecursions(-1);
                break;

            case SDLK_MINUS:
                ChangeRecursions(-1);
                break;

            case SDLK_l:
                tog.Lighting = !tog.Lighting;
                break;

            case SDLK_c:
                tog.BackFace = !tog.BackFace;
                break;

            case SDLK_x:
                tog.WireFrame = !tog.WireFrame;
                break;

            case SDLK_h:
                tog.fullDisplay = !tog.fullDisplay;
                break;

            case SDLK_z:
                tog.DepthBuffer = !tog.DepthBuffer;
                break;

            case SDLK_COMMA:
                ChangeNumLights(-1);
                break;
            case SDLK_PERIOD:
                ChangeNumLights(1);
                break;
            case SDLK_1:
                currentScene = sceneList[0];
                g.isImmidiateMode = true;
                recursions = 1;
                currentScene->CreateSponge(recursions);
                currentScene->Init();
                break;
            case SDLK_2:
                currentScene = sceneList[1];
                g.isImmidiateMode = false;
                recursions = 1;
                currentScene->CreateSponge(recursions);
                currentScene->Init();
                break;
            case SDLK_3:
                break;
            case SDLK_4:
                break;
            case SDLK_5:
                break;
            case SDLK_6:
                break;
            }
        }

        if(event.type == SDL_KEYUP){
            switch (event.key.keysym.sym) {
            case SDLK_w:
                move.moveForwards = false;
                break;

            case SDLK_a:
                move.moveLeft = false;
                break;

            case SDLK_s:
                move.moveBackwards = false;
                break;

            case SDLK_d:
                move.moveRight = false;
                break;

            }
        }

        //check for mouse movement
        if (event.type == SDL_MOUSEMOTION) {

            float xOffset = event.motion.xrel * move.mouseSensitivity;
            float yOffset = -event.motion.yrel * move.mouseSensitivity;

            cam->AddYawAndPitch(xOffset, yOffset);

        }

    }
}

void AssignmentApp::UpdateState(unsigned int td_milli)
{
    g.frames++;
    //update FPS
    if (g.frames > 100) {
        float averageMilli = g.td_milliTotal / 100;
        g.frameRate = roundf((1000.0 / averageMilli) * 100) / 100;
        g.frames = 0;
        g.td_milliTotal = 0;
    }
    g.td_milliTotal += td_milli;

    float milliseconds = (float)td_milli / 1000;

    if (move.moveForwards) {
        cam->MoveCamera(milliseconds, 0);
    }
    if (move.moveBackwards) {
        cam->MoveCamera(milliseconds, 1);
    }
    if (move.moveLeft) {
        cam->MoveCamera(milliseconds, 2);
    }
    if (move.moveRight) {
        cam->MoveCamera(milliseconds, 3);
    }
     
    cam->CalculateMatrix();

    // This is where we will do all our model updating, physics, etc...
}

// Render On-Screen Display
void AssignmentApp::RenderOSD()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    char fpsBuffer[10];
    snprintf(fpsBuffer, sizeof fpsBuffer, "FPS: %f", g.frameRate);

    char sceneBuffer[40];
    snprintf(sceneBuffer, sizeof sceneBuffer, "Scene: %s", currentScene->sceneName);

    char subDivisionBuffer[20];
    snprintf(subDivisionBuffer, sizeof subDivisionBuffer, "Subdivisions: %i", recursions);

    char resolutionBuffer[30];
    snprintf(resolutionBuffer, sizeof resolutionBuffer, "Resolution: %i X %i", m_ScreenHeight, m_ScreenWidth);

    char numVerticesBuffer[60];
    snprintf(numVerticesBuffer, sizeof numVerticesBuffer, "Vertices: %u", currentScene->vertexSize);

    char numFacesBuffer[60];
    snprintf(numFacesBuffer, sizeof numFacesBuffer, "Faces: %u", currentScene->facesSize);

    char dataSizeBuffer[30];
    snprintf(dataSizeBuffer, sizeof dataSizeBuffer, "Data Size: %i", m_ScreenHeight);

    char lightingBuffer[30];
    snprintf(lightingBuffer, sizeof lightingBuffer, tog.Lighting ? "Lighting: ON" : "Lighting: OFF");

    char depthTestBuffer[30];
    snprintf(depthTestBuffer, sizeof depthTestBuffer, tog.DepthBuffer ? "Depth Test: ON" : "Depth Test: OFF");

    char backFaceBuffer[30];
    snprintf(backFaceBuffer, sizeof backFaceBuffer, tog.BackFace ? "Backface: ON" : "Backface: OFF");

    GLTtext* frameRate = gltCreateText();
    GLTtext* sceneName = gltCreateText();
    GLTtext* subDivisions = gltCreateText();
    GLTtext* resolution = gltCreateText();
    GLTtext* numVertices = gltCreateText();
    GLTtext* numFaces = gltCreateText();
    GLTtext* dataSize = gltCreateText();
    GLTtext* lighting = gltCreateText();
    GLTtext* depthTest = gltCreateText();
    GLTtext* backFace = gltCreateText();

    gltSetText(frameRate, fpsBuffer);
    gltSetText(sceneName, sceneBuffer);
    gltSetText(subDivisions, subDivisionBuffer);
    gltSetText(resolution, resolutionBuffer);
    gltSetText(numVertices, numVerticesBuffer);
    gltSetText(numFaces, numFacesBuffer);
    gltSetText(dataSize, dataSizeBuffer);
    gltSetText(lighting, lightingBuffer);
    gltSetText(depthTest, depthTestBuffer);
    gltSetText(backFace, backFaceBuffer);

    gltBeginDraw();
     gltColor(1.0f, 0.0f, 0.0f, 1.0f);
     if (tog.fullDisplay) {
         gltDrawText2D(sceneName, 10, 10, 2.0);
         gltDrawText2D(resolution, 10, 50, 2.0);
         gltDrawText2D(frameRate, 10, 90, 2.0);
         gltDrawText2D(subDivisions, 10, 130, 2.0);
         gltDrawText2D(numVertices, 10, 170, 2.0);
         gltDrawText2D(numFaces, 10, 210, 2.0);
         gltDrawText2D(dataSize, 10, 250, 2.0);
         gltDrawText2D(lighting, 10, 290, 2.0);
         gltDrawText2D(depthTest, 10, 330, 2.0);
         gltDrawText2D(backFace, 10, 370, 2.0);

     }
     else {
         gltDrawText2D(frameRate, 10, 10, 2.0);
     }
    gltEndDraw();

    gltDeleteText(frameRate);
    gltDeleteText(sceneName);
    gltDeleteText(subDivisions);
    gltDeleteText(resolution);
    gltDeleteText(numVertices);
    gltDeleteText(numFaces);
    gltDeleteText(dataSize);
    gltDeleteText(lighting);
    gltDeleteText(depthTest);
    gltDeleteText(backFace);

}

void AssignmentApp::RenderFrame()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_WindowWidth, m_WindowHeight);

    RenderOSD();
    glUseProgram(0);
    glEnable(GL_DEPTH_TEST);

    CheckToggles();

    currentScene->DrawSponge(tog.Lighting, cam);
    
    // The rest of your frame rendering code goes here

    SDL_GL_SwapWindow(m_SDLWindow);
}

int AssignmentApp::Init()
{
    if (int err = RTRApp::Init() != 0) {
        return err;
    }
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(false);

    cam = new Camera(m_ScreenWidth, m_ScreenHeight);

    InitializeLights();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    gltInit();

    return 0;
}

void AssignmentApp::Done()
{
    gltTerminate();

    RTRApp::Done();
}


void AssignmentApp::CheckToggles() {

    if (tog.BackFace) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else {
        glDisable(GL_CULL_FACE);
    }

    if (tog.DepthBuffer) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }
    else {
        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    if (tog.WireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (tog.Lighting) {
        glEnable(GL_LIGHTING);
    }
    else {
        glDisable(GL_LIGHTING);
    }
}

void AssignmentApp::InitializeLights() {

    //setting up lighting

    glEnable(GL_NORMALIZE);

    //create camera light
    GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25.0f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 2);

    GLfloat lightPosition1[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat ambientLight1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight1[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 2);

    GLfloat lightPosition2[] = { -1.0, 0.0, 1.0, 1.0 };
    GLfloat ambientLight2[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight2[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight2[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 2);

    GLfloat lightPosition3[] = { 1.0, 0.0, -1.0, 1.0 };
    GLfloat ambientLight3[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight3[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight3[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight3);
    glLightfv(GL_LIGHT3, GL_SPECULAR, specularLight3);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPosition3);





    //enable the lighting
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

void AssignmentApp::ChangeNumLights(int change)
{
    if (change + g.numberOfLights > 0 && change + g.numberOfLights < 5) {
        g.numberOfLights += change;

        switch (g.numberOfLights) {
        case 1:
            glDisable(GL_LIGHT1);
            break;
        case 2:
            glEnable(GL_LIGHT1);
            glDisable(GL_LIGHT2);
            break;
        case 3:
            glEnable(GL_LIGHT2);
            glDisable(GL_LIGHT3);
            break;
        case 4:
            glEnable(GL_LIGHT3);
            break;
        }
    }
}

void AssignmentApp::ChangeRecursions(int change) {
    if (recursions > 1 || change >= 0) {
        recursions += change;

        currentScene->CreateSponge(recursions);
    }
}

static const char* MAIN_WINDOW_TITLE = "COSC1226 - Assignment 1";

int main(int argc, char** argv)
{
    AssignmentApp* app = new AssignmentApp(MAIN_WINDOW_TITLE, true);
    if (int err = app->Init() != 0) {
        std::cout << "RTR:ERROR: Failed to initialise application. Please see error messages for details..." << std::endl;
        return -1;
    };

    app->Run();
    app->Done();
    return 0;
}
