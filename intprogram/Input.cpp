#include "Input.h"

DequeInput::DequeInput(std::initializer_list<Integer> values) : m_values { values }
{
	// empty
}

DequeInput::DequeInput(const std::string & s) {
	for(const auto c : s)
	{
		if(c == '\n')
		{
			m_values.push_back(10);
		}
		else
		{
			m_values.push_back(static_cast<Integer>(c));
		}
	}
}


Integer DequeInput::readNext()
{
	auto ret = m_values.front();
	m_values.pop_front();
	return ret;
}


