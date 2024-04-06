#pragma once

#include "Graphics/GraphicsDefs.hpp"
#include "Object/Ptr.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Math/Matrix.hpp"
#include "Resource/Mesh/BasicMesh.hpp"
#include <cstddef>

namespace Pt {

class Plane
{
public:
    Plane(const SharedPtr<ShaderProgram>& program, const Matrix4& model = Matrix4::IDENTITY) :
        m_Model(model)
    {
        m_VertexBuffer = CreateShared<VertexBuffer>();
        m_VertexBuffer->Define(BufferUsage::STATIC, PlaneMesh::VertexCount,
            VertexLayout{
                {VertexElementType::FLOAT3, VertexElementSemantic::POSITION}, // 0
                {VertexElementType::FLOAT3, VertexElementSemantic::NORMAL},   // 1
                {VertexElementType::FLOAT2, VertexElementSemantic::TEX_COORD}, // 4
            },
            PlaneMesh::Vertices
        );

        m_IndexBuffer = CreateShared<IndexBuffer>();
        m_IndexBuffer->Define(BufferUsage::STATIC, PlaneMesh::IndexCount, PlaneMesh::Indices);

        m_Program = program;
    }

    void Draw(Graphics* graphics)
    
    {
        m_Program->Bind();
        graphics->SetUniform(m_Program, PresetUniform::U_MODEL, m_Model);

        m_VertexBuffer->Bind(m_VertexBuffer->Attributes());
        m_IndexBuffer->Bind();

        graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, m_IndexBuffer->NumIndices());
    }
private:
    SharedPtr<VertexBuffer> m_VertexBuffer;
    SharedPtr<IndexBuffer> m_IndexBuffer;
    SharedPtr<ShaderProgram> m_Program;

    Matrix4 m_Model;
};

namespace internal {
class TextPlane
{
public:
    TextPlane() 
    {
        m_VertexBuffer = CreateShared<VertexBuffer>();
        m_VertexBuffer->Define(BufferUsage::DYNAMIC, 256, 
            VertexLayout{ // use z as font index.
                {VertexElementType::FLOAT3, VertexElementSemantic::POSITION}
            },
            nullptr
        );

        m_IndexBuffer = CreateShared<IndexBuffer>();
        m_IndexBuffer->Define(BufferUsage::STATIC, 256 * 6, nullptr);
    }

    void Draw(Graphics* graphics, size_t count)
    {
        // Enable only position and texcoord.
        m_VertexBuffer->Bind(m_VertexBuffer->Attributes());
        m_IndexBuffer->Bind();

        graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, count);
    }

    SharedPtr<VertexBuffer> m_VertexBuffer;
    SharedPtr<IndexBuffer> m_IndexBuffer;
};
}

class Cube
{
public:
    Cube(const SharedPtr<ShaderProgram>& program, const Matrix4& model = Matrix4::IDENTITY) :
        m_Model(model)
    {
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

        m_Program = program;
    }

    void Draw(Graphics* graphics)
    {
        m_Program->Bind();
        graphics->SetUniform(m_Program, PresetUniform::U_MODEL, m_Model);

        m_VertexBuffer->Bind(m_VertexBuffer->Attributes());
        m_IndexBuffer->Bind();

        graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, m_IndexBuffer->NumIndices());
    }
private:
    SharedPtr<VertexBuffer> m_VertexBuffer;
    SharedPtr<IndexBuffer> m_IndexBuffer;
    SharedPtr<ShaderProgram> m_Program;

    Matrix4 m_Model;
};

} // namespace Pt