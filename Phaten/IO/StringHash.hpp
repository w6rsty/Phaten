#pragma once

#include <string_view>

namespace Pt {

class StringHash
{
public:
    StringHash() :
        m_HashValue(0)
    {
    }

    StringHash(const StringHash& hash) :
        m_HashValue(hash.m_HashValue)
    {
    }

    explicit StringHash(unsigned hash) :
        m_HashValue(hash)
    {
    }

    StringHash(std::string_view hash) :
        m_HashValue(Calculate(hash))
    {
    }

    StringHash& operator = (const StringHash& rhs)
    {
        m_HashValue = rhs.m_HashValue;
        return *this;
    }

    StringHash& operator = (std::string_view rhs)
    {
        m_HashValue = Calculate(rhs);
        return *this;
    }

    StringHash operator + (const StringHash& rhs)
    {
        StringHash ret;
        ret.m_HashValue = m_HashValue + rhs.m_HashValue;
        return ret;
    }

    StringHash& operator += (const StringHash& rhs)
    {
        m_HashValue += rhs.m_HashValue;
        return *this;
    }

    bool operator == (const StringHash& rhs) const { return m_HashValue == rhs.m_HashValue; }
    bool operator != (const StringHash& rhs) const { return m_HashValue != rhs.m_HashValue; }
    bool operator > (const StringHash& rhs) const { return m_HashValue > rhs.m_HashValue; }
    bool operator < (const StringHash& rhs) const { return m_HashValue < rhs.m_HashValue; }
    operator bool () const { return m_HashValue != 0; }

    std::string ToString() const;
    unsigned ToHash() const { return m_HashValue; }

    static unsigned Calculate(std::string_view str);

    /// Zero hash value(00000000).
    static const StringHash ZERO; 
private:
    unsigned m_HashValue;
};

} // namespace Pt