function(set_linker_flags target)
    if (MSVC)
        set(PROJECT_LINK_OPTIONS /machine:x64 /guard:cf)
    elseif (NOT APPLE AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
        set(PROJECT_LINK_OPTIONS -Wl,--allow-shlib-undefined,--as-needed,-z,noexecstack,-z,relro,-z,now,-z,nodlopen)
    endif()

    target_link_options(${target} PRIVATE ${PROJECT_LINK_OPTIONS})
endfunction()
