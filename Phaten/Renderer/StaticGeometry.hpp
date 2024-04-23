#pragma once

#include "Object/Ptr.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Resource/Mesh/BasicMesh.hpp"

namespace Pt {

class Plane
{   
public:
    Plane(const Matrix4& model = Matrix4::IDENTITY, const Vector2& scale = Vector2(1.0f, 1.0f)) :
        m_Model(model),
        m_Graphics(nullptr)
    {
        PT_ASSERT_MSG(Object::Subsystem<Graphics>()->IsInitialized(), "Graphics system not loaded");
        m_Graphics = Object::Subsystem<Graphics>();
        
        auto layout = VertexLayout {
            {VertexElementType::FLOAT3, VertexElementSemantic::POSITION}, // 0
            {VertexElementType::FLOAT3, VertexElementSemantic::NORMAL},   // 1
            {VertexElementType::FLOAT2, VertexElementSemantic::TEX_COORD}, // 4
        };

        float* data = new float[PlaneMesh::VertexCount * layout.Stride()];
        memcpy(data, PlaneMesh::Vertices, PlaneMesh::VertexCount * layout.Stride());

        if (scale != Vector2(1.0f, 1.0f))
        {
            // modify texcoord
            for (size_t i = 0; i < PlaneMesh::VertexCount; ++i)
            {
                data[i * 8 + 3 * 2 + 0] *= scale.x;
                data[i * 8 + 3 * 2 + 1] *= scale.y;
            }
        }

        m_VertexBuffer = CreateShared<VertexBuffer>();
        m_VertexBuffer->Define(BufferUsage::STATIC, PlaneMesh::VertexCount,
            layout,
            data
        );

        delete[] data;

        m_IndexBuffer = CreateShared<IndexBuffer>();
        m_IndexBuffer->Define(BufferUsage::STATIC, PlaneMesh::IndexCount, PlaneMesh::Indices);
    }

    void Draw(const SharedPtr<ShaderProgram>& program)
    {
        program->Bind();
        m_Graphics->SetUniform(program, PresetUniform::U_MODEL, m_Model);

        m_VertexBuffer->Bind(m_VertexBuffer->Attributes());
        m_IndexBuffer->Bind();

        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, m_IndexBuffer->NumIndices());
    }

    SharedPtr<VertexBuffer> m_VertexBuffer;
    SharedPtr<IndexBuffer> m_IndexBuffer;

    Matrix4 m_Model;
    Graphics* m_Graphics;
};

namespace internal {
/// Only used by TextRenderer.
class TextPlane
{
public:
    TextPlane() :
        m_Graphics(nullptr)
    {
        PT_ASSERT_MSG(Object::Subsystem<Graphics>()->IsInitialized(), "Graphics system not loaded");
        m_Graphics = Object::Subsystem<Graphics>();

        m_VertexBuffer = CreateShared<VertexBuffer>();
        m_VertexBuffer->Define(BufferUsage::DYNAMIC, MAX_TEXT_SIZE * 4, 
            VertexLayout{ // use z as font index.
                {VertexElementType::FLOAT3, VertexElementSemantic::POSITION}
            },
            nullptr
        );

        m_IndexBuffer = CreateShared<IndexBuffer>();
        m_IndexBuffer->Define(BufferUsage::STATIC, MAX_TEXT_SIZE * 6, nullptr);
    }

    void Draw(size_t count)
    {
        m_VertexBuffer->Bind(m_VertexBuffer->Attributes());
        m_IndexBuffer->Bind();

        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, count);
    }

    SharedPtr<VertexBuffer> m_VertexBuffer;
    SharedPtr<IndexBuffer> m_IndexBuffer;
    Graphics* m_Graphics;
};
}

class ScreenPlane
{
public:
    ScreenPlane() :
        m_Graphics(nullptr)
    {
        PT_ASSERT_MSG(Object::Subsystem<Graphics>()->IsInitialized(), "Graphics system not loaded");
        m_Graphics = Object::Subsystem<Graphics>();

        m_VertexBuffer = CreateShared<VertexBuffer>();
        m_VertexBuffer->Define(BufferUsage::STATIC, ScreenPlaneMesh::VertexCount,
            VertexLayout{
                {VertexElementType::FLOAT3, VertexElementSemantic::POSITION}, // 0
                {VertexElementType::FLOAT2, VertexElementSemantic::TEX_COORD}, // 4
            },
            ScreenPlaneMesh::Vertices
        );

        m_IndexBuffer = CreateShared<IndexBuffer>();
        m_IndexBuffer->Define(BufferUsage::STATIC, ScreenPlaneMesh::IndexCount, ScreenPlaneMesh::Indices);
    }

    void Draw(const SharedPtr<ShaderProgram>& program)
    {
        program->Bind();

        m_VertexBuffer->Bind(m_VertexBuffer->Attributes());
        m_IndexBuffer->Bind();

        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, m_IndexBuffer->NumIndices());
    }

    SharedPtr<VertexBuffer> m_VertexBuffer;
    SharedPtr<IndexBuffer> m_IndexBuffer;

    Graphics* m_Graphics;
};

class Cube
{
public:
    Cube(const Matrix4& model = Matrix4::IDENTITY) :
        m_Model(model),
        m_Graphics(nullptr)
    {
        PT_ASSERT_MSG(Object::Subsystem<Graphics>()->IsInitialized(), "Graphics system not loaded");
        m_Graphics = Object::Subsystem<Graphics>();

        m_VertexBuffer = CreateShared<VertexBuffer>();
        m_VertexBuffer->Define(BufferUsage::STATIC, CubeMesh::VertexCount,
            VertexLayout{
                {VertexElementType::FLOAT3, VertexElementSemantic::POSITION}, // 0
                {VertexElementType::FLOAT3, VertexElementSemantic::NORMAL},   // 1
                {VertexElementType::FLOAT2, VertexElementSemantic::TEX_COORD}, // 4
            },
            CubeMesh::Vertices
        );

        m_IndexBuffer = CreateShared<IndexBuffer>();
        m_IndexBuffer->Define(BufferUsage::STATIC, CubeMesh::IndexCount, CubeMesh::Indices);
    }

    void Draw(const SharedPtr<ShaderProgram>& program)
    {
        program->Bind();
        m_Graphics->SetUniform(program, PresetUniform::U_MODEL, m_Model);

        m_VertexBuffer->Bind(m_VertexBuffer->Attributes());
        m_IndexBuffer->Bind();

        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, m_IndexBuffer->NumIndices());
    }

    SharedPtr<VertexBuffer> m_VertexBuffer;
    SharedPtr<IndexBuffer> m_IndexBuffer;

    Matrix4 m_Model;
    Graphics* m_Graphics;
};

} // namespace Pt