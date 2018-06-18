cmake_minimum_required(VERSION 3.8)
project(c_opentimestamps)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_SKIP_BUILD_RPATH FALSE)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()


add_executable(c_opentimestamps src/Op.h main.cpp src/Timestamp.h src/Attestation.h src/Timestamp.cpp src/DetachedFile.h src/Common.h)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)