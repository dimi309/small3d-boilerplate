/*
 * Game.cpp
 *
 *  Created on: 10 May 2022
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include "Game.hpp"
#include "Global.hpp"
#include <small3d/Model.hpp>
#include <small3d/SceneObject.hpp>
#include <small3d/Sound.hpp>
#include <small3d/Time.hpp>

void render();

small3d::Renderer* r;

void process(KeyInput& keyInput) {

#if _WIN32
  if (keyInput.prtscr) {
    r->screenCapture = true;
    keyInput.prtscr = false;
  }
#endif

}

void render() {

  // Rendering transparent surfaces like the sea last ensures that transparency works on
  // some platforms, like MacOS.

  r->swapBuffers();
}


void init() {

}

void initGraphics() {

#if !defined(NDEBUG) && defined(WIN32)
  r = &small3d::Renderer::getInstance(appName, 1024, 768);
#else
  r = &small3d::Renderer::getInstance(appName);
#endif

  r->setBackgroundColour(skyColour);

}


