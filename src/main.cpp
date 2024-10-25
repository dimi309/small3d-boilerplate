/*
 * main.cpp
 *
 *  Created on: 9 Oct 2021
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */
#include "Game.hpp"
#include <exception>
#include "Global.hpp"

int main(int argc, char* argv[]) {
  try {
    init();
    initGraphics();
    GameLoopAndInput::init(r->getWindow(), process, render);
    GameLoopAndInput::run(60);
  }
  catch (std::exception& e) {
    LOGERROR(std::string(e.what()));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

