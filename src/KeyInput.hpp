/*
 * KeyInput.hpp
 *
 *  Created on: 12 Oct 2019
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once

/**
 * @typedef struct KeyInput
 *
 * @brief   Structure holding the state of keys on the keyboard that are related
 *          to the game.
 */

typedef struct KeyInput {
  bool up = false,
    down = false,
    left = false,
    right = false,
    enter = false,
    esc = false,
    lshift = false,
    prtscr = false,
    space = false;
} KeyInput;

