cmake_minimum_required(VERSION 3.1..3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.12)

    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

endif()

project(Labyrinth VERSION 0.1.0
                  DESCRIPTION "A Labyrinth project"
                  LANGUAGES C)

file(GLOB_RECURSE all_c_src
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(LabVM ${all_c_src})

set_property(TARGET LabVM PROPERTY C_STANDARD 11)

target_include_directories(LabVM PUBLIC lib/LabyrinthCUtils/src)

add_subdirectory(lib/LabyrinthCUtils)
target_link_libraries(LabVM PUBLIC LabCUtils)

set(CMAKE_C_FLAGS_RELEASE "-O3 -flto")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)     # 64 bit

    target_compile_definitions(LabVM PUBLIC LAB_64_BIT)

elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32 bit

    target_compile_definitions(LabVM PUBLIC LAB_32_BIT)

endif()