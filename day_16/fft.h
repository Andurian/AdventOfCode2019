#pragma once

#include <string>
#include <vector>

std::vector<int> getPositiveIndices(int round, int maxIndex);
std::vector<int> getNegativeIndices(int round, int maxIndex);

std::vector<int> toIntVector(const std::string i);
int round(const std::vector<int> & v, int round);
std::string phase(const std::string & input);
std::string fft(std::string input, int numPhases);

std::string decode(std::string input, int numRepeats, int numPhases);