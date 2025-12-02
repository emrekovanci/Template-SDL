# detect the compiler
# Note: The detection is order is important because:
# - Visual Studio can both use MSVC and Clang
# - GNUCXX can still be set on macOS when using Clang
if(MSVC)
    set(PROJECT_COMPILER_MSVC 1)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(PROJECT_COMPILER_CLANG_CL 1)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(PROJECT_COMPILER_CLANG 1)

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-v" OUTPUT_VARIABLE CLANG_COMPILER_VERSION ERROR_VARIABLE CLANG_COMPILER_VERSION)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(PROJECT_COMPILER_GCC 1)

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-v" OUTPUT_VARIABLE GCC_COMPILER_VERSION ERROR_VARIABLE GCC_COMPILER_VERSION)
    string(REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" PROJECT_COMPILER_GCC_TDM "${GCC_COMPILER_VERSION}")
else()
    message(WARNING "Unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}. Use at your own risk.")
endif()
