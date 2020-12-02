#pragma once

#include <set>

#include "Tile.h"
#include "Graph.h"
#include "Image.h"

int keyWalk(const Graph & g, Tile start, std::set<char> targetKeys);
int keyWalkImage(const Image & img);