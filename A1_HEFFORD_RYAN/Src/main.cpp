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
typedef struct { float frameRate, td_milliTotal, refreshRate; int frames, numberOfLights, currentBytes; bool isImmidiateMode; } Global;

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

    Global g = { 0,0,0,0,1,0, true };
    Toggles tog = { false, true, true, true, false, true };
    Movement move = { false, false, false, false, (float)m_WindowWidth/2, (float)m_WindowHeight / 2, 0.075f };

    void CheckInput();
    void UpdateState(unsigned int td_milli);
    void RenderFrame();
    void RenderOSD();
    void CheckToggles();
    void ChangeNumLights(int change);
    void ChangeRecursions(int change);
    void UpdateRefreshRate();
    std::string AddCommas(int number);
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
                currentScene->Init(cam);
                currentScene->CreateSponge(recursions);
                break;
            case SDLK_2:
                currentScene = sceneList[1];
                g.isImmidiateMode = false;
                recursions = 1;
                currentScene->Init(cam);
                currentScene->CreateSponge(recursions);
                break;
            case SDLK_3:
                currentScene = sceneList[2];
                g.isImmidiateMode = false;
                recursions = 1;
                currentScene->Init(cam);
                currentScene->CreateSponge(recursions);
                break;
            case SDLK_4:
                currentScene = sceneList[3];
                g.isImmidiateMode = false;
                recursions = 1;
                currentScene->Init(cam);
                currentScene->CreateSponge(recursions);
                break;
            case SDLK_5:
                currentScene = sceneList[4];
                g.isImmidiateMode = false;
                recursions = 1;
                currentScene->Init(cam);
                currentScene->CreateSponge(recursions);
                break;
            case SDLK_6:
                currentScene = sceneList[5];
                g.isImmidiateMode = false;
                recursions = 1;
                currentScene->Init(cam);
                currentScene->CreateSponge(recursions);
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
    UpdateRefreshRate();
    cam->CalculateMatrix();
    lightModel->UpdateDirectional(cam->GetDirection());
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

    char resolutionBuffer[40];
    snprintf(resolutionBuffer, sizeof resolutionBuffer, "Resolution: %i X %i, %i Hz", m_ScreenHeight, m_ScreenWidth, (int)roundf((g.refreshRate) * 100) / 100);

    char numVerticesBuffer[60];
    snprintf(numVerticesBuffer, sizeof numVerticesBuffer, "Vertices: %u", currentScene->vertexSize / 3);

    char numFacesBuffer[60];
    snprintf(numFacesBuffer, sizeof numFacesBuffer, "Faces: %u", currentScene->facesSize / 3);

    char dataSizeBuffer[30];
    snprintf(dataSizeBuffer, sizeof dataSizeBuffer, "Data Size: %s Bytes",  AddCommas(g.currentBytes).c_str());

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

    g.currentBytes = currentScene->DrawSponge(tog.Lighting, cam, lightModel);

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
    currentScene->Init(cam);

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
}

void AssignmentApp::ChangeNumLights(int change)
{
    if (change + g.numberOfLights > 0 && change + g.numberOfLights < 8) {
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
            glDisable(GL_LIGHT4);
            break;
        case 5:
            glEnable(GL_LIGHT4);
            glDisable(GL_LIGHT5);
            break;
        case 6:
            glEnable(GL_LIGHT5);
            glDisable(GL_LIGHT6);
            break;
        case 7:
            glEnable(GL_LIGHT6);
            glDisable(GL_LIGHT7);
            break;
        case 8:
            glEnable(GL_LIGHT7);
            break;
        }
    }
    lightModel->ChangeLightNum(change);
}

void AssignmentApp::ChangeRecursions(int change) {
    if ((recursions > 1 || change >= 0) && (recursions < 5 || change <= 0)) {
        recursions += change;

        currentScene->CreateSponge(recursions);
    }
}

void AssignmentApp::UpdateRefreshRate() {
    SDL_DisplayMode dm;
    int displayIndex = SDL_GetWindowDisplayIndex(m_SDLWindow);

    int defaultRefreshRate = 0;

    if (SDL_GetDesktopDisplayMode(displayIndex, &dm) != 0 || dm.refresh_rate == 0) {
        g.refreshRate = defaultRefreshRate;
    }
    else {
        g.refreshRate = dm.refresh_rate;
    }
}

std::string AssignmentApp::AddCommas(int number)
{
    std::string result = "";
    if (number > 0) {
        std::string strNum = std::to_string(number);
        std::string comma = ",";

        for (int i = strNum.length() - 1; i >= 0; i--) {
            std::string insertStr;
            if ((strNum.length() - i) % 3 == 0 && i > 0) {
                insertStr = strNum.at(i);
                insertStr.insert(0, comma);
            }
            else {
                insertStr = strNum.at(i);
            }
            result.insert(0, insertStr);
        }
    }
    return result;
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
