cmake_minimum_required(VERSION 3.26)
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

if(POLICY CMP0177)
    cmake_policy(SET CMP0177 NEW)
endif()
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")

project(oop)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)


include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(WARNINGS_AS_ERRORS OFF)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.1
        GIT_SHALLOW    1
)

FetchContent_MakeAvailable(SFML)


find_package(Threads REQUIRED)
if(UNIX AND NOT APPLE)
    find_package(X11)
endif()


add_executable(${PROJECT_NAME}
        Source/main.cpp
        generated/src/Helper.cpp
        env_fixes.h
        Source/Player.cpp
        header/Player.h
        Source/Enemy.cpp
        header/Enemy.h
        Source/Map.cpp
        header/Map.h
        Source/GameEngine.cpp
        header/GameEngine.h
        Source/Entity.cpp
        header/Entity.h
        Source/Decoy.cpp
        header/Decoy.h
        header/Exceptions.h
        header/EntityPointer.h
        header/GameTemplates.h
        Source/TextureManager.cpp
        header/TextureManager.h
        header/EntityFactory.h
)

include(cmake/CompilerFlags.cmake)


target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE generated/include)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio sfml-window sfml-system Threads::Threads)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE X11)
endif()


install(TARGETS ${PROJECT_NAME} DESTINATION ${DESTINATION_DIR})

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_SOURCE_DIR}/extlibs/bin/x64/openal32.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

include(cmake/CopyHelper.cmake)
copy_files(FILES tastatura.txt)