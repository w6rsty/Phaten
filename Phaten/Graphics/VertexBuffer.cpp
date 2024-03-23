#include "VertexBuffer.hpp"

#include <glad/glad.h>

#include "Core/Logger.hpp"

namespace Pt {

static unsigned boundVertexAttributes  = 0;
static VertexBuffer* boundVertexBuffer = nullptr;
/// Pointer to vertex buffer whose attributes are being using.
static VertexBuffer* boundVertexAttributeSource = nullptr;

VertexBuffer::VertexBuffer() :
    m_Handle(0),
    m_Usage(BufferUsage::STATIC),
    m_NumVertices(0),
    m_Attributes(0)
{
    // TODO: Ensure graphics system loaded.
}

VertexBuffer::~VertexBuffer()
{
    // TODO: Ensure graphics system loaded.
    Release();    
}

bool VertexBuffer::Define(BufferUsage usage, size_t numVertices, const VertexLayout &layout, const void* data)
{
    // Reset 
    Release();

    m_Usage = usage;
    m_NumVertices = numVertices;
    m_Layout = layout;

    // Read layout and set enabled vertex attributes
    if (m_Layout)
    {
        m_Attributes = CalculateAttributesMask(m_Layout);
    }
    
    return Create(data);
}

bool VertexBuffer::SetData(size_t startIdx, size_t numbVertices, const void *data, bool discard)
{
    if (!m_Handle)
    {
        PT_LOG_ERROR("Index buffer has not been created");
        return false;
    }
    if (data)
    {   
        PT_LOG_ERROR("Vertex data is null, you fool!");
        return false;
    }
    if (startIdx + numbVertices > m_NumVertices)
    {
        PT_LOG_ERROR("Invalid range of vertices");
        return false;
    }

    Bind(0);

    GLsizeiptr totalVertexSize = numbVertices * m_Layout.Stride();
    GLenum usage = m_Usage == BufferUsage::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    if (numbVertices == m_NumVertices)
    {
        glBufferData(GL_ARRAY_BUFFER, totalVertexSize, data, usage);
    }
    else
    {
        if (discard)
        {
            glBufferData( GL_ARRAY_BUFFER, totalVertexSize, nullptr, usage);
        }

        glBufferSubData(
            GL_ARRAY_BUFFER,
            startIdx * m_Layout.Stride(),
            numbVertices * m_Layout.Stride(), 
            data
        );
    }

    return true;
}

void VertexBuffer::Bind(unsigned int attributeMask)
{
    if (!m_Handle)
    {
        return;
    }
    // This 0 condition is only for creating vertex buffer
    // bescase glBufferData needs this buffer being bound. 
    // Just bind and return.
    if (!attributeMask)
    {
        if (boundVertexBuffer != this)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
            boundVertexBuffer = this;
        }
        return;
    }

    // Filter attributes which don't have.
    attributeMask &= m_Attributes;

    // Return if has been bound
    if (attributeMask == boundVertexAttributes 
        && boundVertexAttributeSource == this)
    {
        return;
    }
    // Ensure binding for later operations.
    if (boundVertexBuffer != this)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
        boundVertexBuffer = this;
    }

    unsigned usedAttributes = 0;

    for (const VertexElement& element : m_Layout)
    {
        unsigned attributeIdx = VertexAttributeIdx[static_cast<size_t>(element.semantic)];
        unsigned attributeBit = 1 << attributeIdx;
        // Ignore unused attribute.
        if (!(attributeMask & attributeBit))
        {
            continue; // Ingored here.
        }

        /// Enable attribute if not has been set before
        if (!(boundVertexAttributes & attributeBit))
        {
            glEnableVertexAttribArray(attributeIdx);
        }

        size_t typeAsIdx = static_cast<size_t>(element.type);
        glVertexAttribPointer(
            attributeIdx,
            VertexElementGLCount[typeAsIdx],
            VertexElementGLType[typeAsIdx],
            element.semantic == VertexElementSemantic::VERTEX_COLOR ? GL_TRUE : GL_FALSE,
            m_Layout.Stride(),
            reinterpret_cast<void*>(element.offset)
        );

        usedAttributes |= attributeBit; // Only contains enabled atrributes.
    }

    // Disable unused attributes.
    unsigned disabledAttributes = boundVertexAttributes & (~usedAttributes); // Bits.
    unsigned disableIdx = 0; // Decimal.

    // Traverse
    while (disabledAttributes)
    {
        // Use lowest bit to do and operation with 1.
        if (disabledAttributes & 1)
        {
            glDisableVertexAttribArray(disableIdx);
        }
        disabledAttributes >>= 1;
        ++disableIdx;
    }

    // Update bound state.
    boundVertexAttributes = usedAttributes;
    boundVertexBuffer = this;
    boundVertexAttributeSource = this;
}

// Called by Define() only.
bool VertexBuffer::Create(const void* data)
{
    glGenBuffers(1, &m_Handle);
    if (!m_Handle)
    {
        PT_LOG_ERROR("Failed to create vertex buffer");
        return false;
    }

    Bind(0);

    glBufferData(
        GL_ARRAY_BUFFER,
        m_NumVertices * m_Layout.Stride(),
        data,
        m_Usage == BufferUsage::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
    );
    PT_LOG_INFO("Created vertex buffer(NumVertices: ", m_NumVertices, " VertexSize: ", m_Layout.Stride(), ")");

    return true;
}

void VertexBuffer::Release()
{
    if (m_Handle)
    {
        glDeleteBuffers(1, &m_Handle);
        m_Handle = 0;

        if (boundVertexBuffer == this)
        {
            boundVertexBuffer = nullptr;
        }
        if (boundVertexAttributeSource == this)
        {
            boundVertexAttributeSource = nullptr;
        }
    }
}

/// Static function, calculate attribute mask of a vertex layout.
unsigned VertexBuffer::CalculateAttributesMask(const VertexLayout &layout)
{
    unsigned attributes = 0;
    for (const VertexElement& element : layout)
    {
        unsigned attributeBit = 1 << VertexAttributeIdx[static_cast<size_t>(element.semantic)];
        attributes |= attributeBit;
    }
    return attributes;
}

} // namespace Pt