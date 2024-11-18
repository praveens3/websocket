cmake_minimum_required(VERSION  3.10.0)

# Define the main project
project(Protocol_Solution VERSION 0.1.0.0 DESCRIPTION "C++ and C# Protocol Project" LANGUAGES C CXX CSharp)

# Set the C++ standard to 17
set(CMAKE_CXX_STANDARD 17)

# conan extraction
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_BINARY_DIR}/generators/")
find_package(spdlog REQUIRED)
find_package(libwebsockets REQUIRED)
#find_package(OpenSSL REQUIRED)
message(STATUS "libwebsockets_LIBRARIES: ${websockets_LIBRARIES}")
message(STATUS "libwebsockets_INCLUDE_DIRS: ${websockets_INCLUDE_DIRS}")

# Logger static lib
file(GLOB Logger_SOURCES src/Logger/*.cpp src/Logger/*.h)
add_library(Logger STATIC ${Logger_SOURCES})
target_link_libraries(Logger PUBLIC spdlog::spdlog)
set_property(TARGET Logger PROPERTY FOLDER "Libraries")

# WebSocketServer app
file(GLOB WebSocketServer_SOURCES src/WebSocketServer/*.cpp src/WebSocketServer/*.h)
add_executable(WebSocketServer ${WebSocketServer_SOURCES})
target_link_libraries(WebSocketServer PRIVATE websockets Logger)
target_include_directories(WebSocketServer PRIVATE ${CMAKE_SOURCE_DIR}/src/Logger)
target_link_libraries(WebSocketServer  PRIVATE openssl::openssl)
set_property(TARGET WebSocketServer PROPERTY FOLDER "Applications")

# Test app C# executable websocket client
# file(GLOB_RECURSE TestApp_SOURCES "src/TestApp/*.cs" "src/TestApp/*.resx")
# set(TestApp_SOURCES ${TestApp_SOURCES})
# #set(TestApp_SOURCES src/TestApp/TestApp.cs)
# add_executable(TestApp ${TestApp_SOURCES})
# set_target_properties(TestApp PROPERTIES
#     VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.8"
#     WIN32_EXECUTABLE TRUE
#     VS_GLOBAL_KEYWORD "WindowsApp"
# )
# set_property(TARGET TestApp PROPERTY FOLDER "Applications")