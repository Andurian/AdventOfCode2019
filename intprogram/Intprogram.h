#pragma once

#include <vector>
#include <unordered_map>

#include "Input.h"
#include "Output.h"

#include "int_types.h"


class Intprogram
{
public:
	explicit Intprogram(const Code & code);
	Intprogram(Input & in, Output & out, const Code & code);

	void step();
	void run();
	void terminate() const;

	Integer read(InstructionPointer pos) const;
	Integer readRelative(InstructionPointer pos) const;

	void write(InstructionPointer pos, Integer value);
	void writeRelative(InstructionPointer pos, Integer value);

	Integer readInput() const;
	void writeOutput(Integer value) const;

	void modifyRelativeBase(Integer mod);

	void resetInstructionPointer(InstructionPointer pos = 0) { m_instructionPointer = pos; }

	const Code & getCode() const { return m_code; }

private:
	Input * mep_input;
	Output * mep_output;

	Code m_code;
	InstructionPointer m_instructionPointer;

	mutable std::unordered_map<InstructionPointer, Integer> m_memory;
	Integer m_relativeBase;

	mutable bool m_running;
};
