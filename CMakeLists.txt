cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND ENV{CMAKE_TOOLCHAIN_FILE})
    set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

project(ProfileCurves_build)
set(TargetName ProfileCurves)

find_package(NGL CONFIG REQUIRED)
find_package(Qt6 COMPONENTS OpenGL Widgets QUIET)
if (Qt6_FOUND)
    message("Found Qt6 using that")
else()
    message ("Found Qt5 Using that")
    find_package(Qt5 COMPONENTS OpenGL Widgets REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(${TargetName})
target_include_directories(${TargetName} PRIVATE include)
target_sources(${TargetName} PRIVATE
        src/main.cpp
        src/ControlPoints.cpp
        include/ControlPoints.h
        src/NGLScene.cpp
        include/NGLScene.h
        src/NGLSceneMouseControls.cpp
        src/Mesh.cpp
        include/Mesh.h


)
target_link_libraries(${TargetName} PRIVATE Qt::Widgets Qt::OpenGL NGL)

## Testing Code

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)
add_executable(${TargetName}Tests)

## Test for ControlPointsClass
target_include_directories(${TargetName}Tests PRIVATE include)
target_sources(${TargetName}Tests PRIVATE
        tests/ControlPointsTests.cpp
        src/ControlPoints.cpp
        include/ControlPoints.h
)

## Test for MeshClass
target_include_directories(${TargetName}Tests PRIVATE include)
target_sources(${TargetName}Tests PRIVATE
        tests/MeshTests.cpp
        src/Mesh.cpp
        include/Mesh.h
)


target_link_libraries(${TargetName}Tests PRIVATE GTest::gtest GTest::gtest_main )
gtest_discover_tests(${TargetName}Tests)

add_custom_target(${TargetName}CopyResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        $<TARGET_FILE_DIR:${TargetName}>/models

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${TargetName}>/shaders

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/fonts
        $<TARGET_FILE_DIR:${TargetName}>/fonts

)
