function(set_compiler_warnings target)
    if(MSVC)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(PROJECT_WARNINGS
                -Wall
                -Wextra
                -Wshadow
                -Wpedantic
                -Wunused
                -Wformat=2
                -Wnull-dereference
                -Wnon-virtual-dtor
                -Woverloaded-virtual
                -Wold-style-cast
                -Wno-missing-prototypes
                -Wno-switch-enum
                -Wno-missing-designated-field-initializers
                -Wno-c++98-compat
                -Wno-c++98-compat-pedantic
                /EHsc)
        else()
            # Note that all the flags after /W4 are required for MSVC to conform to the language standard
            set(PROJECT_WARNINGS
                    /guard:cf
                    /utf-8
                    /diagnostics:caret
                    /sdl
                    /W4
                    /w14165 # 'HRESULT' is being converted to 'bool'; are you sure this is what you want?
                    /w44242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
                    /w44254 # 'operator': conversion from 'type1':'field_bits' to 'type2':'field_bits', possible loss of data
                    /w44263 # 'function' : member function does not override any base class virtual member function
                    /w34265 # 'classname': class has virtual functions, but its non-trivial destructor is not virtual; instances of this class may not be destructed correctly
                    /w34287 # 'operator' : unsigned/negative constant mismatch
                    /w44296 # 'operator' : expression is always false
                    /w44388 # 'token' : signed/unsigned mismatch
                    /w44464 # relative include path contains '..'
                    /w14545 # expression before comma evaluates to a function which is missing an argument list
                    /w14546 # function call before comma missing argument list
                    /w14547 # 'operator' : operator before comma has no effect; expected operator with side-effect
                    /w14549 # 'operator' : operator before comma has no effect; did you intend 'operator'?
                    /w14555 # expression has no effect; expected expression with side-effect
                    /w34619 # #pragma warning : there is no warning number 'number'
                    /w34640 # 'instance' : construction of local static object is not thread-safe
                    /w24826 # Conversion from 'type' to 'type' is sign-extended. This may cause unexpected runtime behavior.
                    /w14905 # wide string literal cast to 'LPSTR'
                    /w14906 # string literal cast to 'LPWSTR'
                    /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
                    /w45038 # data member 'member1' will be initialized after data member 'member2', data member 'member' will be initialized after base class 'base_class'
                    /permissive-
                    /volatile:iso
                    /Zc:inline
                    /Zc:preprocessor
                    /Zc:enumTypes
                    /Zc:lambda
                    /Zc:__cplusplus
                    /Zc:externConstexpr
                    /Zc:throwingNew
                    /EHsc)
        endif()
    elseif(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(PROJECT_WARNINGS
            -fstack-protector-strong
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wcast-qual
            -Wformat=2
            -Wundef
            -Werror=float-equal
            -Wshadow
            -Wcast-align
            -Wunused
            -Wnull-dereference
            -Wdouble-promotion
            -Wimplicit-fallthrough
            -Wextra-semi
            -Woverloaded-virtual
            -Wnon-virtual-dtor
            -Wold-style-cast
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(PROJECT_WARNINGS
            -U_FORTIFY_SOURCE
            -D_FORTIFY_SOURCE=3
            -D_GLIBCXX_ASSERTIONS=1
            -fstack-protector-strong
            -fcf-protection=full
            -fstack-clash-protection
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wcast-qual
            -Wformat=2
            -Wundef
            -Werror=float-equal
            -Wshadow
            -Wcast-align
            -Wunused
            -Wnull-dereference
            -Wdouble-promotion
            -Wimplicit-fallthrough
            -Wextra-semi
            -Woverloaded-virtual
            -Wnon-virtual-dtor
            -Wold-style-cast
        )
    else()
        message(AUTHOR_WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()

    target_compile_options(${target} PRIVATE ${PROJECT_WARNINGS})
endfunction()
