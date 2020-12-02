#include "amplifiers.h"

#include <sstream>
#include <thread>
#include <algorithm>
#include <list>

#include "Intprogram.h"
#include "InterProgramCommunication.h"


Integer amplifier_signal(const std::vector<Integer> & phases, const Code & code)
{
	Integer inputSignal = 0;

	for (int i = 0; i < static_cast<int>(phases.size()); ++i)
	{
		DequeInput in{ phases[i], inputSignal };
		DequeOutput out;

		Intprogram{ in, out, code }.run();

		inputSignal = out.getValues()[0];
	}

	return inputSignal;
}


Integer max_amplifier_signal(int numAmplifiers, const Code & code)
{
	std::vector<Integer> phases(5);
	std::generate(phases.begin(), phases.end(), [n = 0]() mutable { return n++; });

	auto maxSignal = std::numeric_limits<Integer>::lowest();

	do
	{
		const auto signal = amplifier_signal(phases, code);
		maxSignal = std::max(maxSignal, signal);
	} while (std::next_permutation(phases.begin(), phases.end()));

	return maxSignal;
}


Integer amplifier_signal_loop(const std::vector<Integer> & phases, const Code & code)
{
	const int numAmplifiers = static_cast<int>(phases.size());
	std::vector<InterProgramCommunication> communications(numAmplifiers);

	for (int i = 0; i < numAmplifiers; ++i)
	{
		communications[i].write(phases[i]);
	}

	communications[0].write(0);

	std::list<std::thread> threads;

	for (int i = 0; i < numAmplifiers; ++i)
	{
		threads.emplace_back([&communications, i, numAmplifiers, code]
		{
			Intprogram{ communications[i], communications[(i + 1) % numAmplifiers], code }.run();
		});
	}

	for(auto & t : threads)
	{
		t.join();
	}

	return communications[0].readNext();
}


Integer max_amplifier_signal_loop(std::vector<Integer> possiblePhases, const Code & code)
{
	auto maxSignal = std::numeric_limits<Integer>::lowest();

	do
	{
		const auto signal = amplifier_signal_loop(possiblePhases, code);
		maxSignal = std::max(maxSignal, signal);
	} while (std::next_permutation(possiblePhases.begin(), possiblePhases.end()));

	return maxSignal;
}
