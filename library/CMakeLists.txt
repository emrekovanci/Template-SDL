# ---- Define library ----

add_library(Core
    include/Core/Lib.hpp
    src/Core/Lib.cpp
)
target_include_directories(Core PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/library/include>")
set_target_warnings(Core)
set_linker_flags(Core)

include(GenerateExportHeader)
generate_export_header(Core
    EXPORT_FILE_NAME Core_Export.h
    EXPORT_MACRO_NAME CORE_API
    NO_EXPORT_MACRO_NAME CORE_PRIVATE
)
target_sources(Core
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
        FILES ${CMAKE_CURRENT_BINARY_DIR}/Core_Export.h
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(Core PUBLIC CORE_STATIC_DEFINE)
endif()

set_target_properties(Core PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
)

install(TARGETS Core)

if (FEATURE_TESTS)
    add_subdirectory(test)
endif()
