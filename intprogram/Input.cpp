#include "Input.h"

DequeInput::DequeInput(std::initializer_list<Integer> values) : m_values { values }
{
	// empty
}


Integer DequeInput::readNext()
{
	auto ret = m_values.front();
	m_values.pop_front();
	return ret;
}
