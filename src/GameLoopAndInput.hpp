/*
 * GameLoopAndInput.hpp
 *
 *  Created on: 9 Oct 2021
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#pragma once

// The Renderer is just included here so that the glfw and glew inclusions appear in the right
// sequence the first time they are included in the program, avoiding the "gl.h included
// before glew error
#include <small3d/Renderer.hpp>
#include <GLFW/glfw3.h>

#include "KeyInput.hpp"

class GameLoopAndInput {
private:
  GameLoopAndInput() {};
  static void (*process)(KeyInput& keyInput);
  static void (*render)();
  static bool stop_;
  static GLFWwindow *window;
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
    int mods);
  static KeyInput input;
public:
  static void init(GLFWwindow *window, void (*process_function)(KeyInput&), void (*render_function)());
  static void run(const int fps);
  static void stop();
};

  
