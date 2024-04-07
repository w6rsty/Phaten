#include "TextRenderer.hpp"

#include "IO/Assert.hpp"
#include "Input/Application.hpp"
#include "Resource/Image.hpp"

namespace Pt {

static constexpr std::string_view FontSheetPath = "Assets/Textures/fontsheet_rgba.png";
/// 95 printable characters. 32-126, didn't use this array actually.
static constexpr char FontSheet[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
static const Vector2 FontSize {10.0f, 12.0f};


TextRenderer::TextRenderer(const SharedPtr<ShaderProgram>& program, float scale) :
    m_LastText(""),
    m_LastPosition(Vector2::ZERO),
    m_Scale(scale),
    m_FontSize(Vector2::ZERO),
    m_Program(program),
    m_Vertices(nullptr)
{   
    Initialize();
    m_FontSize.x = 1.0f / Application::sWindowSize.x * FontSize.x * scale;
    m_FontSize.y = m_FontSize.x * Application::sWindowSize.x / Application::sWindowSize.y;
}

TextRenderer::~TextRenderer()
{
    delete[] m_Vertices;
}

void TextRenderer::SetupText(const Vector2& position, std::string_view text)
{
    if (text.size() > MAX_TEXT_SIZE)
    {
        PT_ASSERT_MSG(false, "Text size exceeds the maximum size(", MAX_TEXT_SIZE, "), got: ", text.size());
        return;
    }

    m_LastText = text;
    m_LastPosition = position; // store origin position.
    // Left corner.
    Vector2 pos = Vector2(
        2.0f * position.x / Application::sWindowSize.x - 1.0f,
        1.0f - 2.0f * position.y / Application::sWindowSize.y
    );

    int lineIndex = 0;
    int mainIndex = 0;
    while (mainIndex < text.size())
    {
        if (text[mainIndex] == '\n')
        {
            pos.y -= m_FontSize.y * 1.1f;
            mainIndex++;
            lineIndex = 0;
            continue;
        }

        float index = text[mainIndex] - FontSheet[0];
        // Vertex coordinates. Not texture coordinates.
        m_Vertices[mainIndex * 4 + 0] = Vector3(pos.x + lineIndex * m_FontSize.x, pos.y - m_FontSize.y, index);
        m_Vertices[mainIndex * 4 + 1] = Vector3(pos.x + lineIndex * m_FontSize.x + m_FontSize.x, pos.y - m_FontSize.y, index);
        m_Vertices[mainIndex * 4 + 2] = Vector3(pos.x + lineIndex * m_FontSize.x + m_FontSize.x, pos.y, index);
        m_Vertices[mainIndex * 4 + 3] = Vector3(pos.x + lineIndex * m_FontSize.x, pos.y, index);
        
        mainIndex++;
        lineIndex++;
    }

    m_TextPlane.m_VertexBuffer->SetData(0, text.size() * 4, m_Vertices);
}

void TextRenderer::Render(const Vector2 &position, Graphics *graphics, std::string_view text)
{
    if (text != m_LastText || position != m_LastPosition)
    {
        SetupText(position, text);
    }

    m_Program->Bind();
    m_FontSheetTex->Bind(0);

    m_TextPlane.Draw(graphics, text.size() * 6);
}

void TextRenderer::Initialize()
{
    // Load font sheet and create texture.    
    auto image = CreateShared<Image>();
    image->Load(FontSheetPath);

    m_FontSheetTex = CreateShared<Texture>();
    m_FontSheetTex->Define(TextureType::TEX_2D, image);
    m_FontSheetTex->SetFilterMode(TextureFilterMode::NEAREST);

    /// Allocate vertices buffer.
    m_Vertices = new Vector3[MAX_TEXT_SIZE * 4];

    // Setup index buffer.
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
    m_TextPlane.m_IndexBuffer->SetData(0, MAX_TEXT_SIZE * 6, indices);
    delete[] indices;
}

} // namespace Pt