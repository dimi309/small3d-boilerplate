cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_library(log-lib
        log)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../src/*.c*)
file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../src/*.h*)

add_compile_definitions(SMALL3D_OPENGL)
add_compile_definitions(SMALL3D_OPENGLES)

MESSAGE(STATUS "Sources: ${SOURCE_FILES} ${HEADER_FILES}")

find_package (oboe REQUIRED CONFIG)

add_library(game
        SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES})

target_include_directories(game PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include
        ${ANDROID_NDK}/sources/android/native_app_glue
        )

target_link_libraries(game
        android
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libnative_app_glue.a
        ${log-lib}
	    GLESv2
        EGL
		oboe::oboe
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libpng.a
        z
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libvorbis.a
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libvorbisfile.a
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libogg.a
		${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libfreetype.a
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libsmall3d.a
        )
