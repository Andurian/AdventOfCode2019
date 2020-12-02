#include "Instruction.h"

#include <iostream>

#include "Intprogram.h"

#include "read_file.h"


Instruction::Identifier::Identifier(Integer i)
	: opcode{ static_cast<Opcode>(i % 100) }
	, first{ static_cast<ParameterMode>(i / 100 % 10) }
	, second{ static_cast<ParameterMode>(i / 1000 % 10) }
	, third{ static_cast<ParameterMode>(i / 10000 % 10) }
{
	// empty
}


std::unique_ptr<Instruction> Instruction::makeInstruction(Identifier id)
{
	switch (id.opcode)
	{
		case ADD:
			return std::make_unique<InstructionAdd>(
				InputParameter::make(id.first, 1), InputParameter::make(id.second, 2), OutputParameter::make(id.third, 3));
		case MUL:
			return std::make_unique<InstructionMul>(
				InputParameter::make(id.first, 1), InputParameter::make(id.second, 2), OutputParameter::make(id.third, 3));
		case INPUT:
			return std::make_unique<InstructionInput>(OutputParameter::make(id.first, 1));
		case OUTPUT:
			return std::make_unique<InstructionOutput>(InputParameter::make(id.first, 1));
		case JUMP_IF_TRUE:
			return std::make_unique<InstructionJumpIfTrue>(
				InputParameter::make(id.first, 1), InputParameter::make(id.second, 2));
		case JUMP_IF_FALSE:
			return std::make_unique<InstructionJumpIfFalse>(
				InputParameter::make(id.first, 1), InputParameter::make(id.second, 2));
		case LESS_THAN:
			return std::make_unique<InstructionLessThan>(
				InputParameter::make(id.first, 1), InputParameter::make(id.second, 2), OutputParameter::make(id.third, 3));
		case EQUALS:
			return std::make_unique<InstructionEquals>(
				InputParameter::make(id.first, 1), InputParameter::make(id.second, 2), OutputParameter::make(id.third, 3));
		case ADJUST_BASE:
			return std::make_unique<InstructionAdjustBase>(
				InputParameter::make(id.first, 1));
		case END:
			return std::make_unique<InstructionEnd>();
		default:
			std::cout << "INVALID OPCODE" << std::endl;
			throw std::runtime_error("Invalid Opcode");
	}
}


InstructionAdd::InstructionAdd(InputParameterPtr getInput1, InputParameterPtr getInput2, OutputParameterPtr writeOutput)
	: m_getInput1{ std::move(getInput1) }
	, m_getInput2{ std::move(getInput2) }
	, m_writeOutput{ std::move(writeOutput) }
{
	// empty
}


InstructionPointer InstructionAdd::execute(InstructionPointer instructionPointer, Intprogram * program)
{
	const auto input1 = m_getInput1->get(instructionPointer, program);
	const auto input2 = m_getInput2->get(instructionPointer, program);

	m_writeOutput->write(instructionPointer, program, input1 + input2);

	return instructionPointer + 4;
}


InstructionMul::InstructionMul(InputParameterPtr getInput1, InputParameterPtr getInput2, OutputParameterPtr writeOutput)
	: m_getInput1{ std::move(getInput1) }
	, m_getInput2{ std::move(getInput2) }
	, m_writeOutput{ std::move(writeOutput) }
{
	// empty
}


InstructionPointer InstructionMul::execute(InstructionPointer instructionPointer, Intprogram * program)
{
	const auto input1 = m_getInput1->get(instructionPointer, program);
	const auto input2 = m_getInput2->get(instructionPointer, program);

	m_writeOutput->write(instructionPointer, program, input1 * input2);

	return instructionPointer + 4;
}


InstructionInput::InstructionInput(OutputParameterPtr writeOutput)
	: m_writeOutput{ std::move(writeOutput) }
{
	// empty
}


InstructionPointer InstructionInput::execute(InstructionPointer instructionPointer, Intprogram * program)
{
	m_writeOutput->write(instructionPointer, program, program->readInput());

	return instructionPointer + 2;
}


InstructionOutput::InstructionOutput(InputParameterPtr getInput)
	: m_getInput{ std::move(getInput) }
{
	// empty
}


InstructionPointer InstructionOutput::execute(InstructionPointer instructionPointer, Intprogram * program)
{
	program->writeOutput(m_getInput->get(instructionPointer, program));
	return instructionPointer + 2;
}


InstructionJumpIfTrue::InstructionJumpIfTrue(InputParameterPtr getInput1, InputParameterPtr getInput2)
	: m_getInput1{ std::move(getInput1) }
	, m_getInput2{ std::move(getInput2) }
{
	// empty
}


InstructionPointer InstructionJumpIfTrue::execute(InstructionPointer instructionPointer, Intprogram * program)
{
	const auto input = m_getInput1->get(instructionPointer, program);
	if (input != 0)
	{
		return m_getInput2->get(instructionPointer, program);
	}
	return instructionPointer + 3;
}


InstructionJumpIfFalse::InstructionJumpIfFalse(InputParameterPtr getInput1, InputParameterPtr getInput2)
	: m_getInput1{ std::move(getInput1) }
	, m_getInput2{ std::move(getInput2) }
{
	// empty
}


InstructionPointer InstructionJumpIfFalse::execute(InstructionPointer instructionPointer, Intprogram * program)
{
	const auto input = m_getInput1->get(instructionPointer, program);
	if (input == 0)
	{
		return m_getInput2->get(instructionPointer, program);
	}
	return instructionPointer + 3;
}


InstructionLessThan::InstructionLessThan(InputParameterPtr getInput1, InputParameterPtr getInput2, OutputParameterPtr writeOutput)
	: m_getInput1{ std::move(getInput1) }
	, m_getInput2{ std::move(getInput2) }
	, m_writeOutput{ std::move(writeOutput) }
{
	// empty
}


InstructionPointer InstructionLessThan::execute(InstructionPointer instructionPointer, Intprogram * program)
{
	const auto input1 = m_getInput1->get(instructionPointer, program);
	const auto input2 = m_getInput2->get(instructionPointer, program);

	const Integer output = (input1 < input2) ? 1 : 0;

	m_writeOutput->write(instructionPointer, program, output);

	return instructionPointer + 4;
}


InstructionEquals::InstructionEquals(InputParameterPtr getInput1, InputParameterPtr getInput2, OutputParameterPtr writeOutput)
	: m_getInput1{ std::move(getInput1) }
	, m_getInput2{ std::move(getInput2) }
	, m_writeOutput{ std::move(writeOutput) }
{
	// empty
}


InstructionPointer InstructionEquals::execute(InstructionPointer instructionPointer, Intprogram * program)
{
	const auto input1 = m_getInput1->get(instructionPointer, program);
	const auto input2 = m_getInput2->get(instructionPointer, program);

	const Integer output = (input1 == input2) ? 1 : 0;

	m_writeOutput->write(instructionPointer, program, output);

	return instructionPointer + 4;
}


InstructionAdjustBase::InstructionAdjustBase(InputParameterPtr getInput)
	: m_getInput{ std::move(getInput) }
{
	// empty
}


InstructionPointer InstructionAdjustBase::execute(InstructionPointer instructionPointer, Intprogram * program)
{
	const auto input = m_getInput->get(instructionPointer, program);
	program->modifyRelativeBase(input);
	return instructionPointer + 2;
}


InstructionPointer InstructionEnd::execute(InstructionPointer instructionPointer, Intprogram * program)
{
	program->terminate();
	return instructionPointer + 1;
}
