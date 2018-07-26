cmake_minimum_required(VERSION 3.5.1)
project(c_opentimestamps)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_SKIP_BUILD_RPATH FALSE)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()


file(GLOB SOURCE_FILES "src/*.cpp")
file(GLOB HEADER_FILES  "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} )
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)

# CMake instructions to test using the lib
set( APP_EXE main )
add_executable( ${APP_EXE} main.cpp)
target_link_libraries(${APP_EXE} ${PROJECT_NAME} OpenSSL::Crypto)

