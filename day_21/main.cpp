#include <iostream>
#include <thread>

#include "Input.h"
#include "Intprogram.h"
#include "Output.h"
#include "read_file.h"

void runScript(const std::string& filenameCode, const std::string& filenameScript)
{
	const auto code = read_collection(filenameCode, ",", Converter<Integer>{});
	const auto script = read_file(filenameScript);

	DequeInput in{ script };
	DequeOutput out;

	Intprogram p{ in, out, code };
	p.run();

	for (const auto c : out.getValues())
	{
		if (c == 10)
		{
			std::cout << std::endl;
		}
		else if(c < 255)
		{
			std::cout << static_cast<char>(c);
		}
		if(c >= 255)
		{
			std::cout << "Damage Report: " << c << std::endl;
		}
	}
}


int main()
{
	// WALK: (NOT(A) OR NOT(B) OR NOT(C)) AND D
	// RUN:  (NOT(A) OR NOT(B) OR NOT(C)) AND D AND (E OR H)
	runScript("input.txt", "script_walk.txt");
	runScript("input.txt", "script_run.txt");

	return 0;
}