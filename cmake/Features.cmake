option(FEATURE_TESTS "Enable the tests" OFF)
if(FEATURE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

if(PROJECT_IS_TOP_LEVEL)
    option(BUILD_SHARED_LIBS "Build shared libs." OFF)
endif()
