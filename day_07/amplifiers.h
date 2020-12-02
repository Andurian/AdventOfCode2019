#pragma once

#include <vector>

#include "int_types.h"


Integer amplifier_signal(const std::vector<Integer> & phases, const Code & code);
Integer max_amplifier_signal(int numAmplifiers, const Code & code);

Integer amplifier_signal_loop(const std::vector<Integer> & phases, const Code & code);
Integer max_amplifier_signal_loop(std::vector<Integer> possiblePhases, const Code & code);
