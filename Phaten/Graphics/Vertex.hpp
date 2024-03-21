#pragma once

#include <vector>

#include "Core/Logger.hpp"
#include "GraphicsDefs.hpp"

namespace Pt {

struct VertexElement
{

    VertexElement() :
        type(VertexElementType::FLOAT3),
        semantic(VertexElementSemantic::POSITION),
        offset(0)
    {
    }

    VertexElement(VertexElementType type_, VertexElementSemantic semantic_) :
        type(type_),
        semantic(semantic_),
        offset(0)
    {
    }

    VertexElementType type;
    VertexElementSemantic semantic;
    size_t offset;
};

/// Vertex elements layout.
/// 0 POSITION.
/// 1 NORMAL.
/// 2 TANGENT.
/// 3 VERTEX_COLOR.
/// 4 TEXCOORD.
class VertexLayout
{
public:
    VertexLayout() = default;
    VertexLayout(const std::vector<VertexElement>& elements) :
        m_Elements(elements), m_Stride(0)
    {
        CalculateStride();
    }
    VertexLayout(std::initializer_list<VertexElement> elements) :
        m_Elements(elements), m_Stride(0)
    {
        CalculateStride();
    }

    size_t Stride() const { return m_Stride; } 

    std::vector<VertexElement>::const_iterator begin() const { return m_Elements.begin(); }
    std::vector<VertexElement>::const_iterator end() const { return m_Elements.end(); }

    operator bool () const { return !m_Elements.empty(); }
private:
    void CalculateStride()
    {
        size_t stride = 0;
        for (VertexElement& element : m_Elements)
        {
            element.offset = stride;
            stride += VertexElementSize[static_cast<size_t>(element.type)];
        }
        m_Stride = stride;
    }

    std::vector<VertexElement> m_Elements;
    size_t m_Stride;
};

} // namespace Pt