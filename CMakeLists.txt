cmake_minimum_required(VERSION 3.19)

project(WindowsServiceExample
        LANGUAGES CXX C
        VERSION 1.0)

# if(MSVC)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
# else(MSVC)
# target_compile_features(WindowsServiceExample PRIVATE cxx_std_17)
# endif(MSVC)

include(FetchContent)
FetchContent_Declare(
        LibWindowsService
        GIT_REPOSITORY https://github.com/Allar/windows-service
        GIT_TAG        0.1
)
FetchContent_MakeAvailable(LibWindowsService)


set(EXE_TARGET_NAME WindowsServiceExample)

set(${EXE_TARGET_NAME}_SRC
        main.cpp
)

add_executable(${EXE_TARGET_NAME} ${${EXE_TARGET_NAME}_SRC})

target_link_libraries(${EXE_TARGET_NAME} LibWindowsService)