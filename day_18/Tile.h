#pragma once

#include <set>
#include <map>
#include <list>

#include <Eigen/Core>

#include "Direction.h"

#include "n_comparator.h"

using Tile = Eigen::Vector2i;
using TileComparator = NComparator<Tile, 2>;
using TileMap = std::map<Tile, char, TileComparator>;
using TileSet = std::set<Tile, TileComparator>;

Tile nextTile(Tile t, Direction d);


