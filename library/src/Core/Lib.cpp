#include <Core/Lib.hpp>

Lib::Lib() :
    m_name { "Library" }
{
}

std::string Lib::getName() const
{
    return m_name;
}
