#pragma once

#include <deque>
#include <string>
#include <vector>

#include "int_types.h"

class Input
{
public:
	class Stopped : public std::exception{};

	virtual ~Input() = default;
	virtual Integer readNext() = 0;
};


class DequeInput : public Input
{
public:
	DequeInput(std::initializer_list<Integer> values);
	DequeInput(const std::string& s);

	~DequeInput() = default;

	Integer readNext() override;

private:
	std::deque<Integer> m_values;
};





