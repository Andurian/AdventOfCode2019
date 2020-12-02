#include "PaintRobot.h"
#include "Intprogram.h"
#include "InterProgramCommunication.h"
#include "read_file.h"
#include "multicall.h"
#include <iostream>
#include <thread>

void runRobot(const std::string& filename, Color startColor)
{
	InterProgramCommunication colorIn;
	InterProgramCommunication instructionOut;

	std::thread t{
		[&]
		{
		  const Code code = read_collection(filename, ",", Converter<Integer>{ });
		  Intprogram{ colorIn, instructionOut, code }.run();
		  colorIn.stop();
		  instructionOut.stop();
		}
	};

	PaintRobot robot{startColor};
	Color initial = robot.currentColor();
	colorIn.write(static_cast<Integer>(initial));
	while (true)
	{
		try
		{
			auto c = static_cast<Color>(instructionOut.readNext());
			auto t = static_cast<Turn>(instructionOut.readNext());

			Color c2 = robot.process(c, t);

			colorIn.write(static_cast<Integer>(c2));
		}
		catch (InterProgramCommunication::Stopped &)
		{
			break;
		}
	}

	std::cout << "Painted: " << robot.getTiles().size() << std::endl << robot.getImage() << std::endl;

	t.join();
}

void solve(const std::string& filename)
{
	runRobot(filename, Black);
	runRobot(filename, White);
}

int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
