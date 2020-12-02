#pragma once

#include <deque>
#include <vector>

#include "int_types.h"

class Output
{
public:
	virtual ~Output() = default;
	virtual void write(Integer value) = 0;
};


class DequeOutput : public Output
{
public:
	DequeOutput() = default;
	~DequeOutput() = default;

	void write(Integer value) override;

	const auto & getValues() const { return m_values; }

private:
	std::deque<Integer> m_values;
};

class ConsoleOutput : public Output
{
public:
	~ConsoleOutput() = default;

	void write(Integer value) override;

private:
	bool m_firstOut = true;
};