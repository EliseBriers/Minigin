#pragma once


#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

// #pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
// #pragma warning(pop)

// Engine stuff
#include <IComponent.h>
#include <GameObject.h>
#include <TransformComponent.h>