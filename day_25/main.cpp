#include <iostream>
#include <list>
#include <thread>
#include <queue>

#include "Input.h"
#include "Output.h"
#include "Intprogram.h"
#include "enumerate.h"
#include "find_optional.h"
#include "read_file.h"

class AsciiOutput : public Output{
public:
	void write(Integer v) override{
		std::cout << static_cast<char>(v);
	}
};

class InteractiveInput : public Input{
public:
	Integer readNext() override{
		if(m_buffer.empty())
		{
			std::string command;
			std::getline(std::cin, command);

			for (auto c : command)
			{
				m_buffer.push_back(c);
			}
			m_buffer.push_back(10);
		}

		auto v = m_buffer.front();
		m_buffer.pop_front();
		return v;
	}
private:
	std::deque<Integer> m_buffer;
};



int main()
{
	const auto code = read_collection("input.txt", ",", Converter<Integer>{});
	InteractiveInput in;
	AsciiOutput out;
	Intprogram{in, out, code}.run();

	// candy cane, coin, semiconductor, mouse

	return 0;
}