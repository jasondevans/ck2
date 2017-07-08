# Minimum CMake version
cmake_minimum_required(VERSION 3.6)

# Our project
project(ck2)

# Use C++14
set(CMAKE_CXX_STANDARD 14)

# Source files
set(SOURCE_FILES main.cpp Util.cpp Util.h UtilException.cpp UtilException.h Site.cpp Site.h ck_common_includes.h)

# Our executable
add_executable(ck2 ${SOURCE_FILES})

# Include directories
include_directories(/usr/local/opt/openssl/include)

# Our link library dependencies
target_link_libraries(ck2 sqlcipher)