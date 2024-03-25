#include "Resource.hpp"

#include "IO/Assert.hpp"

namespace Pt {

bool Resource::BeginLoad()
{
    PT_ASSERT_MSG(false, "Do not use base BeginLoad");
    return false;
}

bool Resource::EndLoad()
{
    
    return true;
}

bool Resource::Save()
{
    PT_ASSERT_MSG(false, "Do not use base Save");
    return false;
}

bool Resource::Load()
{
    bool success = BeginLoad();
    if (success)
    {
        success &= EndLoad();
    }

    return success;
}

void Resource::SetName(std::string_view newName)
{
    m_Name = std::string(newName);
    m_NameHash = StringHash(newName.data());
}

} // namespace Pt