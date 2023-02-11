#pragma once
#include <string>
#include <memory>
#include <small3d/Renderer.hpp>

#if defined(SMALL3D_IOS)
const std::string resourceDir = "resources1";
#else
const std::string resourceDir = "resources";
#endif
const glm::vec4 skyColour(0.5f, 0.5f, 7.0f, 1.0f);
extern small3d::Renderer* r;
const std::string appName = "Game";

