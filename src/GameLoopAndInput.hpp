/*
 * GameLoopAndInput.hpp
 *
 *  Created on: 9 Oct 2021
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#pragma once

// see https://stackoverflow.com/questions/15328751/android-macro-suddenly-not-defined
// (I am not sure it is relevant to the code below :| )

#if !defined(__ANDROID__) && !defined(SMALL3D_IOS)
// The Renderer just included here so that the glfw and glew inclusions appear in the right
// sequence the first time they are included in the program, avoiding the "gl.h included
// before glew error
#include <small3d/Renderer.hpp>
#include <GLFW/glfw3.h>
#endif

#include "KeyInput.hpp"

#if defined(__ANDROID__)
#include <cstdint>
#include <android_native_app_glue.h>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/configuration.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <iostream>
#include <unistd.h>
#include <small3d/Logger.hpp>
#include "small3d_android.h"
#endif

class GameLoopAndInput {
private:
  GameLoopAndInput() {};
  static void (*process)(KeyInput& keyInput);
  static void (*render)();
  static bool stop_;
#if !defined(__ANDROID__) && !defined(SMALL3D_IOS)
  static GLFWwindow *window;
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
    int mods);
#elif defined(__ANDROID__)
  static struct android_app *state;
#endif
  static KeyInput input;
public:
#if !defined(__ANDROID__) && !defined(SMALL3D_IOS)
  static void init(GLFWwindow *window, void (*process_function)(KeyInput&), void (*render_function)());
#else
  static float screenWidth, screenHeight;
  static KeyInput keyInput;
  static bool appActive;
  static bool instantiated;
  static void (*init_graphics)();
  static void (*pause_graphics)();
  static void (*resume_graphics)();
  static void (*get_screen_info)();
  
  static void interpret_input(float xval, float yval, bool keyValue, bool reset);

#if defined(SMALL3D_IOS)
  static void init(void (*process_function)(KeyInput&), void (*render_function)(),
                   void (*init_graphics_function)(),
                   void (*pause_graphics_function)(),
                   void (*resume_graphics_function)(),
                   void (*get_screen_info_function)());
  static void handle_input(int x, int y, bool isTouched, bool reset);
  static bool deactivatedOnce;
#endif
#if defined(__ANDROID__)
  static void init( struct android_app *state,
      void (*process_function)(KeyInput&), void (*render_function)(),
                   void (*init_graphics_function)(),
                   void (*pause_graphics_function)(),
                   void (*resume_graphics_function)(),
                   void (*get_screen_info_function)());

  static JNIEnv *env;
  static jobject tm;
  static jmethodID adjustVolume;
  
  static int events;
  static android_poll_source *pSource;
  static void initVolumeControl();
  static void handle_cmd(android_app *pApp, int32_t cmd);
  static int32_t handle_input(android_app *app, AInputEvent *event);
#endif
#endif
  static void run(const int fps);
  static void stop();
};

  
