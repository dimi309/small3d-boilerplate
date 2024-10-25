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

void init();
void initGraphics();
void process(KeyInput& keyInput);
void render();