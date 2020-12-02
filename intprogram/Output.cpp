#include "Output.h"
#include <iostream>


void DequeOutput::write(Integer value)
{
	m_values.push_back(value);
}


void ConsoleOutput::write(Integer value)
{
	if(!m_firstOut)
	{
		std::cout << " ";
	}
	else
	{
		m_firstOut = false;
	}
	std::cout << value;
}
