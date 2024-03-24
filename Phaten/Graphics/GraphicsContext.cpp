#include "GraphicsContext.hpp"

#include <glad/glad.h>
#include <SDL.h>

#include "Core/Assert.hpp"

namespace Pt {

GraphicsContext::GraphicsContext(SDL_Window* windowHandle) :
    m_WindowHandle(windowHandle),
    m_GLContextHandle(nullptr),
    m_IsValid(false)
{
    if (!InitWindowContext())
    {
        PT_ASSERT_MSG(false, "Failed to create window context");
    } 
    if (!InitOpenGLContext())
    {
        PT_ASSERT_MSG(false, "Failed to create OpenGL context");
    }

    m_IsValid = true;

    PT_LOG_INFO("OpenGL version: ", glGetString(GL_VERSION));
}

GraphicsContext::~GraphicsContext()
{
    Release();
}

bool GraphicsContext::InitWindowContext()
{
    PT_ASSERT_MSG(m_WindowHandle, "Window handle is null");
    m_GLContextHandle = SDL_GL_CreateContext(m_WindowHandle);
    if (!m_GLContextHandle)
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

void GraphicsContext::Release()
{
    if (m_GLContextHandle)
    {
        SDL_GL_DeleteContext(m_GLContextHandle);
        m_GLContextHandle = nullptr;
    }
}

} // namespace Pt