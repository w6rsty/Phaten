#pragma once

#include <string>
#include <string_view>

#include "IO/StringHash.hpp"

namespace Pt {

/// Base class for resources
class Resource
{
public:
    virtual bool BeginLoad() = 0;
    virtual bool EndLoad();

    bool Load();
    void SetName(std::string_view newName);

    std::string_view Name() const { return m_Name; }
    StringHash NameHash() const { return m_NameHash; }
private:
    std::string m_Name;
    StringHash m_NameHash;
};

} // namespace Pt