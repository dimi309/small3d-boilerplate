/*
 * Game.hpp
 *
 *  Created on: 10 May 2022
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once
#include "GameLoopAndInput.hpp"
#include <small3d/Renderer.hpp>

#if defined(__ANDROID__)
void init(struct android_app* state);
void toURL(std::string url);
#else
void init();
#endif

#if defined(SMALL3D_IOS) && defined(SMALL3D_OPENGLES)
void initGraphics(uint32_t width, uint32_t height, GLint framebuffer, GLint renderbuffer);
#else
  void initGraphics();
#endif

void process(KeyInput& keyInput);
void render();

#if defined(__ANDROID__) || defined(SMALL3D_IOS)
void pauseGraphics();
void resumeGraphics();
void getScreenInfo();
#endif
