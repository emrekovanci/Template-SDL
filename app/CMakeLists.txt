# ---- Define executable ----

add_executable(Application
    App.cpp
    Common.hpp Common.cpp
    AppContext.hpp
    Vertex.hpp
    PipelineWatcherContext.hpp PipelineWatcherContext.cpp
    PipelineReloader.hpp PipelineReloader.cpp
)
target_link_libraries(Application PRIVATE Core)

find_package(efsw CONFIG REQUIRED)
target_link_libraries(Application PRIVATE efsw::efsw)

target_link_libraries(Application PRIVATE "${CMAKE_SOURCE_DIR}/vendor/shadercross/lib/SDL3_shadercross.lib")
target_include_directories(Application PRIVATE "${CMAKE_SOURCE_DIR}/vendor/shadercross/include")

find_package(glm CONFIG REQUIRED)
target_link_libraries(Application PRIVATE glm::glm)

find_package(SDL3_image CONFIG REQUIRED)
target_link_libraries(Application PRIVATE SDL3_image::SDL3_image)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(Application PRIVATE SDL3::SDL3)

set_compiler_warnings(Application)
set_linker_flags(Application)
install(TARGETS Application)

add_custom_command(
    TARGET Application POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Application>/assets
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/vendor/shadercross/bin/SDL3_shadercross.dll $<TARGET_FILE_DIR:Application>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/vendor/shadercross/bin/spirv-cross-c-shared.dll $<TARGET_FILE_DIR:Application>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/vendor/shadercross/bin/dxcompiler.dll $<TARGET_FILE_DIR:Application>
)
