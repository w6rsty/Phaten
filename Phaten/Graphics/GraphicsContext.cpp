#include "GraphicsContext.hpp"

#include <glad/glad.h>
#include <SDL.h>

#include "Core/Assert.hpp"

namespace Pt {

GraphicsContext::GraphicsContext(SDL_Window* windowHandle) :
    windowHandle(windowHandle)
{
    PT_ASSERT(windowHandle);
    if (InitWindowContext() && InitOpenGLContext())
        isValid = true;

    PT_LOG_INFO("Using OpenGL version: ", glGetString(GL_VERSION));
}

GraphicsContext::~GraphicsContext()
{
    Clean();
}

bool GraphicsContext::InitWindowContext()
{
    PT_ASSERT(windowHandle);
    contextHandle = SDL_GL_CreateContext(windowHandle);
    if (!contextHandle)
    {
        PT_LOG_FATAL("Failed to create OpenGL context: ", SDL_GetError());
        return false;
    }
    return true;
}

bool GraphicsContext::InitOpenGLContext()
{
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        PT_LOG_FATAL("Failed to initialize GLAD");
        return false;
    }
    return true;
}

void GraphicsContext::SwapBuffers()
{
    SDL_GL_SwapWindow(windowHandle);
}

void GraphicsContext::Clean()
{
    if (contextHandle)
    {
        SDL_GL_DeleteContext(contextHandle);
        contextHandle = nullptr;
    }
}

} // namespace Pt