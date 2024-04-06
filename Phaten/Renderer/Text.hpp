#pragma once

#include "Object/Ptr.hpp"

#include "Graphics/Graphics.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/VertexBuffer.hpp"

#include "Math/IntVector.hpp"
#include "Math/Vector.hpp"

#include "StaticGeometry.hpp"

namespace Pt {

class TextRenderer : RefCounted
{
public:
    TextRenderer(const SharedPtr<ShaderProgram>& program);
    void SetupText(std::string_view text, const IntV2& position);
    void Render(Graphics* graphics, std::string_view text, const IntV2& position);
private:
    void Initialize();

    std::string m_LastText;
    IntV2 m_LastPosition;

    SharedPtr<ShaderProgram> m_Program;
    SharedPtr<Texture> m_FontSheetTex;

    internal::TextPlane m_TextPlane;
    Vector3* m_Vertices;
};

} // namespace Pt