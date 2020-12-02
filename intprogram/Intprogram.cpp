#include "Intprogram.h"

#include "Instruction.h"
#include "read_file.h"


Intprogram::Intprogram(const Code & code)
	: mep_input(nullptr)
	, mep_output(nullptr)
	, m_code(code)
	, m_instructionPointer(0)
	, m_memory()
	, m_relativeBase(0)
	, m_running(true)
{
	// empty
}


Intprogram::Intprogram(Input & in, Output & out, const Code & code)
	: mep_input(&in)
	, mep_output(&out)
	, m_code(code)
	, m_instructionPointer(0)
	, m_memory()
	, m_relativeBase(0)
	, m_running(true)
{
	// empty
}


void Intprogram::step()
{
	auto instruction = Instruction::makeInstruction(m_code[m_instructionPointer]);
	m_instructionPointer = instruction->execute(m_instructionPointer, this);
}


void Intprogram::run()
{
	while (m_running)
	{
		step();
	}
}


Integer Intprogram::read(InstructionPointer pos) const
{
	if (pos < m_code.size())
	{
		return m_code[pos];
	}

	if(m_memory.find(pos) == m_memory.end())
	{
		m_memory[pos] = 0;
	}
	return m_memory.at(pos);
}


Integer Intprogram::readRelative(InstructionPointer pos) const
{
	return read(m_relativeBase + read(pos));
}


void Intprogram::write(InstructionPointer pos, Integer value)
{
	if (pos < m_code.size())
	{
		m_code[pos] = value;
	}
	else
	{
		m_memory[pos] = value;
	}
}


void Intprogram::writeRelative(InstructionPointer pos, Integer value)
{
	write(m_relativeBase + read(pos), value);
}


Integer Intprogram::readInput() const
{
	if (!mep_input)
	{
		throw std::runtime_error("No input setup for program to read.");
	}

	try
	{
		return mep_input->readNext();
	}
	catch(const Input::Stopped &)
	{
		terminate();
		return 0;
	}
}


void Intprogram::writeOutput(Integer value) const
{
	if (!mep_output)
	{
		throw std::runtime_error("No output setup for program to write to.");
	}

	mep_output->write(value);
}


void Intprogram::modifyRelativeBase(Integer mod)
{
	m_relativeBase += mod;
}


void Intprogram::terminate() const
{
	m_running = false;
}
