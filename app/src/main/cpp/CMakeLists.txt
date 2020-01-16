# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


include_directories(./mediaserver/src/base/include ./mediaserver/src/http_parser ./mediaserver/src/crypto/include ./mediaserver/src/json/include ./mediaserver/src/net/include ./mediaserver/src/http/include ./mediaserver/src/http/speedTest/ ./mediaserver/src/signal/include ./mediaserver/src/libuv/include ./mediaserver/src/libuv/src ./mediaserver/src/libuv/src/unix )

file(GLOB SOURCES "*.cpp"  "./mediaserver/src/libuv/src/*.cpp" "./mediaserver/src/http_parser/*.cpp" "./mediaserver/src/libuv/src/unix/*.cpp" "./mediaserver/src/base/src/*.cpp" "./mediaserver/src/net/src/TcpC*.cpp" "./mediaserver/src/net/src/U*.cpp" "./mediaserver/src/net/src/IP.cpp" "./mediaserver/src/http/src/p*.cpp" "./mediaserver/src/http/src/r*.cpp" "./mediaserver/src/http/src/u*.cpp" "./mediaserver/src/http/src/f*.cpp" "./mediaserver/src/http/speedTest/Speed*.cpp" "./mediaserver/src/signal/src/*.cpp" "./mediaserver/src/http/src/w*.cpp" "./mediaserver/src/http/src/m*.cpp" "./mediaserver/src/http/src/HttpClient*.cpp" "./mediaserver/src/http/src/c*.cpp"  )
#
#file(GLOB SOURCES "*.cpp")

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${SOURCES})
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.



target_link_libraries(native-lib
        android
        log)
