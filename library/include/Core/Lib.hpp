#pragma once

#include "Core_Export.h"

#include <string>

struct CORE_API Lib
{
    Lib();

    std::string getName() const;

private:
    std::string m_name;
};
