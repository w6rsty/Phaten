// TODO: Support text color.
#pragma once

#include "Object/Ptr.hpp"

#include "Graphics/Graphics.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/ShaderProgram.hpp"

#include "StaticGeometry.hpp"

namespace Pt {

/// Text batch renderer.
class TextRenderer : public Object
{
    OBJECT(TextRenderer);
public:
    TextRenderer(const SharedPtr<ShaderProgram>& program, float scale = 2.0f, float spacing = 0.2f);
    ~TextRenderer();
    // Render text.(Pixel position)
    void Render(const Vector2& position, Graphics* graphics, std::string_view text);
private:
    /// Setup vertices data of text.
    // For static text, you and pre setup the text and position.
    void SetupText(const Vector2& position, std::string_view text);
    /// Initialize renderer.
    void Initialize();

    std::string m_LastText;
    Vector2 m_LastPosition;
    float m_Scale;
    float m_Spacing;
    Vector2 m_FontSize;

    SharedPtr<ShaderProgram> m_Program;
    SharedPtr<Texture> m_FontSheetTex;

    internal::TextPlane m_TextPlane;
    Vector3* m_Vertices;
};

} // namespace Pt