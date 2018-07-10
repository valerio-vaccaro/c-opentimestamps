cmake_minimum_required(VERSION 3.8)
project(c_opentimestamps)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_SKIP_BUILD_RPATH FALSE)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()


file(GLOB SOURCE_FILES "src/*.cpp")
file(GLOB HEADER_FILES  "src/*.h")
add_executable(c_opentimestamps main.cpp ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)