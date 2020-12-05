#pragma once

#include "Intprogram.h"
#include "InterProgramCommunication.h"

bool isInsideBeam(int x, int y, const std::vector<Integer> & code);

int countTractorPoints(int width, int height, const std::vector<Integer> & code);
std::pair<int, int> findClosestPoint(int width, int height, const std::vector<Integer> & code);



