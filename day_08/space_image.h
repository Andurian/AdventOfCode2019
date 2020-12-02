#pragma once

#include <vector>
#include <string>


std::vector<std::vector<int>> split_layers(const std::vector<int> & image, int width, int height);
std::vector<int> final_color(const std::vector<int> & image, int width, int height);
std::string to_string(const std::vector<int> & image, int width, int height);
