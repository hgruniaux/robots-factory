#pragma once

#include <box2d/box2d.h>
#include <string>

/**
 * Loads a world description from a file and creates the world in the given Box2D world.
 */
bool load_world_description(b2World *world, const std::string &path);
