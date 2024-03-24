#include "Graphics.hpp"

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Core/Assert.hpp"

namespace Pt {

Graphics::Graphics(WindowCreateInfo windowInfo)
{
    // TODO: Add another struct to store window properties.
    m_Window = CreateScoped<Window>(windowInfo.title, windowInfo.windowSize, windowInfo.mode);
    // Use the window handle to create the graphics context.
    m_GraphicsContext = CreateScoped<GraphicsContext>(m_Window->NativeHandle());

    // Create a default VAO and use forever :D
    // (I'd like to switch to vulkan soon)
    unsigned defaultVAO;
    glGenVertexArrays(1, &defaultVAO);
    glBindVertexArray(defaultVAO);

    // Initialization Done ====================================================
    
}

void Graphics::Present()
{
    SDL_GL_SwapWindow(m_GraphicsContext->m_WindowHandle);
}

} // namespace Pt