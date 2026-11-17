add_library(ufbxw_strict_c STATIC ../../ufbx_write.c)
set_target_properties(ufbxw_strict_c PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

add_library(ufbxw_strict_cpp STATIC strict_cpp.cpp)
set_target_properties(ufbxw_strict_cpp PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

set(UFBXW_STRICT_WARNINGS_GNU
    -Wall
    -Wextra
    -Werror
    -Wsign-conversion
    -Wshadow
    -Wundef
)

set(UFBXW_STRICT_WARNINGS_CLANG
    ${UFBXW_STRICT_WARNINGS_GNU}
    -ferror-limit=1000
    -Wunreachable-code-break
    -Wmissing-variable-declarations
    -Wfloat-conversion
    -Wextra-semi-stmt
    -Wimplicit-int-float-conversion
    -Wimplicit-int-conversion
)

set(UFBXW_STRICT_WARNINGS_GCC
    ${UFBXW_STRICT_WARNINGS_GNU}
    -Wswitch-default
    -Wconversion
    -Wduplicated-cond
    -Warith-conversion
)

set(UFBXW_STRICT_WARNINGS_C
    -Wmissing-prototypes
    -Wstrict-prototypes
)

set(UFBXW_STRICT_WARNINGS_CPP
    -Wconversion-null
)

if(MSVC)
    target_compile_options(ufbxw_strict_c PRIVATE /W4 /WX)
    target_compile_options(ufbxw_strict_cpp PRIVATE /W4 /WX)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(ufbxw_strict_c PRIVATE ${UFBXW_STRICT_WARNINGS_CLANG} ${UFBXW_STRICT_WARNINGS_C})
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(ufbxw_strict_c PRIVATE ${UFBXW_STRICT_WARNINGS_GCC} ${UFBXW_STRICT_WARNINGS_C})
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(ufbxw_strict_cpp PRIVATE
        ${UFBXW_STRICT_WARNINGS_CLANG}
        ${UFBXW_STRICT_WARNINGS_CPP}
        -Wclass-varargs
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(ufbxw_strict_cpp PRIVATE ${UFBXW_STRICT_WARNINGS_GCC} ${UFBXW_STRICT_WARNINGS_CPP})
endif()

target_compile_definitions(ufbxw_strict_c PRIVATE UFBXW_DEV=1 UFBX_DEV=1)
target_compile_definitions(ufbxw_strict_cpp PRIVATE UFBXW_DEV=1 UFBX_DEV=1)
