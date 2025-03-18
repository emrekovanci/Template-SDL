# ---- Dependencies ----

include(Catch)
set(CMAKE_CATCH_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)

# ---- Define executable ----

add_executable(CoreTests Tests.cpp)
target_link_libraries(CoreTests PRIVATE Core Catch2::Catch2WithMain)

# ---- Tests ----

catch_discover_tests(CoreTests WORKING_DIRECTORY $<TARGET_FILE_DIR:Core>)
