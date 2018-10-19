cmake_minimum_required(VERSION 2.8)
project(OHO)

file(GLOB_RECURSE formatted
    *.cpp
    *.h
)

option(OHO_WITH_CLANG_FORMAT "use clang-format to format the source code" ON)
if(OHO_WITH_CLANG_FORMAT)
    find_program(OHO_CLANG_FORMAT NAMES clang-format-3.7 clang-format PATHS "C:/Program Files/LLVM/bin")
    if(OHO_CLANG_FORMAT)
        add_custom_target(clang-format COMMAND "${OHO_CLANG_FORMAT}" -i ${formatted} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
        option(OHO_WITH_CLANG_FORMAT_AUTO "format the code automatically before the build" OFF)
    endif()
endif()

if(MSVC)
    #avoid bogus warnings in Windows headers
    add_definitions(/DNTDDI_WIN7SP1=0)
    add_definitions(/DWINAPI_PARTITION_SYSTEM=0)
    add_definitions(/D_WIN32_WINNT_WIN10_TH2=0)
    add_definitions(/D_WIN32_WINNT_WIN10_RS1=0)
    add_definitions(/D_WIN32_WINNT_WIN10_RS2=0)
    add_definitions(/D_WIN32_WINNT_WIN10_RS3=0)
    add_definitions(/D_WIN32_WINNT_WIN10_RS4=0)
    add_definitions(/Qspectre)
    add_definitions(/D_CRT_SECURE_NO_WARNINGS)
    set(OHO_COMPILE_FLAGS ${OHO_COMPILE_FLAGS} /Wall /wd4820 /wd4255 /wd4201 /wd4204 /wd4710 /wd4711 /wd4221 /wd5045 /WX)
else()
    set(OHO_COMPILE_FLAGS ${OHO_COMPILE_FLAGS}
        -std=c++17 -Wall -Wextra -Wconversion -Werror -Wunused -Wundef -Wvla -Wshadow -Wpedantic
        -Wdouble-promotion -Wformat -Wswitch-enum -Wuninitialized -Wstrict-overflow=5)
endif()

file(GLOB sources *.cpp *.h)
add_executable(OHO ${sources})
target_compile_options(OHO PUBLIC ${OHO_COMPILE_FLAGS})
add_dependencies(OHO clang-format)
