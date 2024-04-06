#include "Text.hpp"

#include "IO/Assert.hpp"
#include "Math/Vector.hpp"
#include "Resource/Image.hpp"
#include "Graphics/GraphicsDefs.hpp"

namespace Pt {

/// 95 printable characters.
static constexpr char CharSheet[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
static constexpr std::string_view fontSheetPath = "Assets/Textures/fontsheet.png";
static constexpr size_t MAX_TEXT_SIZE = 256;

TextRenderer::TextRenderer(const SharedPtr<ShaderProgram>& program) :
    m_Program(program),
    m_LastText(""),
    m_LastPosition(IntV2::ZERO),
    m_Vertices(nullptr)
{
    Initialize();
}

void TextRenderer::SetupText(std::string_view text, const IntV2& position)
{
    if (text.size() > MAX_TEXT_SIZE)
    {
        PT_ASSERT_MSG(false, "Text size exceeds the maximum size(", MAX_TEXT_SIZE, "), got: ", text.size());
        return;
    }

    m_LastText = text;
    m_LastPosition = position;

    Vector2 pos = Vector2(position.x / 1280.0f, position.y / 720.0f);
    // Text position start from left-bottom corner.
    pos = pos * 2.0f - Vector2(1.0f, 1.0f);

    float spacing = 1.0f / 40.0f;
    for (int i = 0; i < text.size(); ++i)
    {
        float index = text[i] - CharSheet[0];
        // Vertex coordinates. Not texture coordinates.
        m_Vertices[i * 4 + 0] = Vector3(pos.x + i * spacing, pos.y, index);
        m_Vertices[i * 4 + 1] = Vector3(pos.x + i * spacing + spacing, pos.y, index);
        m_Vertices[i * 4 + 2] = Vector3(pos.x + i * spacing + spacing, pos.y + spacing, index);
        m_Vertices[i * 4 + 3] = Vector3(pos.x + i * spacing, pos.y + spacing, index);
    }

    m_TextPlane.m_VertexBuffer->SetData(0, text.size() * 4, m_Vertices);
}

void TextRenderer::Render(Graphics *graphics, std::string_view text, const IntV2 &position)
{
    if (text != m_LastText || position != m_LastPosition)
    {
        SetupText(text, position);
    }

    m_Program->Bind();
    m_FontSheet->Bind(0);

    m_TextPlane.Draw(graphics, text.size() * 6);
}

void TextRenderer::Initialize()
{
    auto image = CreateShared<Image>();
    image->Load(fontSheetPath);

    m_FontSheet = CreateShared<Texture>();
    m_FontSheet->Define(TextureType::TEX_2D, image);
    m_FontSheet->SetFilterMode(TextureFilterMode::NEAREST);

    m_Vertices = new Vector3[MAX_TEXT_SIZE * 4];

    unsigned*  indices = new unsigned[MAX_TEXT_SIZE * 6];
    unsigned offset = 0;
    for (int i = 0; i < MAX_TEXT_SIZE; ++i)
    {
        indices[i * 6 + 0] = offset + 0;
        indices[i * 6 + 1] = offset + 1;
        indices[i * 6 + 2] = offset + 2;
        indices[i * 6 + 3] = offset + 2;
        indices[i * 6 + 4] = offset + 3;
        indices[i * 6 + 5] = offset + 0;

        offset += 4;
    }
    m_TextPlane.m_IndexBuffer->SetData(0, 256 * 4, indices);
    delete[] indices;
}

} // namespace Pt