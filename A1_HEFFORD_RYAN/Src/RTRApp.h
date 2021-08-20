//-----------------------------------------------------------------------------
// RTRApp.h
//-----------------------------------------------------------------------------
// Assignment 1 Starter Code
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <sdl/SDL.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "RTRSceneBase.h"
#include "RTRScene1.h"
#include "RTRScene2.h"

class RTRApp
{
private:
    struct Colours { float red, green, blue; };

public:
    Camera* cam;
    int recursions = 1;
    glm::vec3 rotation;
    RTRSceneBase* currentScene;

    RTRSceneBase* sceneList[6];

    RTRApp(const char* title, bool fullscreen = false, int width = 1024, int height = 768);
    virtual int Init();
    virtual void Done();
    void Update();
    void Run();

    virtual bool Tick(unsigned int td_milli) {
        std::cout << "RTRApp::Tick()" << std::endl;
        return true;
    }

protected:
    std::string m_MainWindowTitle;
    bool m_FullScreen;
    int m_WindowWidth;
    int m_WindowHeight;
    int m_ScreenWidth;
    int m_ScreenHeight;
    SDL_Window* m_SDLWindow;
    SDL_Renderer* m_SDLRenderer;
    SDL_GLContext m_GLContext;
};
