cmake_minimum_required(VERSION 3.19)

project(WindowsServiceExample
        LANGUAGES CXX C
        VERSION 1.0)

include(FetchContent)
FetchContent_Declare(
        LibWindowsService
        GIT_REPOSITORY https://github.com/Allar/windows-service
        GIT_TAG        1.0
)
FetchContent_MakeAvailable(LibWindowsService)


set(EXE_TARGET_NAME WindowsServiceExample)

set(${EXE_TARGET_NAME}_SRC
        main.cpp
)

add_executable(${EXE_TARGET_NAME} ${${EXE_TARGET_NAME}_SRC})

target_link_libraries(${EXE_TARGET_NAME} LibWindowsService)