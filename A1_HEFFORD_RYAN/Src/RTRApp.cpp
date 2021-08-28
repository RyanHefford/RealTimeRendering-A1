//-----------------------------------------------------------------------------
// RTRApp.cpp
//-----------------------------------------------------------------------------
// Assignment 1 Starter Code
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include "RTRApp.h"
#define STB_IMAGE_IMPLEMENTATION

//void GLAPIENTRY
//MessageCallback(GLenum source,
//    GLenum type,
//    GLuint id,
//    GLenum severity,
//    GLsizei length,
//    const GLchar* message,
//    const void* userParam)
//{
//    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
//        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
//        type, severity, message);
//}

RTRApp::RTRApp(const char* title, bool fullscreen, int width, int height)
{
    m_MainWindowTitle = title;
    m_FullScreen = fullscreen;
    m_WindowWidth = width;
    m_WindowHeight = height;
    m_ScreenWidth = 0;
    m_ScreenHeight = 0;
    m_SDLWindow = nullptr;
    m_SDLRenderer = nullptr;
    m_GLContext = nullptr;

    //create all scenes
    sceneList[0] = new RTRScene1(lightModel);
    sceneList[1] = new RTRScene2();
    sceneList[2] = new RTRScene3();
    sceneList[3] = new RTRScene4();
    sceneList[4] = new RTRScene5();
    sceneList[5] = new RTRScene6();

    currentScene = sceneList[0];
    currentScene->CreateSponge(1);

    recursions = 1;
}

int RTRApp::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "RTR:ERROR: SDL2 video subsystem couldn't be initialized. Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (m_FullScreen == true) {
        m_SDLWindow = SDL_CreateWindow(
            m_MainWindowTitle.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            0, 0,
            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    }
    else {
        m_SDLWindow = SDL_CreateWindow(
            m_MainWindowTitle.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            m_WindowWidth, m_WindowHeight,
            SDL_WINDOW_OPENGL);
    }

    m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
    if (m_SDLRenderer == nullptr) {
        std::cerr << "RTR:ERROR: SDL2 Renderer couldn't be created. Error: " << SDL_GetError() << std::endl;
        return -2;
    }

    m_GLContext = SDL_GL_CreateContext(m_SDLWindow);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "RTR:ERROR: Failed to initialize the OpenGL context." << std::endl;
        return -3;
    }

    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) == 0) {
        m_ScreenWidth = dm.w;
        m_ScreenHeight = dm.h;
        if (m_FullScreen == true) {
            m_WindowWidth = m_ScreenWidth;
            m_WindowHeight = m_ScreenHeight;
        }
    }
    else {
        std::cerr << "RTR:WARNING: SDL coudn't retrieve current display mode details." << std::endl;
    }


    std::cout << "RTR:MESSAGE: OpenGL version " << GLVersion.major << "." << GLVersion.minor << " initialised." << std::endl;

    /*glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);*/

    return 0;
}

void RTRApp::Run()
{
    bool quit_app = false;

    while (quit_app == false) {

        static unsigned int last_time = 0;
        unsigned int cur_time = SDL_GetTicks();
        unsigned int td_milli = cur_time - last_time;
        last_time = cur_time;
        quit_app = Tick(td_milli);
    }
}

void RTRApp::Done()
{
    SDL_GL_DeleteContext(m_GLContext);
    SDL_DestroyRenderer(m_SDLRenderer);
    SDL_DestroyWindow(m_SDLWindow);
    SDL_Quit();
    m_SDLWindow = nullptr;
    m_SDLRenderer = nullptr;
    m_GLContext = nullptr;
}

void RTRApp::Update() {
    
}