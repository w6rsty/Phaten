#pragma once

namespace Pt {

class Texture
{
public:
    Texture();
    ~Texture();
    unsigned GLHandle() const { return m_Handle; }
private:
    unsigned m_Handle;
};

} // namespace Pt